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

#include "config.h"

#if PIN_TOOGLING != true

    #ifndef ICLED_24bit_SPI_H
    #define ICLED_24bit_SPI_H


    #include <stdint.h>
    #include <stddef.h>
    

    #define ICLED_DIN_PIN 29        // COPI (MOSI) pin on the feather M0

    #define ICLED_CIN_PIN 30        // SCK pin on the feather M0

    typedef enum
    {
        RGB,
        HSV,
    } ICLED_Color_System;

    /**
     * @brief       Intialize the interfaces for ICLED array.
     *
     * @param[in]   color_system: Specify the color system to be used. See ICLED_Color_System.
     *
     * @return      True if successful, false otherwise.
     */
    bool ICLED_Init(ICLED_Color_System color_system =  RGB);

    /**
     * @brief       Deintializes the interfaces for ICLED array.
     *
     * @return      True if successful, false otherwise.
     */
    bool ICLED_Deinit();

    /**
     * @brief       Set an ICLED in the array using its index to the given color with the given global current gain.
     *
     * @param[in]   pixel_number: Index of the ICLED.
     * @param[in]   R_H: R/H-coordinate of color.
     * @param[in]   G_S: G/S-coordinate of color.
     * @param[in]   B_V: B/V-coordinate of color.
     * @param[in]   g_current_gain: Global current gain
     * @param[in]   write_buffer: Optional argument that indicates whether the LED buffer should be applied to the LED screen. Defaults to true.
     *
     * @return      True if successful, false otherwise.
     */
    bool ICLED_set_pixel(uint16_t pixel_number, uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t g_current_gain, bool write_buffer = true);

    /**
     * @brief       Set all ICLEDs in the array to the given color with the given global current gain.
     *
     * @param[in]   R_H: R/H-coordinate of color.
     * @param[in]   G_S: G/S-coordinate of color.
     * @param[in]   B_V: B/V-coordinate of color.
     * @param[in]   g_current_gain: Global current gain
     * @param[in]   write_buffer: Optional argument that indicates whether the LED buffer should be applied to the LED screen. Defaults to true.
     *
     * @return      True if successful, false otherwise.
     */
    bool ICLED_set_all_pixels(uint16_t R_H, uint16_t G_S, uint16_t B_V, uint8_t g_current_gain, bool write_buffer = true);

    /**
     * @brief       Enable the sleep modus of the  ICLED in the array using its index.
     *
     * @param[in]   pixel_number: Index of the ICLED.
     * @param[in]   write_buffer: Optional argument that indicates whether the LED buffer should be applied to the LED screen. Defaults to true.
     *
     * @return      True if successful, false otherwise.
     */
    bool ICLED_sleep_pixel(uint16_t pixel_number, bool write_buffer = true);

    /**
     * @brief       Enable the sleep modus of all ICLEDs in the array.
     *
     * @param[in]   write_buffer: Optional argument that indicates whether the LED buffer should be applied to the LED screen. Defaults to true.
     *
     * @return      True if successful, false otherwise.
     */
    bool ICLED_sleep_all_pixels(bool write_buffer = true);

    /**
     * @brief       Set the color system to be used, this will be applied
     *              for the following calls to set_pixel and similar functions.
     *
     * @param[in]   color_system: Specify the color system to be used. See ICLED_Color_System.
     *
     * @return      None
     */
    void ICLED_set_color_system(ICLED_Color_System color_system);

    /**
     * @brief       Get the color system that is currently set.
     *
     * @return      The color system that is currently set. See ICLED_Color_System.
     */
    ICLED_Color_System ICLED_get_color_system();

    /**
     * @brief       Clear the ICLED buffer.
     *
     * @param[in]   write_buffer: Optional argument that indicates whether the LED buffer should be applied to the LED screen. Defaults to true.
     *
     * @return      True if successful, false otherwise.
     */
    void ICLED_clear(bool write_buffer = true);

    #endif
    
#endif