/**
 * \file
 * \brief Main file for the WE Dual Wire 24-bit ICLEDs (8-bit per channel).
 *
 * \copyright (c) 2025 Würth Elektronik eiSos GmbH & Co. KG
 *
 * \page License
 *
 * THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE
 * THAT WÜRTH ELEKTRONIK EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY
 * KIND RELATED TO, BUT NOT LIMITED TO THE NON-INFRINGEMENT OF THIRD PARTIES’
 * INTELLECTUAL PROPERTY RIGHTS OR THE MERCHANTABILITY OR FITNESS FOR YOUR
 * INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT WARRANT OR
 * REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY
 * PATENT RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY
 * RIGHT RELATING TO ANY COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT
 * IS USED. INFORMATION PUBLISHED BY WÜRTH ELEKTRONIK EISOS REGARDING
 * THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE FROM WÜRTH
 * ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR
 * ENDORSEMENT THEREOF
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS PACKAGE
 */


/** Includes */
#include "config.h"
#include "global.h"
#include "debug.h"

#if PIN_TOOGLING == true
#include "ICLED_24bit.h"
#else
#include "ICLED_24bit_SPI.h"
#endif


#include "ICLED_24bit_demos.h"


/* Test Modes */
typedef enum {
  TEST1,
  TEST2,
  TEST3,
  TEST4,
  TEST5,
  TEST6, 
  TEST7
} TestMode;

static volatile TestMode current_mode = TEST5;

void setup() 
{
  // Using the USB serial port for debug messages
  #ifdef WE_DEBUG
    WE_Debug_Init();
  #endif
  
  if (!ICLED_Init(RGB))
  {
    WE_DEBUG_PRINT("ICLED init failed \r\n");
  }
  ICLED_clear();
}

void loop() 
{
  ICLED_set_color_system(RGB);

  switch (current_mode)
  {
    case TEST1:
      ICLED_demo_Blink(0, 0x8, 500);                        // Send the data to the first LED in the chain with 50% of maximum current for the selected colors (4-bit value) & 500 ms delay time between data packages 
      break;

    case TEST2: 
      ICLED_demo_Breathing(0x8, 100);                       // 50% of maximum current for the selected colors (4-bit value) & 100 ms delay time between data packages 
      break; 

    case TEST3:
      ICLED_demo_ColorWhipe(0x00, 0x00, 0xFF, 0x8, 100);    // 50% of maximum current for the selected colors (4-bit value) & 100 ms delay time between data packages
      break;

    case TEST4: 
      ICLED_demo_Cyclon(0xFF, 0x00, 0xFF, 0x8, 100);        // 50% of maximum current for the selected colors (4-bit value) & 100 ms delay time between data packages
      break; 

    case TEST5: 
      ICLED_demo_Rainbow(0x8, 10);                          // 50% of maximum current for the selected colors (4-bit value) & 10 ms delay time between data packages 
      break;

    case TEST6: 
      ICLED_demo_TheaterChase(0xFF, 0xFF, 0xFF, 0x8, 100);  // 50% of maximum current for the selected colors (4-bit value) & 100 ms delay time between data packages
      break;

    case TEST7: 
      ICLED_demo_FadingSleep(0x8, 500);                     // 50% of maximum current for the selected colors (4-bit value) & 500 ms delay time between data packages
      break; 

    default: 
      break;  
  }
}
