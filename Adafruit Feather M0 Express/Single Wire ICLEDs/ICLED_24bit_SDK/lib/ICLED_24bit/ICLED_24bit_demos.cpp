/**
***************************************************************************************************
* This file is part of ICLED SDK:
*
*
* THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE
*THAT WÜRTH ELEKTRONIK EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY KIND
*RELATED TO, BUT NOT LIMITED TO THE NON-INFRINGEMENT OF THIRD PARTIES’
*INTELLECTUAL PROPERTY RIGHTS OR THE MERCHANTABILITY OR FITNESS FOR YOUR
*INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT WARRANT OR REPRESENT
*THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY PATENT RIGHT,
*COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY RIGHT RELATING TO
*ANY COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT IS USED. INFORMATION
*PUBLISHED BY WÜRTH ELEKTRONIK EISOS REGARDING THIRD-PARTY PRODUCTS OR SERVICES
*DOES NOT CONSTITUTE A LICENSE FROM WÜRTH ELEKTRONIK EISOS TO USE SUCH PRODUCTS
*OR SERVICES OR A WARRANTY OR ENDORSEMENT THEREOF
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS DRIVER PACKAGE.
*
* COPYRIGHT (c) 2025 Würth Elektronik eiSos GmbH & Co. KG
*
***************************************************************************************************
**/
#include "global.h"
#include "ICLED_24bit.h"
#include "ICLED_24bit_demos.h"

bool ICLED_demo_Blink(uint16_t pixel_number, uint8_t brightness, uint16_t delay_ms)
{
        ICLED_set_pixel(pixel_number, 255, 0, 0, brightness);
        WE_Delay(delay_ms);

        ICLED_set_pixel(pixel_number, 0, 255, 0, brightness);
        WE_Delay(delay_ms);
        
        ICLED_set_pixel(pixel_number, 0, 0, 255, brightness);
        WE_Delay(delay_ms);
        
        ICLED_set_pixel(pixel_number, 255, 0, 255, brightness);
        WE_Delay(delay_ms);
        
        ICLED_set_pixel(pixel_number, 0, 255, 255, brightness);
        WE_Delay(delay_ms);
        
        ICLED_set_pixel(pixel_number, 255, 255, 0, brightness);
        WE_Delay(delay_ms);
        
        ICLED_set_pixel(pixel_number, 255, 255, 255, brightness);
        WE_Delay(delay_ms);
        
        ICLED_set_pixel(pixel_number, 0, 0, 0, brightness);
        WE_Delay(delay_ms);
    return true;
}

bool ICLED_demo_Breathing(uint16_t brightness, uint16_t delay_ms)
{
    for (int i = 0; i <= brightness; i++)
    {
        ICLED_set_all_pixels(255, 255, 255, i);
        WE_Delay(delay_ms);
    } 

        WE_Delay(delay_ms);

    for (int i = brightness; i >= 0; i--)
    {
        ICLED_set_all_pixels(255, 255, 255, i);
        WE_Delay(delay_ms);
    } 

        WE_Delay(delay_ms);
    
    return true;
}

bool ICLED_demo_ColorWhipe(uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t brightness, uint16_t delay_ms)
{
    for (int i = 0; i < ICLED_NUM; i++)
    {
        ICLED_set_pixel(i, R_H, G_S, B_V, brightness);
        WE_Delay(delay_ms);
    }
    ICLED_clear();
    WE_Delay(delay_ms*2);

    return true;
}

bool ICLED_demo_Cyclon(uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t brightness, uint16_t delay_ms)
{
    // Scan from left to right
    for (int i = 0; i < ICLED_NUM; i++)
    {
        ICLED_set_pixel(i, R_H, G_S, B_V, brightness);
        WE_Delay(delay_ms);
    }

    ICLED_clear();
    WE_Delay(delay_ms*2);

    // Scan from right to left
    for (int i = ICLED_NUM-1; i >= 0; i--)
    {
        ICLED_set_pixel(i, R_H, G_S, B_V, brightness);
        WE_Delay(delay_ms);
    }

    ICLED_clear();
    WE_Delay(delay_ms*2);

    return true;
}

bool ICLED_demo_Rainbow(uint8_t brightness, uint16_t delay_ms) 
{
    for (int i = 0; i < 256; i++) 
    {
        for (int j = 0; j < ICLED_NUM; j++) 
        {
            // Calculate the position in the rainbow color wheel
            int pos = (j + i) % 256;

            // Calculate RGB values based on the position
            uint8_t r = (pos < 85) ? (pos * 3) : ((pos < 170) ? (255 - (pos - 85) * 3) : 0);
            uint8_t g = (pos < 85) ? 0 : ((pos < 170) ? ((pos - 85) * 3) : (255 - (pos - 170) * 3));
            uint8_t b = (pos < 85) ? (255 - pos * 3) : ((pos < 170) ? 0 : ((pos - 170) * 3));

            // Set the color of the j-th ICLED
            ICLED_set_pixel(j, r, g, b, brightness);
        }
        // Wait for the specified delay before the next iteration
        WE_Delay(delay_ms);
    }
    return true;
}

bool ICLED_demo_TheaterChase(uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t brightness, uint16_t delay_ms) 
{
    for (int j = 0; j < 10; j++) 
    {  // Do 10 cycles of chasing
        for (int q = 0; q < 3; q++) 
        {
            for (int i = 0; i < ICLED_NUM; i = i + 3) 
            {
                ICLED_set_pixel(i + q, R_H, G_S, B_V, brightness); // Turn every third ICLED on
            }
            
            WE_Delay(delay_ms);
            
            ICLED_clear();
        }
    }
    return true;
}