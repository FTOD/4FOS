#include "../Inc/ILI9341_Driver.h"

#define CS_PORT GPIOB
#define RD_PORT GPIOB
#define WR_PORT GPIOB
#define DC_PORT GPIOB
#define RST_PORT GPIOB
#define DATA_PORT GPIOB 

#define CS_PIN LL_GPIO_PIN_3
#define RD_PIN LL_GPIO_PIN_5
#define WR_PIN LL_GPIO_PIN_6
#define DC_PIN LL_GPIO_PIN_4
#define RST_PIN LL_GPIO_PIN_9
#define DATA_PIN LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_9|LL_GPIO_PIN_10|LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15

/* Le masque depend des pins défini a la ligne précedente */
#define MASK_BSRR_DATA 0b11110111100000001111011110000000

unsigned int tabPinData[8] = {LL_GPIO_PIN_15 ,LL_GPIO_PIN_14,LL_GPIO_PIN_13, LL_GPIO_PIN_12, LL_GPIO_PIN_10, LL_GPIO_PIN_9, LL_GPIO_PIN_8, LL_GPIO_PIN_7};

/* Global Variables ------------------------------------------------------------------*/
volatile uint16_t TFTWIDTH = 240 ;
volatile uint16_t TFTHEIGHT = 320;


 /* Fonction delay en ms  */
void delay(int value){
    LL_TIM_DisableCounter(TIM1);
    int psc = LL_TIM_GetPrescaler(TIM1);
    int arr = value * 12000 / psc;
    if (arr > 65535)
        arr = 65535;
    LL_TIM_SetCounter(TIM1,0);
    LL_TIM_SetAutoReload(TIM1,arr);
    LL_TIM_EnableCounter(TIM1);
    while(LL_TIM_IsActiveFlag_UPDATE(TIM1) == 0);
    LL_TIM_ClearFlag_UPDATE(TIM1);
    LL_TIM_DisableCounter(TIM1);
}


 /* Fonction delay en microsec  */
void delayMicrosecond(int value){
    LL_TIM_DisableCounter(TIM2);
    int arr = value * 96;
    if (arr > 4294967295)
        arr = 4294967295;
    LL_TIM_SetCounter(TIM2,0);
    LL_TIM_SetAutoReload(TIM2,arr);
    LL_TIM_EnableCounter(TIM2);
    while(LL_TIM_IsActiveFlag_UPDATE(TIM2) == 0);
    LL_TIM_ClearFlag_UPDATE(TIM2);
    LL_TIM_DisableCounter(TIM2);
}



void WRStrobe(){                                                                    
    LL_GPIO_ResetOutputPin(WR_PORT,WR_PIN);                                                       
    LL_GPIO_SetOutputPin(WR_PORT,WR_PIN);                                                                 
  }


/* Envoie la valeur en entré sur les 8 pins data */
// void write8(uint8_t value){
//     DATA_PORT->BSRR = MASK_BSRR_DATA & (value << 1); // decalage de 1 vers la gauche car nos pins commencent sur A1 
//     DATA_PORT->BSRR = MASK_BSRR_DATA & (~(value) << (1+16));
//     WRStrobe();
// }

void write8(uint8_t value){
    uint bit_value;
    for(int i = 0; i < 8; i++){
        bit_value = (( value >> i) & 1);
        if (bit_value == 1){
            LL_GPIO_SetOutputPin(DATA_PORT,tabPinData[i]);
        }
        else{
            LL_GPIO_ResetOutputPin(DATA_PORT, tabPinData[i]);
        }
    }
    WRStrobe();
}

/* Envoie des données à l'ecran (non utiliser)*/
void writeData8(uint8_t data){
    LL_GPIO_SetOutputPin(DC_PORT,DC_PIN);
    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
    write8(data);
    LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);

}

/* Envoie une commande à l'ecran (non utiliser)*/
void writeCmd8(uint8_t cmd){
    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
    LL_GPIO_ResetOutputPin(DC_PORT,DC_PIN);
    write8(cmd);
    LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
}


