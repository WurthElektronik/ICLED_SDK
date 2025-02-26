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

#ifndef CONFIG_H
#define CONFIG_H

#define PIN_TOOGLING  true                  // When false, SPI is used for communication, when true, pin toggling (low speed communication) is used

#define ICLED_PN  1315050930246             // ICLED part number used in the demo

#define ICLED_NUM 7                         // Define the size of the ICLED array that is being used

#define ICLED_BYTESPERPIXEL 4               // BGR, each is 8bit = 1 Byte + 1 byte to setup the LED (Flag + Global current gain)

#define START_FRAME 4                       // Command to indicate the start of a data string [32x0] bits  = 4 Bytes

#define END_FRAME ((ICLED_NUM + 7) / 8)     // Define END_FRAME as the number of LEDs divided by 2(divided by 4 bytes/LED), rounding up to the next integer if needed

#define ICLED_BYTESTOTAL (START_FRAME + ICLED_NUM * ICLED_BYTESPERPIXEL + END_FRAME) // Max transfer size: Start Frame + LED Data + End frame

 /**
 * @brief   Create a variable to limit the maximum PWM value to be used. Reccomended to be used in temperature sensitive applications.
 * 
 */
 #define ICLED_MAX_BRIGHTNESS 255           // Max Brightness of ICLED (8-bit value)

 /**
 * @brief   Create a variable to limit the maximum global current gain to be used. Reccomended to be used in temperature sensitive applications.
 * 
 */
 #define ICLED_MAX_CURRENTGAIN 15           // Max Current Gain of ICLED

#endif // CONFIG_H