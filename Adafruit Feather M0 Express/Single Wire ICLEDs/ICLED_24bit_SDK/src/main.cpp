/**
 * \file
 * \brief Main file for the WE Single Wire 24-bit ICLEDs.
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

/**         Includes         */
#include "global.h"
#include "debug.h"
#include "ICLED_24bit.h"
#include "ICLED_24bit_demos.h"

/* Test Modes */
typedef enum
{
    TEST1,
    TEST2,
    TEST3,
    TEST4,
    TEST5,
    TEST6,
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

void loop() {

  ICLED_set_color_system(RGB);

  switch (current_mode)
    {
    case TEST1:
      ICLED_demo_Blink(0, 64, 500); 
    break;

    case TEST2: 
      ICLED_demo_Breathing(50, 25);
    break; 

    case TEST3:
      ICLED_demo_ColorWhipe(255, 0 , 0, 20, 80);
    break;

    case TEST4: 
      ICLED_demo_Cyclon(255, 0, 255, 20, 25);
    break; 

    case TEST5: 
      ICLED_demo_Rainbow(20, 10);
    break;

    case TEST6: 
      ICLED_demo_TheaterChase(255, 255, 255, 10, 100);
    break;

    default: 
    break;  
  }
}