/* Ecrit une donnée d dans un registre de l'ecran à l'adresse a (version 8bits) */
void writeRegister8(uint8_t a, uint8_t d){
    LL_GPIO_ResetOutputPin(DC_PORT,DC_PIN);
    write8(a);
    LL_GPIO_SetOutputPin(DC_PORT,DC_PIN);
    write8(d);
}


/* Ecrit une donnée d dans un registre de l'ecran à l'adresse a (version 16bits) */
void writeRegister16(uint16_t a, uint16_t d){ 
    uint8_t hi, lo;
    hi = (a) >> 8;
    lo = (a);
    LL_GPIO_ResetOutputPin(DC_PORT,DC_PIN);
    write8(hi);
    write8(lo);
    hi = (d) >> 8;     
    lo = (d);
    LL_GPIO_SetOutputPin(DC_PORT,DC_PIN);
    write8(hi);
    write8(lo);

}

/*  */
void writeRegisterPair(uint8_t aH, uint8_t aL, uint16_t d){
    uint8_t hi = (d) >> 8, lo = (d);
    LL_GPIO_ResetOutputPin(DC_PORT,DC_PIN);
    write8(aH);
    LL_GPIO_SetOutputPin(DC_PORT,DC_PIN);              
    write8(hi);
    LL_GPIO_ResetOutputPin(DC_PORT,DC_PIN);            
    write8(aL);
    LL_GPIO_SetOutputPin(DC_PORT,DC_PIN);
    write8(lo); 
}



/* Ecrit une donnée d dans un registre de l'ecran à l'adresse a (version 32bits) */
void writeRegister32(uint8_t r, uint32_t d) {
    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
    LL_GPIO_ResetOutputPin(DC_PORT,DC_PIN);
    write8(r);
    LL_GPIO_SetOutputPin(DC_PORT,DC_PIN);
    delayMicrosecond(10);
    write8(d >> 24);
    delayMicrosecond(10);
    write8(d >> 16);
    delayMicrosecond(10);
    write8(d >> 8);
    delayMicrosecond(10);
    write8(d);
    LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
}



/* Fonction reset de l'ecran */
void reset(){ 
    LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
    LL_GPIO_SetOutputPin(WR_PORT,WR_PIN);
    LL_GPIO_SetOutputPin(RD_PORT,RD_PIN);

    LL_GPIO_ResetOutputPin(RST_PORT,RST_PIN);
    delay(2);
    LL_GPIO_SetOutputPin(RST_PORT,RST_PIN);

    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
    LL_GPIO_ResetOutputPin(DC_PORT,DC_PIN);
    write8(0x00);
    for (uint8_t i = 0; i < 3; i++)
        WRStrobe(); // Three extra 0x00s
    LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
}



/* Initialise l'ecran */
void initScreen(){
    LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
    LL_GPIO_SetOutputPin(WR_PORT,WR_PIN);
    LL_GPIO_SetOutputPin(RD_PORT,RD_PIN);
    LL_GPIO_SetOutputPin(DC_PORT,DC_PIN);
    LL_GPIO_SetOutputPin(RST_PORT,RST_PIN);
 
    reset(); 
    delay(200);

    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
    writeRegister8(ILI9341_SOFTRESET, 0);
    delay(50);
    writeRegister8(ILI9341_DISPLAYOFF, 0);

    writeRegister8(ILI9341_POWERCONTROL1, 0x23);
    writeRegister8(ILI9341_POWERCONTROL2, 0x10);
    writeRegister16(ILI9341_VCOMCONTROL1, 0x2B2B);
    writeRegister8(ILI9341_VCOMCONTROL2, 0xC0);
    writeRegister8(ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    writeRegister8(ILI9341_PIXELFORMAT, 0x55);
    writeRegister16(ILI9341_FRAMECONTROL, 0x001B);
    writeRegister8(ILI9341_ENTRYMODE, 0x07);

    writeRegister8(ILI9341_SLEEPOUT, 0);
    delay(150);
    writeRegister8(ILI9341_DISPLAYON, 0);
    delay(500);

    setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);
}

