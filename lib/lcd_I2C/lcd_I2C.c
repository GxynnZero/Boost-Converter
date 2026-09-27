#define FCY 100000000UL

#include <libpic30.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../mcc_generated_files/system/system.h"
#include "../../mcc_generated_files/i2c_host/i2c1.h"
#include "lcd_I2C.h"

/* Private Functions */
static void LCD_ExpanderWrite(LCD_I2C *lcd, uint8_t data);
static void LCD_PulseEnable(LCD_I2C *lcd, uint8_t data);
static void LCD_Write4Bits(LCD_I2C *lcd, uint8_t value);
static void LCD_Send(LCD_I2C *lcd, uint8_t value, uint8_t mode);

/*----------------------------------------------------------*/

static void LCD_ExpanderWrite(LCD_I2C *lcd, uint8_t data)
{
    uint8_t tx = data | lcd->backlight;

    while(!I2C1_Write(lcd->address, &tx, 1));

    while(I2C1_IsBusy());
}

/*----------------------------------------------------------*/

static void LCD_PulseEnable(LCD_I2C *lcd, uint8_t data)
{
    LCD_ExpanderWrite(lcd, data | LCD_EN);
    __delay_us(1);

    LCD_ExpanderWrite(lcd, data & ~LCD_EN);
    __delay_us(50);
}

/*----------------------------------------------------------*/

static void LCD_Write4Bits(LCD_I2C *lcd, uint8_t value)
{
    LCD_ExpanderWrite(lcd, value);
    LCD_PulseEnable(lcd, value);
}

/*----------------------------------------------------------*/

static void LCD_Send(LCD_I2C *lcd, uint8_t value, uint8_t mode)
{
    LCD_Write4Bits(lcd, (value & 0xF0) | mode);
    LCD_Write4Bits(lcd, ((value << 4) & 0xF0) | mode);
}

/*----------------------------------------------------------*/

void LCD_Command(LCD_I2C *lcd, uint8_t cmd)
{
    LCD_Send(lcd, cmd, 0);
}

/*----------------------------------------------------------*/

void LCD_WriteChar(LCD_I2C *lcd, char c)
{
    LCD_Send(lcd, (uint8_t)c, LCD_RS);
}

/*----------------------------------------------------------*/

void LCD_Print(LCD_I2C *lcd, const char *str)
{
    while(*str)
    {
        LCD_WriteChar(lcd, *str++);
    }
}

/*----------------------------------------------------------*/

void LCD_Clear(LCD_I2C *lcd)
{
    LCD_Command(lcd, LCD_CLEARDISPLAY);
    __delay_ms(2);
}

/*----------------------------------------------------------*/

void LCD_Home(LCD_I2C *lcd)
{
    LCD_Command(lcd, LCD_RETURNHOME);
    __delay_ms(2);
}

/*----------------------------------------------------------*/

void LCD_SetCursor(LCD_I2C *lcd, uint8_t col, uint8_t row)
{
    static const uint8_t row_offsets[] =
    {
        0x00,
        0x40,
        0x14,
        0x54
    };

    if(lcd->rows == 0)
        return;

    if(row >= lcd->rows)
        row = lcd->rows - 1;

    LCD_Command(lcd,
                LCD_SETDDRAMADDR |
                (col + row_offsets[row]));
}

/*----------------------------------------------------------*/

void LCD_Backlight(LCD_I2C *lcd, bool enable)
{
    lcd->backlight = enable ? LCD_BL : LCD_NOBACKLIGHT;
    LCD_ExpanderWrite(lcd, 0x00);
}

/*----------------------------------------------------------*/

void LCD_Init(LCD_I2C *lcd,
              uint8_t address,
              uint8_t cols,
              uint8_t rows)
{
    lcd->address = address;
    lcd->cols = cols;
    lcd->rows = rows;

    lcd->backlight = LCD_BL;

    lcd->displayFunction = LCD_4BITMODE |
                           LCD_5x8DOTS;

    if(rows > 1)
        lcd->displayFunction |= LCD_2LINE;

    __delay_ms(50);

    LCD_ExpanderWrite(lcd, 0x00);
    __delay_ms(50);

    /* Initialize LCD into 4-bit mode */

    LCD_Write4Bits(lcd, 0x30);
    __delay_ms(5);

    LCD_Write4Bits(lcd, 0x30);
    __delay_us(150);

    LCD_Write4Bits(lcd, 0x30);
    __delay_us(150);

    LCD_Write4Bits(lcd, 0x20);

    /* Function Set */

    LCD_Command(lcd,
                LCD_FUNCTIONSET |
                lcd->displayFunction);

    /* Display Control */

    lcd->displayControl =
            LCD_DISPLAYON |
            LCD_CURSOROFF |
            LCD_BLINKOFF;

    LCD_Command(lcd,
                LCD_DISPLAYCONTROL |
                lcd->displayControl);

    /* Clear */

    LCD_Clear(lcd);

    /* Entry Mode */

    lcd->displayMode =
            LCD_ENTRYLEFT |
            LCD_ENTRYSHIFTDEC;

    LCD_Command(lcd,
                LCD_ENTRYMODESET |
                lcd->displayMode);

    LCD_Home(lcd);
}