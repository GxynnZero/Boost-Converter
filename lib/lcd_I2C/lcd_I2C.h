/*
 * File:   lcd_I2C.h
 * Author: Gagan Shetty
 *
 * Created on September 13, 2026, 12:47 PM
 */

#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>
#include <stdbool.h>

/* Commands */
#define LCD_CLEARDISPLAY      0x01
#define LCD_RETURNHOME        0x02
#define LCD_ENTRYMODESET      0x04
#define LCD_DISPLAYCONTROL    0x08
#define LCD_CURSORSHIFT       0x10
#define LCD_FUNCTIONSET       0x20
#define LCD_SETCGRAMADDR      0x40
#define LCD_SETDDRAMADDR      0x80

/* Entry Mode */
#define LCD_ENTRYRIGHT        0x00
#define LCD_ENTRYLEFT         0x02
#define LCD_ENTRYSHIFTINC     0x01
#define LCD_ENTRYSHIFTDEC     0x00

/* Display Control */
#define LCD_DISPLAYON         0x04
#define LCD_DISPLAYOFF        0x00
#define LCD_CURSORON          0x02
#define LCD_CURSOROFF         0x00
#define LCD_BLINKON           0x01
#define LCD_BLINKOFF          0x00

/* Cursor Shift */
#define LCD_DISPLAYMOVE       0x08
#define LCD_CURSORMOVE        0x00
#define LCD_MOVERIGHT         0x04
#define LCD_MOVELEFT          0x00

/* Function Set */
#define LCD_8BITMODE          0x10
#define LCD_4BITMODE          0x00
#define LCD_2LINE             0x08
#define LCD_1LINE             0x00
#define LCD_5x10DOTS          0x04
#define LCD_5x8DOTS           0x00

/* Backlight */
#define LCD_BACKLIGHT         0x08
#define LCD_NOBACKLIGHT       0x00

/* PCF8574 Pin Mapping (YWRobot) */
#define LCD_RS                0x01
#define LCD_RW                0x02
#define LCD_EN                0x04
#define LCD_BL                0x08

#define LCD_D4                0x10
#define LCD_D5                0x20
#define LCD_D6                0x40
#define LCD_D7                0x80

typedef struct {
    uint8_t address;

    uint8_t cols;
    uint8_t rows;

    uint8_t displayFunction;
    uint8_t displayControl;
    uint8_t displayMode;

    uint8_t backlight;

} LCD_I2C;

/* Initialization */
void LCD_Init(LCD_I2C *lcd,
        uint8_t address,
        uint8_t cols,
        uint8_t rows);

/* Basic Commands */
void LCD_Clear(LCD_I2C *lcd);
void LCD_Home(LCD_I2C *lcd);
void LCD_Command(LCD_I2C *lcd, uint8_t cmd);

/* Data */
void LCD_WriteChar(LCD_I2C *lcd, char c);
void LCD_Print(LCD_I2C *lcd, const char *s);

/* Cursor */
void LCD_SetCursor(LCD_I2C *lcd,
        uint8_t col,
        uint8_t row);

/* Display */
void LCD_Display(LCD_I2C *lcd);
void LCD_NoDisplay(LCD_I2C *lcd);

void LCD_Cursor(LCD_I2C *lcd);
void LCD_NoCursor(LCD_I2C *lcd);

void LCD_Blink(LCD_I2C *lcd);
void LCD_NoBlink(LCD_I2C *lcd);

/* Backlight */
void LCD_Backlight(LCD_I2C *lcd, bool enable);

/* Text Direction */
void LCD_LeftToRight(LCD_I2C *lcd);
void LCD_RightToLeft(LCD_I2C *lcd);

/* Scrolling */
void LCD_ScrollLeft(LCD_I2C *lcd);
void LCD_ScrollRight(LCD_I2C *lcd);

/* Custom Characters */
void LCD_CreateChar(LCD_I2C *lcd,
        uint8_t location,
        const uint8_t charmap[8]);

#endif