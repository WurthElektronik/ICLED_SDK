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
#include "ICLED_48bit.h"
#include "ICLED_48bit_demos.h"

bool ICLED_demo_Blink(uint16_t pixel_number, uint16_t delay_ms)
{
    uint16_t LED_current = 0x4;         // 25% of maximum current (4-bit value)
    uint16_t LED_brightness = 0xFFF;    // Maximum PWM brightness (12-bit value)
    uint16_t LED_off = 0x000;           // LED off state

    ICLED_set_pixel(pixel_number, (LED_current << 12) | LED_brightness, LED_off, LED_off);
    WE_Delay(delay_ms);

    ICLED_set_pixel(pixel_number, LED_off, (LED_current << 12) | LED_brightness, LED_off);
    WE_Delay(delay_ms);

    ICLED_set_pixel(pixel_number, LED_off, LED_off, (LED_current << 12) | LED_brightness);
    WE_Delay(delay_ms);

    ICLED_set_pixel(pixel_number, (LED_current << 12) | LED_brightness, LED_off, (LED_current << 12) | LED_brightness);
    WE_Delay(delay_ms);

    ICLED_set_pixel(pixel_number, LED_off, (LED_current << 12) | LED_brightness, (LED_current << 12) | LED_brightness);
    WE_Delay(delay_ms);

    ICLED_set_pixel(pixel_number, (LED_current << 12) | LED_brightness, (LED_current << 12) | LED_brightness, LED_off);
    WE_Delay(delay_ms);

    ICLED_set_pixel(pixel_number, (LED_current << 12) | LED_brightness, (LED_current << 12) | LED_brightness, (LED_current << 12) | LED_brightness);
    WE_Delay(delay_ms);

    ICLED_set_pixel(pixel_number, LED_off, LED_off, LED_off);
    WE_Delay(delay_ms);

    return true;
}

bool ICLED_demo_Breathing( uint16_t delay_ms)
{
    uint16_t LED_current = 0x4; // 25% of maximum current (4-bit value)
    uint16_t LED_brightness = 0xFFF; // Maximum PWM brightness (12-bit value)
    
    for (uint16_t brightness = 0; brightness <= LED_brightness; brightness = brightness + 10)
    {
        uint16_t red   = (LED_current << 12) | brightness;
        uint16_t green = (LED_current << 12) | brightness;
        uint16_t blue  = (LED_current << 12) | brightness;
        
        ICLED_set_all_pixels(red, green, blue);
        WE_Delay(delay_ms);
    }
    
    WE_Delay(delay_ms);
    
    for (int16_t brightness = LED_brightness; brightness >= 0; brightness = brightness - 10)
    {
        uint16_t red   = (LED_current << 12) | brightness;
        uint16_t green = (LED_current << 12) | brightness;
        uint16_t blue  = (LED_current << 12) | brightness;
        
        ICLED_set_all_pixels(red, green, blue);
        WE_Delay(delay_ms);
    }
    
    WE_Delay(delay_ms);
    
    return true;
}

bool ICLED_demo_ColorWhipe(uint16_t R, uint16_t G, uint16_t B, uint16_t delay_ms)
{
    for (int i = 0; i < ICLED_NUM; i++)
    {
        ICLED_set_pixel(i, R, G, B);
        WE_Delay(delay_ms);
    }
    
    ICLED_set_all_pixels(0, 0, 0);

    WE_Delay(delay_ms);

    return true;
}

bool ICLED_demo_Cyclon(uint16_t R, uint16_t G, uint16_t B, uint16_t delay_ms)
{
    // Scan from left to right
    for (int i = 0; i < ICLED_NUM; i++)
    {
        ICLED_set_pixel(i, R, G, B);
        WE_Delay(delay_ms);
    }
    
    // Clear the strip
    ICLED_set_all_pixels(0, 0, 0);
    WE_Delay(delay_ms);
    
    // Scan from right to left
    for (int i = ICLED_NUM - 1; i >= 0; i--)
    {
        ICLED_set_pixel(i, R, G, B);
        WE_Delay(delay_ms);
    }
    
    // Clear the strip again
    ICLED_set_all_pixels(0, 0, 0);
    WE_Delay(delay_ms);
    
    return true;
}

bool ICLED_demo_Rainbow(uint16_t delay_ms) 
{
    uint16_t LED_current = 0x1; // 25% of maximum current (4-bit value)

    for (int i = 0; i < 4096; i++) 
    {
        for (int j = 0; j < ICLED_NUM; j++) 
        {
            // Calculate the position in the rainbow color wheel
            int pos = (j + i) % 4096;

            // Calculate RGB values based on the position
            uint16_t r = (pos < 1365) ? (pos * 3) : ((pos < 2730) ? (4095 - (pos - 1365) * 3) : 0);
            uint16_t g = (pos < 1365) ? 0 : ((pos < 2730) ? ((pos - 1365) * 3) : (4095 - (pos - 2730) * 3));
            uint16_t b = (pos < 1365) ? (4095 - pos * 3) : ((pos < 2730) ? 0 : ((pos - 2730) * 3));

             // Combine with the fixed 4-bit current in the upper 4 bits
             uint16_t R   = (LED_current << 12) | (r  & 0xFFF);
             uint16_t G = (LED_current << 12) | (g  & 0xFFF);
             uint16_t B  = (LED_current << 12) | (b  & 0xFFF);
 
             // Set the color for the j-th pixel
             ICLED_set_pixel(j, R, G, B);
         }
         WE_Delay(delay_ms);
     }
     return true;
 }

 bool ICLED_demo_TheaterChase(uint16_t R, uint16_t G, uint16_t B, uint16_t delay_ms) 
 {     
     for (int j = 0; j < 10; j++) 
     {  
         // Do 10 cycles of chasing
         for (int q = 0; q < 3; q++) 
         {
             for (int i = 0; i < ICLED_NUM; i += 3) 
             {
                 ICLED_set_pixel(i + q, R, G, B);
             }
             
             WE_Delay(delay_ms);
             
             ICLED_clear();
         }
     }
     return true;
 }