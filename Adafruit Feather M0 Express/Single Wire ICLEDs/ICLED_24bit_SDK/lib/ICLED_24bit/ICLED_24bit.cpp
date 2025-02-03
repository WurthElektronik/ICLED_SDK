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
#include "ICLED_24bit.h"
#include "ConfigPlatform.h"
#include "debug.h"
#include "global.h"

/**
 * @brief       Apply the specified brightness to the color coordinate.
 *
 * @param[in]   color: Color coordinate value.
 * @param[in]   brightness:  Brightness value.
 *
 * @return      The color coordinate value with brightness applied.
 */
static inline uint8_t calculate_brightness(uint8_t color, uint8_t brightness);

/**
 * @brief       Applies the current LED buffer from the specified start column to the
 *              ICLED board by copying the LED buffer to the DMA buffer.
 *
 * @param[in]   column: The starting column from which the LED buffer will be copied.
 *
 * @return      None
 */
static void write_ledbuffer_to_DMAbuffer();

/**
 * @brief       Convert HSV values to RGB.
 *
 * @param[in]   h: Hue.
 * @param[in]   s: Saturation.
 * @param[in]   v: Value.
 * @param[out]  r: Red.
 * @param[out]  g: Green.
 * @param[out]  b: Blue.
 *
 * @return      None
 */
static void HSV_to_RGB(float h, float s, float v, float *r, float *g, float *b);

static ICLED_Color_System ColorSystem = RGB;

static uint8_t dmaBuf[ICLED_BYTESTOTAL] = {0}; // The raw buffer we write to SPI
static Adafruit_ZeroDMA dma; ///< The DMA manager for the SPI class
static SPIClass *spi;        ///< Underlying SPI hardware interface we use to DMA

#define MIN_LOOP_DELAY_MS 5
#define OFFSET 1

typedef union
{
    struct
    {
        uint8_t G;
        uint8_t R;
        uint8_t B;
    };
    uint8_t GBR[3];
} Pixel;

// buffer for LEDs --> will be written into dmaBuf after bit-expansion in
static Pixel LEDBuf[ICLED_BYTESTOTAL];

#define ZEROPATTERN 0x8 // 4-bit
#define ONEPATTERN 0xE  // 4-bit

#define ZEROZEROPATTERN ((ZEROPATTERN << 4) | ZEROPATTERN) // 10001000
#define ZEROONEPATTERN ((ZEROPATTERN << 4) | ONEPATTERN)   // 10001110
#define ONEZEROPATTERN ((ONEPATTERN << 4) | ZEROPATTERN)   // 11101000
#define ONEONEPATTERN ((ONEPATTERN << 4) | ONEPATTERN)     // 11101110

bool ICLED_Init(ICLED_Color_System color_system)
{
    // set color System to given Color system
    ColorSystem = color_system;

    // clear Buffer and set all values to zero
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
    // clear Buffer and set all values to zero
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

static void HSV_to_RGB(float h, float s, float v, float *r, float *g, float *b)
{
    int i = floor(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch (i % 6)
    {
    case 0:
        *r = v, *g = t, *b = p;
        break;
    case 1:
        *r = q, *g = v, *b = p;
        break;
    case 2:
        *r = p, *g = v, *b = t;
        break;
    case 3:
        *r = p, *g = q, *b = v;
        break;
    case 4:
        *r = t, *g = p, *b = v;
        break;
    case 5:
        *r = v, *g = p, *b = q;
        break;
    }
}

bool ICLED_set_pixel(uint16_t pixel_number, uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t brightness, bool write_buffer)
{
    // Check, if parameters are ok & write LEDBuf
    if (pixel_number >= ICLED_NUM)
    {
        WE_DEBUG_PRINT("Pixel index %d is out of the given range.\r\n", pixel_number);
        return false;
    }

    uint8_t R = 0, G = 0, B = 0;
    switch (ColorSystem)
    {
    case RGB:
    {
        if (R_H > 255 || G_S > 255 || B_V > 255)
        {
            WE_DEBUG_PRINT("RGB values should be between (0-255).\r\n");
            return false;
        }
        break;
    }
    case HSV:
    {
        if (R_H > 360 || G_S > 100 || B_V > 100)
        {
            WE_DEBUG_PRINT("H should be between (0-360), S and V between (0-100).\r\n");
            return false;
        }

        // Convert HSV to RGB
        float r = 0, g = 0, b = 0;
        HSV_to_RGB((float)R_H / 360, (float)G_S / 100, (float)B_V / 100, &r, &g, &b);
        R_H = (uint8_t)(r * 255);
        G_S = (uint8_t)(g * 255);
        B_V = (uint8_t)(b * 255);
        break;
    }
    default:
        WE_DEBUG_PRINT("Invalid color system.\r\n");
        return false;
    }

    R = calculate_brightness((uint8_t)R_H, brightness);
    G = calculate_brightness((uint8_t)G_S, brightness);
    B = calculate_brightness((uint8_t)B_V, brightness);

    LEDBuf[pixel_number].G = G;
    LEDBuf[pixel_number].R = R;
    LEDBuf[pixel_number].B = B;

    if (write_buffer)
    {
        write_ledbuffer_to_DMAbuffer();
    }

    return true;
}

static inline uint8_t calculate_brightness(uint8_t color, uint8_t brightness)
{
    return (uint8_t)(((uint16_t)color * (uint16_t)brightness) / ((uint16_t)255));
}

static void write_ledbuffer_to_DMAbuffer()
{
    for (int i = 0; i < ICLED_NUM; i++)
    {
        for (uint8_t colorIdx = 0; colorIdx < 3; colorIdx++)
        {
            for (uint8_t bitIdx = 0; bitIdx < 4; bitIdx++)
            {
                switch ((LEDBuf[i].GBR[colorIdx] << (2 * bitIdx)) & 0xC0)
                { // mask upper two bit
                case 0x00:
                    dmaBuf[4 * 3 * i + 4 * colorIdx + bitIdx] = ZEROZEROPATTERN;
                    break;
                case 0x40:
                    dmaBuf[4 * 3 * i + 4 * colorIdx + bitIdx] = ZEROONEPATTERN;
                    break;
                case 0x80:
                    dmaBuf[4 * 3 * i + 4 * colorIdx + bitIdx] = ONEZEROPATTERN;
                    break;
                case 0xC0:
                    dmaBuf[4 * 3 * i + 4 * colorIdx + bitIdx] = ONEONEPATTERN;
                    break;
                }
            }
        }
    }
}

bool ICLED_set_all_pixels(uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t brightness, bool write_buffer)
{
    for (int i = 0; i < ICLED_NUM; i++)
    {
        if (!ICLED_set_pixel(i, R_H, G_S, B_V, brightness, false))
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