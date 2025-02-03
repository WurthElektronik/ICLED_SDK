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

#ifndef ICLED_24bit_DEMOS_H
#define ICLED_24bit_DEMOS_H

#include <stdint.h>
#include <stddef.h>


// 

/** 
 * @brief       Creates a blinking effect on the selected ICLED number acording to predefined color and brightness level set by the user.
 *
 * @param[in]   pixel_number: Index of the ICLED.
 * @param[in]   brightness: Brightness.
 * @param[in]   delay_ms: Delay time for the animation (in miliseconds)
 *
 * @return      True if successful, false otherwise.
 */
bool ICLED_demo_Blink(uint16_t pixel_number, uint8_t brightness, uint16_t delay_ms); 

/** 
 * @brief       Creates a smooth "breathing" light effect on the ICLED strip by gradually increasing and decreasing brightness
 *
 * @param[in]   brightness: Brightness.
 * @param[in]   delay_ms: Delay time for the animation (in miliseconds)
 *
 * @return      True if successful, false otherwise.
 */
bool ICLED_demo_Breathing(uint16_t brightness, uint16_t delay_ms);

/** 
 * @brief       Sequentially lights up the ICLEDs in the given color and brightness from start to end and then clears the ICLED strip
 *
 * @param[in]   pixel_number: Index of the ICLED.
 * @param[in]   R_H: R/H-coordinate of color.
 * @param[in]   G_S: G/S-coordinate of color.   
 * @param[in]   B_V: B/V-coordinate of color.
 * @param[in]   brightness: Brightness.
 * @param[in]   delay_ms: Delay time for the animation (in miliseconds)
 *
 * @return      True if successful, false otherwise.
 */
bool ICLED_demo_ColorWhipe(uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t brightness, uint16_t delay_ms);

/** 
 * @brief       Sequentially lights up the ICLEDs one at a time from left to right and then back from right to left in the given color and brightness
 *
 * @param[in]   pixel_number: Index of the ICLED.
 * @param[in]   R_H: R/H-coordinate of color.
 * @param[in]   G_S: G/S-coordinate of color.   
 * @param[in]   B_V: B/V-coordinate of color.
 * @param[in]   brightness: Brightness.
 * @param[in]   delay_ms: Delay time for the animation (in miliseconds)
 *
 * @return      True if successful, false otherwise.
 */
bool ICLED_demo_Cyclon(uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t brightness, uint16_t delay_ms);

/** 
 * @brief       Creates a dynamic "rainbow effect" on an ICLED strip by cycling through colors smoothly across all pixels
 *
 * @param[in]   brightness: Brightness.
 * @param[in]   delay_ms: Delay time for the animation (in miliseconds)
 *
 * @return      True if successful, false otherwise.
 */
bool ICLED_demo_Rainbow(uint8_t brightness, uint16_t delay_ms);

/** 
 * @brief       Sequentially lights up every third ICLED in a repeating pattern, creating a "theater chase" effect
 *
 * @param[in]   R_H: R/H-coordinate of color.
 * @param[in]   G_S: G/S-coordinate of color.   
 * @param[in]   B_V: B/V-coordinate of color.
 * @param[in]   brightness: Brightness.
 * @param[in]   delay_ms: Delay time for the animation (in miliseconds)
 *
 * @return      True if successful, false otherwise.
 */
bool ICLED_demo_TheaterChase(uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t brightness, uint16_t delay_ms);

#endif