/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
 *
 * @skipline @version   PLIB Version 1.4.1
 *
 * @skipline  Device : dsPIC33CK64MC105
*/

/*
© [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB5 GPIO Pin which has a custom name of Supply to High
 * @pre      The RB5 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define Supply_SetHigh()          (_LATB5 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB5 GPIO Pin which has a custom name of Supply to Low
 * @pre      The RB5 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define Supply_SetLow()           (_LATB5 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB5 GPIO Pin which has a custom name of Supply
 * @pre      The RB5 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define Supply_Toggle()           (_LATB5 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB5 GPIO Pin which has a custom name of Supply
 * @param    none
 * @return   none  
 */
#define Supply_GetValue()         _RB5

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB5 GPIO Pin which has a custom name of Supply as Input
 * @param    none
 * @return   none  
 */
#define Supply_SetDigitalInput()  (_TRISB5 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB5 GPIO Pin which has a custom name of Supply as Output
 * @param    none
 * @return   none  
 */
#define Supply_SetDigitalOutput() (_TRISB5 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD13 GPIO Pin which has a custom name of LED to High
 * @pre      The RD13 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED_SetHigh()          (_LATD13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD13 GPIO Pin which has a custom name of LED to Low
 * @pre      The RD13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_SetLow()           (_LATD13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD13 GPIO Pin which has a custom name of LED
 * @pre      The RD13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED_Toggle()           (_LATD13 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD13 GPIO Pin which has a custom name of LED
 * @param    none
 * @return   none  
 */
#define LED_GetValue()         _RD13

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD13 GPIO Pin which has a custom name of LED as Input
 * @param    none
 * @return   none  
 */
#define LED_SetDigitalInput()  (_TRISD13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD13 GPIO Pin which has a custom name of LED as Output
 * @param    none
 * @return   none  
 */
#define LED_SetDigitalOutput() (_TRISD13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