/* Definie une zone dans laquelle on souhaite faire un traitement (fillRect,fillScreen...) */
void setAddrWindow(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2) { 
    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);

    uint32_t t;
    t = X1;
    t <<= 16;
    t |= X2;
    writeRegister32(ILI9341_COLADDRSET, t); 
    t = Y1;
    t <<= 16;
    t |= Y2;
    writeRegister32(ILI9341_PAGEADDRSET, t);

   LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
}


/* Remplis une zone (definie avec setAddrWindow()) d'une couleur donné en parametre */
void floodScreen(uint16_t color, uint32_t len) {  
    uint16_t blocks;
    uint8_t i, hi = color >> 8, lo = color;
    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
    LL_GPIO_ResetOutputPin(DC_PORT,DC_PIN);
    write8(ILI9341_MEMORYWRITE);

    LL_GPIO_SetOutputPin(DC_PORT,DC_PIN);
    write8(hi);
    write8(lo);

    len--;
    blocks = (uint16_t)(len / 64); // 64 pixels/block
    if (hi == lo) {
    // High and low bytes are identical.  Leave prior data
    // on the port(s) and just toggle the write strobe.
    while (blocks--) {
        i = 16; // 64 pixels/block / 4 pixels/pass
        do {
        WRStrobe();
        WRStrobe();
        WRStrobe();
        WRStrobe(); // 2 bytes/pixel
        WRStrobe();
        WRStrobe();
        WRStrobe();
        WRStrobe(); // x 4 pixels
        } while (--i);
    }
    // Fill any remaining pixels (1 to 64)
    for (i = (uint8_t)len & 63; i--;) {
        WRStrobe();
        WRStrobe();
    }
    } else {
    while (blocks--) {
        i = 16; // 64 pixels/block / 4 pixels/pass
        do {
        write8(hi);
        write8(lo);
        write8(hi);
        write8(lo);
        write8(hi);
        write8(lo);
        write8(hi);
        write8(lo);
        } while (--i);
    }
    for (i = (uint8_t)len & 63; i--;) {
        write8(hi);
        write8(lo);
    }
    }
    LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
}


void SetRotation(uint8_t rotation){
    uint16_t t = 0;

    switch(rotation){
        case 2: // Vertical
            TFTHEIGHT = 240;
            TFTWIDTH = 320;
            t = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR;
            break;

        case 3: // Horizontal
            TFTHEIGHT = 320;
            TFTWIDTH = 240;
            t = ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
            break;

        case 0: // Vertical
            TFTHEIGHT = 240;
            TFTWIDTH = 320;
            t = ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR;
            break;

        case 1: // Horizontal
            TFTHEIGHT = 320;
            TFTWIDTH = 240;
            t = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
            break;

        default:
            break;
    }
    writeRegister8(ILI9341_MADCTL, t);
    setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1); // CS_IDLE happens here
}


void setLR(void) {
  LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
  writeRegisterPair(HX8347G_COLADDREND_HI, HX8347G_COLADDREND_LO, TFTWIDTH - 1);
  writeRegisterPair(HX8347G_ROWADDREND_HI, HX8347G_ROWADDREND_LO, TFTHEIGHT - 1);
  LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
}


void fillScreen(uint16_t color){
    setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);
    floodScreen(color, (long)TFTWIDTH * (long)TFTHEIGHT);
}

void drawPixel(int16_t x, int16_t y, uint16_t color){
    if ((x < 0) || (y < 0) || (x >= TFTWIDTH) || (y >= TFTHEIGHT))
        return;

    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
    setAddrWindow(TFTWIDTH-x, TFTHEIGHT-y, TFTWIDTH - 1, TFTHEIGHT - 1);
    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
    LL_GPIO_ResetOutputPin(DC_PORT,DC_PIN);
    write8(ILI9341_MEMORYWRITE);
    LL_GPIO_SetOutputPin(DC_PORT,DC_PIN);
    write8(color >> 8);
    write8(color);
  
    LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
}



