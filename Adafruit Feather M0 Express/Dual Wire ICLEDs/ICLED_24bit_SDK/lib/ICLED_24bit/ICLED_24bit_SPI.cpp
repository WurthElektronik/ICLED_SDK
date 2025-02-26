/**
***************************************************************************************************
* This file is part of ICLED SDK:
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

#include "config.h"

#if PIN_TOOGLING != true

    #include "global.h"
    #include "debug.h"
    #include "ICLED_24bit_SPI.h"
    #include "ConfigPlatform.h"
   
    #include <SPI.h>

    static ICLED_Color_System ColorSystem = RGB;
    static uint8_t SPIBuf[ICLED_BYTESTOTAL] = {true};

    static void write_ledbuffer_to_SPIBuf();

    typedef union
    {
        struct
        {
            uint8_t F_C;
            uint8_t B;
            uint8_t G;
            uint8_t R;
        };
        uint8_t BGR[4];
    } Pixel;

    static Pixel LEDBuf[ICLED_NUM];

    bool ICLED_Init(ICLED_Color_System color_system)
    {
        ColorSystem = color_system;
        ICLED_clear();
        SPI.begin();
        return true;
    }

    bool ICLED_Deinit()
    {
        ICLED_clear();
        SPI.end();
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

    bool ICLED_set_pixel(uint16_t pixel_number, uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t g_current_gain, bool write_buffer)
    {
        if (pixel_number >= ICLED_NUM)
        {
            WE_DEBUG_PRINT("Pixel index %d is out of the given range.\r\n", pixel_number);
            return false;
        }

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

        if (ICLED_PN == 1315050930246)      //LED Frame data with order: Blue-Green-Red
        {
            LEDBuf[pixel_number].F_C = (0xE0 | (g_current_gain & 0x1F));
            LEDBuf[pixel_number].B = B_V;
            LEDBuf[pixel_number].G = G_S;
            LEDBuf[pixel_number].R = R_H;
        
        } else                              //LED Frame data with order: Green-Blue-Red
        {
            LEDBuf[pixel_number].F_C = (0xE0 | (g_current_gain & 0x1F));
            LEDBuf[pixel_number].B = G_S;
            LEDBuf[pixel_number].G = B_V;
            LEDBuf[pixel_number].R = R_H;
        }

        if (write_buffer)
        {
            write_ledbuffer_to_SPIBuf();
        }
        return true;
    }

    static void write_ledbuffer_to_SPIBuf()
    {
        for(int i= 0; i < START_FRAME ; i++)
        {
            SPIBuf[i] = 0;
        }

        for (int i = 0; i < ICLED_NUM; i++)
        {
            for (uint8_t colorIdx = 0; colorIdx < 4; colorIdx++)
            {
                SPIBuf[4 + 4 * i + colorIdx] = LEDBuf[i].BGR[colorIdx];
            }
        }

        SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
        for (size_t i = 0; i < sizeof(SPIBuf); i++)
        {
            SPI.transfer(SPIBuf[i]);
        }
        SPI.endTransaction();
    }

    bool ICLED_set_all_pixels(uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t g_current_gain, bool write_buffer)
    {
        for (int i = 0; i < ICLED_NUM; i++)
        {
            if (!ICLED_set_pixel(i, R_H, G_S, B_V, g_current_gain, false))
            {
                return false;
            }
        }

        if (write_buffer)
        {
            write_ledbuffer_to_SPIBuf();
        }
        return true;
    }

    bool ICLED_sleep_pixel(uint16_t pixel_number, bool write_buffer)
    {
        if (pixel_number >= ICLED_NUM)
        {
            WE_DEBUG_PRINT("Pixel index %d is out of the given range.\r\n", pixel_number);
            return false;
        }

        LEDBuf[pixel_number].F_C = 0xA0;
        LEDBuf[pixel_number].B = 0;
        LEDBuf[pixel_number].G = 0;
        LEDBuf[pixel_number].R = 0;

        if (write_buffer)
        {
            write_ledbuffer_to_SPIBuf();
        }
        return true;
    }

    bool ICLED_sleep_all_pixels( bool write_buffer)
    {
        for (int i = 0; i < ICLED_NUM; i++)
        {
            if (!ICLED_sleep_pixel(i, false))
            {
                return false;
            }
        }

        if (write_buffer)
        {
            write_ledbuffer_to_SPIBuf();
        }
        return true;
    }

    void ICLED_clear(bool write_buffer)
    {
        memset(LEDBuf, 0, sizeof(LEDBuf));
        if (write_buffer)
        {
            ICLED_set_all_pixels(0, 0, 0, 0);
        }
    }
#endif