/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for PIN MANAGER.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.4
        Device            :  dsPIC33EP512MU810
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H
/**
    Section: Includes
*/
#include <xc.h>

/**
    Section: Device Pin Macros
*/
/**
  @Summary
    Sets the GPIO pin, RE2, high using LATE2.

  @Description
    Sets the GPIO pin, RE2, high using LATE2.

  @Preconditions
    The RE2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RE2 high (1)
    LED3_SetHigh();
    </code>

*/
#define LED3_SetHigh()          (_LATE2 = 1)
/**
  @Summary
    Sets the GPIO pin, RE2, low using LATE2.

  @Description
    Sets the GPIO pin, RE2, low using LATE2.

  @Preconditions
    The RE2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RE2 low (0)
    LED3_SetLow();
    </code>

*/
#define LED3_SetLow()           (_LATE2 = 0)
/**
  @Summary
    Toggles the GPIO pin, RE2, using LATE2.

  @Description
    Toggles the GPIO pin, RE2, using LATE2.

  @Preconditions
    The RE2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RE2
    LED3_Toggle();
    </code>

*/
#define LED3_Toggle()           (_LATE2 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RE2.

  @Description
    Reads the value of the GPIO pin, RE2.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RE2
    postValue = LED3_GetValue();
    </code>

*/
#define LED3_GetValue()         _RE2
/**
  @Summary
    Configures the GPIO pin, RE2, as an input.

  @Description
    Configures the GPIO pin, RE2, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RE2 as an input
    LED3_SetDigitalInput();
    </code>

*/
#define LED3_SetDigitalInput()  (_TRISE2 = 1)
/**
  @Summary
    Configures the GPIO pin, RE2, as an output.

  @Description
    Configures the GPIO pin, RE2, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RE2 as an output
    LED3_SetDigitalOutput();
    </code>

*/
#define LED3_SetDigitalOutput() (_TRISE2 = 0)
/**
  @Summary
    Sets the GPIO pin, RE4, high using LATE4.

  @Description
    Sets the GPIO pin, RE4, high using LATE4.

  @Preconditions
    The RE4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RE4 high (1)
    LED2_SetHigh();
    </code>

*/
#define LED2_SetHigh()          (_LATE4 = 1)
/**
  @Summary
    Sets the GPIO pin, RE4, low using LATE4.

  @Description
    Sets the GPIO pin, RE4, low using LATE4.

  @Preconditions
    The RE4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RE4 low (0)
    LED2_SetLow();
    </code>

*/
#define LED2_SetLow()           (_LATE4 = 0)
/**
  @Summary
    Toggles the GPIO pin, RE4, using LATE4.

  @Description
    Toggles the GPIO pin, RE4, using LATE4.

  @Preconditions
    The RE4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RE4
    LED2_Toggle();
    </code>

*/
#define LED2_Toggle()           (_LATE4 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RE4.

  @Description
    Reads the value of the GPIO pin, RE4.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RE4
    postValue = LED2_GetValue();
    </code>

*/
#define LED2_GetValue()         _RE4
/**
  @Summary
    Configures the GPIO pin, RE4, as an input.

  @Description
    Configures the GPIO pin, RE4, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RE4 as an input
    LED2_SetDigitalInput();
    </code>

*/
#define LED2_SetDigitalInput()  (_TRISE4 = 1)
/**
  @Summary
    Configures the GPIO pin, RE4, as an output.

  @Description
    Configures the GPIO pin, RE4, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RE4 as an output
    LED2_SetDigitalOutput();
    </code>

*/
#define LED2_SetDigitalOutput() (_TRISE4 = 0)
/**
  @Summary
    Sets the GPIO pin, RG13, high using LATG13.

  @Description
    Sets the GPIO pin, RG13, high using LATG13.

  @Preconditions
    The RG13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RG13 high (1)
    LED4_SetHigh();
    </code>

*/
#define LED4_SetHigh()          (_LATG13 = 1)
/**
  @Summary
    Sets the GPIO pin, RG13, low using LATG13.

  @Description
    Sets the GPIO pin, RG13, low using LATG13.

  @Preconditions
    The RG13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RG13 low (0)
    LED4_SetLow();
    </code>

*/
#define LED4_SetLow()           (_LATG13 = 0)
/**
  @Summary
    Toggles the GPIO pin, RG13, using LATG13.

  @Description
    Toggles the GPIO pin, RG13, using LATG13.

  @Preconditions
    The RG13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RG13
    LED4_Toggle();
    </code>

*/
#define LED4_Toggle()           (_LATG13 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RG13.

  @Description
    Reads the value of the GPIO pin, RG13.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RG13
    postValue = LED4_GetValue();
    </code>

*/
#define LED4_GetValue()         _RG13
/**
  @Summary
    Configures the GPIO pin, RG13, as an input.

  @Description
    Configures the GPIO pin, RG13, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RG13 as an input
    LED4_SetDigitalInput();
    </code>

*/
#define LED4_SetDigitalInput()  (_TRISG13 = 1)
/**
  @Summary
    Configures the GPIO pin, RG13, as an output.

  @Description
    Configures the GPIO pin, RG13, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RG13 as an output
    LED4_SetDigitalOutput();
    </code>

*/
#define LED4_SetDigitalOutput() (_TRISG13 = 0)

/**
    Section: Function Prototypes
*/
/**
  @Summary
    Configures the pin settings of the dsPIC33EP512MU810
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        PIN_MANAGER_Initialize();
    }
    </code>

*/
void PIN_MANAGER_Initialize (void);



#endif
