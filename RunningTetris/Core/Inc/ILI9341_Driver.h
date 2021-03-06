#include "main.h"
#include "../Inc/registers.h"
#include "../Inc/5x5_font.h"
#include <stdint.h>


#define BLACK       0x0000      
#define NAVY        0x000F      
#define DARKGREEN   0x03E0      
#define DARKCYAN    0x03EF      
#define MAROON      0x7800      
#define PURPLE      0x780F      
#define OLIVE       0x7BE0      
#define LIGHTGREY   0xC618      
#define DARKGREY    0x7BEF      
#define BLUE        0x001F      
#define GREEN       0x07E0      
#define CYAN        0x07FF      
#define RED         0xF800     
#define MAGENTA     0xF81F      
#define YELLOW      0xFFE0      
#define WHITE       0xFFFF      
#define ORANGE      0xFD20      
#define GREENYELLOW 0xAFE5     
#define PINK        0xF81F

extern volatile uint16_t TFTWIDTH ;
extern volatile uint16_t TFTHEIGHT ;


void delay(int value);
void WRStrobe();
void write8(uint8_t value);
void writeData8(uint8_t data);
void writeCmd8(uint8_t cmd);
void writeRegister8(uint8_t a, uint8_t d);
void writeRegister16(uint16_t a, uint16_t d);
void setPinDataOut();
void setPinDataIN();

void reset();
void initScreen();
void setAddrWindow(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void floodScreen(uint16_t color, uint32_t len);
void SetRotation(uint8_t rotation);
void setLR(void);
void fillScreen(uint16_t color);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, uint16_t fillcolor);
void drawFastVLine(int16_t x, int16_t y, int16_t length, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t length, uint16_t color);

void DrawChar(char Character, uint8_t X, uint8_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour);
void DrawText(const char* Text, uint8_t X, uint8_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour);