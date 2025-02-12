/**
***************************************************************************************************
* This file is part of ICLED SDK:
*
*
* THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE THAT WÜRTH ELEKTRONIK
* EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY KIND RELATED TO, BUT NOT LIMITED
* TO THE NON-INFRINGEMENT OF THIRD PARTIES’ INTELLECTUAL PROPERTY RIGHTS OR THE
* MERCHANTABILITY OR FITNESS FOR YOUR INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT
* WARRANT OR REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY PATENT
* RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY RIGHT RELATING TO ANY
* COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT IS USED. INFORMATION PUBLISHED BY
* WÜRTH ELEKTRONIK EISOS REGARDING THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE
* FROM WÜRTH ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR ENDORSEMENT
* THEREOF
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS DRIVER PACKAGE.
*
* COPYRIGHT (c) 2025 Würth Elektronik eiSos GmbH & Co. KG
*
***************************************************************************************************
**/

#include <SPI.h>
#include "ICLED_48bit.h"
#include "ConfigPlatform.h"
#include "debug.h"
#include "global.h"

/**
 * @brief       Applies the current LED buffer from the specified start column to the
 *              ICLED board by copying the LED buffer to the DMA buffer.
 *
 * @return      None
 */
static void write_ledbuffer_to_DMAbuffer();

static ICLED_Color_System ColorSystem = RGB;

static uint8_t dmaBuf[ICLED_BYTESTOTAL]; // The raw buffer we write to SPI
static Adafruit_ZeroDMA dma; ///< The DMA manager for the SPI class
static SPIClass *spi;        ///< Underlying SPI hardware interface we use to DMA

#define MIN_LOOP_DELAY_MS 5
#define OFFSET 1

typedef union
{
    struct
    {
        uint16_t R;
        uint16_t G;
        uint16_t B;
    };
    uint16_t RGB[3];
} Pixel;

// Buffer for LEDs --> will be written into dmaBuf after bit-expansion
static Pixel LEDBuf[ICLED_BYTESTOTAL];

// Bit patterns for data encoding
#define ZEROPATTERN 0x8  // 4-bit pattern for logical "0"
#define ONEPATTERN  0xE  // 4-bit pattern for logical "1"

#define ZEROZEROPATTERN ((ZEROPATTERN << 4) | ZEROPATTERN) // 10001000
#define ZEROONEPATTERN  ((ZEROPATTERN << 4) | ONEPATTERN)  // 10001110
#define ONEZEROPATTERN  ((ONEPATTERN << 4) | ZEROPATTERN)  // 11101000
#define ONEONEPATTERN   ((ONEPATTERN << 4) | ONEPATTERN)   // 11101110

bool ICLED_Init(ICLED_Color_System color_system)
{
    // Set color system to given color system
    ColorSystem = color_system;

    // Clear buffer and set all values to zero
    ICLED_clear();

    spi = new SPIClass(&sercom5, ICLED_DIN_PIN, ICLED_DIN_PIN, ICLED_DIN_PIN, SPI_PAD_2_SCK_3, SERCOM_RX_PAD_1);
    spi->begin();

    if (pinPeripheral(ICLED_DIN_PIN, PIO_SERCOM) < 0)
    {
        WE_DEBUG_PRINT("Problem changing pin %d configuration.\r\n", ICLED_DIN_PIN);
        return false;
    }

    dma.setTrigger(SERCOM5_DMAC_ID_TX);
    dma.setAction(DMA_TRIGGER_ACTON_BEAT);

    if (dma.allocate() != DMA_STATUS_OK)
    {
        WE_DEBUG_PRINT("Failed to allocate DMA channel.\r\n");
        return false;
    }

    if (dma.addDescriptor(dmaBuf, (void *)(&SERCOM5->SPI.DATA.reg), ICLED_BYTESTOTAL, DMA_BEAT_SIZE_BYTE, true, false) == NULL)
    {
        WE_DEBUG_PRINT("Failed to allocate DMA descriptor.\r\n");
        return false;
    }

    dma.loop(true);

    spi->beginTransaction(
        SPISettings(3200000, MSBFIRST, SPI_MODE0));

    if (dma.startJob() != DMA_STATUS_OK)
    {
        WE_DEBUG_PRINT("Failed to start DMA job.\r\n");
        return false;
    }

    return true;
}

bool ICLED_Deinit()
{
    // Clear buffer and set all values to zero
    ICLED_clear();

    dma.abort();

    if (dma.free() != DMA_STATUS_OK)
    {
        WE_DEBUG_PRINT("Failed to free DMA channel.\r\n");
        return false;
    }

    spi->endTransaction();
    delete spi;

    return true;
}

void ICLED_set_color_system(ICLED_Color_System color_system)
{
    ColorSystem = color_system;
}

ICLED_Color_System ICLED_get_color_system()
{
    return ColorSystem;
}

bool ICLED_set_pixel(uint16_t pixel_number, uint16_t R, uint16_t G, uint16_t B, bool write_buffer)
{
    if (pixel_number >= ICLED_NUM)
    {
        WE_DEBUG_PRINT("Pixel index %d is out of range.\r\n", pixel_number);
        return false;
    }

    if (R > 0xFFFF || G > 0xFFFF || B > 0xFFFF)
    {
        WE_DEBUG_PRINT("RGB values should be between 0-65535.\r\n");
        return false;
    }

    LEDBuf[pixel_number].R = R;
    LEDBuf[pixel_number].G = G;
    LEDBuf[pixel_number].B = B;

    if (write_buffer)
    {
        write_ledbuffer_to_DMAbuffer();
    }

    return true;
}

static void write_ledbuffer_to_DMAbuffer()
{
    for (int i = 0; i < ICLED_NUM; i++)
    {
        for (uint8_t colorIdx = 0; colorIdx < 3; colorIdx++)
        {
            for (uint8_t bitIdx = 0; bitIdx < 8; bitIdx++)
            {
                switch ((LEDBuf[i].RGB[colorIdx] << (2 * bitIdx)) & 0xC000)
                {
                case 0x0000:
                    dmaBuf[(4 * 6) * i + 8 * colorIdx + bitIdx] = ZEROZEROPATTERN;
                    break;
                case 0x4000:
                    dmaBuf[(4 * 6) * i + 8 * colorIdx + bitIdx] = ZEROONEPATTERN;
                    break;
                case 0x8000:
                    dmaBuf[(4 * 6) * i + 8 * colorIdx + bitIdx] = ONEZEROPATTERN;
                    break;
                case 0xC000:
                    dmaBuf[(4 * 6) * i + 8 * colorIdx + bitIdx] = ONEONEPATTERN;
                    break;
                }
            }
        }
    }
}

bool ICLED_set_all_pixels(uint16_t R, uint16_t G, uint16_t B, bool write_buffer)
{
    for (int i = 0; i < ICLED_NUM; i++)
    {
        if (!ICLED_set_pixel(i, R, G, B, false))
        {
            return false;
        }
    }

    if (write_buffer)
    {
        write_ledbuffer_to_DMAbuffer();
    }

    return true;
}

void ICLED_clear(bool write_buffer)
{
    memset(LEDBuf, 0, sizeof(LEDBuf));

    if (write_buffer)
    {
        memset(dmaBuf, 0, sizeof(dmaBuf));
    }
}
