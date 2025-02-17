/*
 ***************************************************************************************************
 * This file is part of WIRELESS CONNECTIVITY SDK for STM32:
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
 * COPYRIGHT (c) 2023 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 */

/**
 * @file
 * @brief Contains global function definitions for the Wireless Connectivity SDK for STM32.
 */

#include "global.h"
#include "global_types.h"

#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

    bool WE_InitPins(WE_Pin_t pins[], uint8_t numPins)
    {
        /* Sort pins by port */
        for (uint8_t i = 0; i < numPins; i++)
        {
            if (0 == pins[i].pin)
            {
                /* Unused */
                continue;
            }

            if (WE_Pin_Type_Output == pins[i].type)
            {
                /* Configure GPIO pin output level */
                SetPinMode(pins[i].pin, OUTPUT);
                WritePin(pins[i].pin, 0);
            }
            else if (WE_Pin_Type_Input == pins[i].type)
            {
                SetPinMode(pins[i].pin, INPUT);
            }
        }

        return true;
    }

    bool WE_DeinitPin(WE_Pin_t pin)
    {
        return true;
    }

    bool WE_SetPin(WE_Pin_t pin, WE_Pin_Level_t out)
    {
        if (0 == pin.pin || pin.type != WE_Pin_Type_Output)
        {
            return false;
        }

        switch (out)
        {
        case WE_Pin_Level_High:
            WritePin(pin.pin, 1);
            break;

        case WE_Pin_Level_Low:
            WritePin(pin.pin, 0);
            break;

        default:
            return false;
        }

        return true;
    }

    WE_Pin_Level_t WE_GetPinLevel(WE_Pin_t pin)
    {
        uint8_t pinLevel;
        readPin(pin.pin, &pinLevel);
        switch (pinLevel)
        {
        case 0:
            return WE_Pin_Level_Low;

        case 1:
            return WE_Pin_Level_High;

        default:
            return WE_Pin_Level_Low;
        }
    }

    void WE_Delay(uint16_t sleepForMs)
    {
        if (sleepForMs > 0)
        {
            delay((uint32_t)sleepForMs);
        }
    }

    uint32_t WE_GetTick()
    {
        return millis();
    }

#ifndef WE_MICROSECOND_TICK
    void WE_DelayMicroseconds(uint32_t sleepForUsec)
    {
        /* Microsecond tick is disabled: round to ms */
        WE_Delay(((sleepForUsec + 500) / 1000));
    }

    uint32_t WE_GetTickMicroseconds()
    {
        /* Microsecond tick is disabled: return ms tick * 1000 */
        return WE_GetTick() * 1000;
    }
#endif /* WE_MICROSECOND_TICK */

#ifdef __cplusplus
}
#endif
