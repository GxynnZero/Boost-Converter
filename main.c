#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/pwm_hs/pwm.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/timer/sccp1.h"
#include "mcc_generated_files/adc/adc1.h"
#include "mcc_generated_files/X2Cscope/X2Cscope.h"
#define FCY 100000000UL
#include "libpic30.h"
#include "lib/LCD_I2C/LCD_I2C.h"
#include "stdio.h"
#define DUTY 0.5

volatile uint16_t pot = 0;
volatile float vin = 0;
volatile float vout = 0;
volatile float Iin = 0;
volatile float Iout = 0;
volatile int Flag = 0;

void timer_interrupt_function(void) {
    Flag = 1;
    X2Cscope_Update();
    X2Cscope_Communicate();
}

int main(void) {
    SYSTEM_Initialize();
    X2Cscope_Init();
    SCCP1_Timer_TimeoutCallbackRegister(timer_interrupt_function);
    SCCP1_Timer_Start();
    PWM_GeneratorEnable(PWM_GENERATOR_3);

    LED_SetHigh();
    Supply_SetHigh();

    for (int duty = 0; duty <= 4000 * DUTY; duty += 200) {
        while (Flag == 0);
        PWM_DutyCycleSet(PWM_GENERATOR_3, duty);
        Flag = 0;
    }

    LCD_I2C lcd;
    char buffer[16];

    LCD_Init(&lcd, 0x27, 16, 2);
    LCD_Backlight(&lcd, true);
    LCD_SetCursor(&lcd, 0, 0);
    LCD_Print(&lcd, "Boost converter");
    LCD_SetCursor(&lcd, 4, 1);
    LCD_Print(&lcd, "Readings");
    __delay_ms(2000);
    
    PWM_TriggerCompareValueSet(PWM_GENERATOR_3, PG3DC);
    
    while (1) {
        ADC1_SoftwareTriggerEnable();
        if (ADC1_IsConversionComplete(Channel_AN1)) {
            pot = ADC1_ConversionResultGet(Channel_AN1);
            PWM_DutyCycleSet(PWM_GENERATOR_3, pot * DUTY);
        }
        if (ADC1_IsConversionComplete(Channel_AN2)) {
            Iout = ADC1_ConversionResultGet(Channel_AN2);
            Iout = Iout * 1.188 * 0.001;
        }
        if (ADC1_IsConversionComplete(Channel_AN3)) {
            vout = ADC1_ConversionResultGet(Channel_AN3);
            vout = vout* 8.98 * 0.001 - 0.61;
        }
        if (ADC1_IsConversionComplete(Channel_AN4)) {
            Iin = ADC1_ConversionResultGet(Channel_AN4);
            Iin = Iin * 0.0165 - 0.03;
        }
        if (ADC1_IsConversionComplete(Channel_AN9)) {
            vin = ADC1_ConversionResultGet(Channel_AN9);
            vin = vin * 4.95 * 0.001 - 1.29;
        }

        LCD_Clear(&lcd);
        LCD_SetCursor(&lcd, 0, 0);
        sprintf(buffer, "Vi:%.2f", vin);
        LCD_Print(&lcd, buffer);
        
        LCD_SetCursor(&lcd, 0, 1);
        sprintf(buffer, "Ii:%.2f", Iin);
        LCD_Print(&lcd, buffer);
        __delay_ms(1000);
        
        LCD_Clear(&lcd);
        LCD_SetCursor(&lcd, 0, 0);
        sprintf(buffer, "Vo:%.2f", vout);
        LCD_Print(&lcd, buffer);
        
        LCD_SetCursor(&lcd, 0, 1);
        sprintf(buffer, "Io:%.2f", Iout);
        LCD_Print(&lcd, buffer);
        __delay_ms(1000);
    }
}