void fillRect(int16_t x1, int16_t y1, int16_t w, int16_t h, uint16_t fillcolor) {
    int16_t x2, y2;

    // Initial off-screen clipping
    if ((w <= 0) || (h <= 0) || (x1 >= TFTWIDTH) || (y1 >= TFTHEIGHT) ||
        ((x2 = x1 + w - 1) < 0) || ((y2 = y1 + h - 1) < 0))
        return;
    if (x1 < 0) { // Clip left
        w += x1;
        x1 = 0;
    }
    if (y1 < 0) { // Clip top
        h += y1;
        y1 = 0;
    }
    if (x2 >= TFTWIDTH) { // Clip right
        x2 = TFTWIDTH - 1;
        w = x2 - x1 + 1;
    }
    if (y2 >= TFTHEIGHT) { // Clip bottom
        y2 = TFTHEIGHT - 1;
        h = y2 - y1 + 1;
    }
    
    //modif
    setAddrWindow(TFTWIDTH-x2, TFTHEIGHT-y2, TFTWIDTH-x1, TFTHEIGHT-y1);
    floodScreen(fillcolor, (uint32_t)w * (uint32_t)h);
    setLR();
}


void drawFastHLine(int16_t x, int16_t y, int16_t length, uint16_t color) {
    int16_t x2;

    // Initial off-screen clipping
    if ((length <= 0) || (y < 0) || (y >= TFTHEIGHT) || (x >= TFTWIDTH) ||
        ((x2 = (x + length - 1)) < 0))
        return;

    if (x < 0) { // Clip left
        length += x;
        x = 0;
    }
    if (x2 >= TFTWIDTH) { // Clip right
        x2 = TFTWIDTH - 1;
        length = x2 - x + 1;
    }
    setAddrWindow(x, y, x2, y);
    floodScreen(color, length);
    setLR();
}


void drawFastVLine(int16_t x, int16_t y, int16_t length, uint16_t color) {
    int16_t y2;

    // Initial off-screen clipping
    if ((length <= 0) || (x < 0) || (x >= TFTWIDTH) || (y >= TFTHEIGHT) ||
        ((y2 = (y + length - 1)) < 0))
        return;
    if (y < 0) { // Clip top
        length += y;
        y = 0;
    }
    if (y2 >= TFTHEIGHT) { // Clip bottom
        y2 = TFTHEIGHT - 1;
        length = y2 - y + 1;
    }
    setAddrWindow(x, y, x, y2);
    floodScreen(color, length);
    setLR();
}

void DrawChar(char Character, uint8_t X, uint8_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour) 
{
	uint8_t function_char;
    uint8_t i,j;
		
	function_char = Character;
		
    if (function_char < ' ') {
        Character = 0;
    }else{
        function_char -= 32;
	}
   	
	char temp[CHAR_WIDTH];
	for(uint8_t k = 0; k<CHAR_WIDTH; k++){
	    temp[k] = font[function_char][k];
	}	
    // Draw pixels
	fillRect(X, Y, CHAR_WIDTH*Size, CHAR_HEIGHT*Size, Background_Colour);
    for (j=0; j<CHAR_WIDTH; j++) {
        for (i=0; i<CHAR_HEIGHT; i++) {
            if (temp[j] & (1<<i)) {			
                if(Size == 1){
                    drawPixel(X+CHAR_WIDTH-i, Y+j, Colour);
                }else{
                    fillRect(X+ CHAR_WIDTH*Size -(i*Size), Y+(j*Size), Size, Size, Colour);
                }
            }						
        }
    }
}

void DrawText(const char* Text, uint8_t X, uint8_t Y, uint16_t Colour, uint16_t Size, uint16_t Background_Colour){
    while (*Text) {
        DrawChar(*Text++, X, Y, Colour, Size, Background_Colour);
        Y += CHAR_WIDTH*Size;
    }
}
