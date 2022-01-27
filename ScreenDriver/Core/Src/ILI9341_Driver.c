#include "main.h"
#include "../Inc/registers.h"
#include "../Inc/ILI9341_Driver.h"

#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_tim.h"


#define TFTWIDTH 240
#define TFTHEIGHT 320

#define CS_PORT GPIOA
#define RD_PORT GPIOA
#define WR_PORT GPIOB
#define DC_PORT GPIOB
#define RST_PORT GPIOB
#define DATA_PORT GPIOA // pins [A1-A8]

#define CS_PIN LL_GPIO_PIN_10
#define RD_PIN LL_GPIO_PIN_15
#define WR_PIN LL_GPIO_PIN_14
#define DC_PIN LL_GPIO_PIN_13
#define RST_PIN LL_GPIO_PIN_9
#define DATA_PIN LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_8

/* Le masque depend des pins défini a la ligne précedente */
#define MASK_BSRR_DATA 0b00000001111111100000000111111110


 /* Fonction delay en ms (semble etre un peut rapide, j'ai peut etre fais une erreur) */
void delay(int value){
    LL_TIM_DisableCounter(TIM1);
    int psc = LL_TIM_GetPrescaler(TIM1);
    int arr = value * 16000 / psc;
    if (arr > 65535)
        arr = 65535;
    LL_TIM_SetCounter(TIM1,0);
    LL_TIM_SetAutoReload(TIM1,arr);
    LL_TIM_EnableCounter(TIM1);
    while(LL_TIM_IsActiveFlag_UPDATE(TIM1) == 0);
    LL_TIM_ClearFlag_UPDATE(TIM1);
    LL_TIM_DisableCounter(TIM1);
}

void WRStrobe(){                                                                    
    LL_GPIO_ResetOutputPin(WR_PORT,WR_PIN);                                                       
    LL_GPIO_SetOutputPin(WR_PORT,WR_PIN);                                                                 
  }


/* Envoie la valeur en entré sur les 8 pins data */
void write8(uint8_t value){
    DATA_PORT->BSRR = MASK_BSRR_DATA & (value << 1); // decalage de 1 vers la gauche car nos pins commencent sur A1 
    DATA_PORT->BSRR = MASK_BSRR_DATA & (~(value) << (1+16));
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
    delay(2);
    write8(d >> 24);
    delay(2);
    write8(d >> 16);
    delay(1);
    write8(d >> 8);
    delay(2);
    write8(d);
    LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
}


/* Probablement inutile */
/*void setPinDataOut(){ // Voir si mieux avec MODER
    GPIO_InitStruct.Pin = DATA_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(DATA_PORT, &GPIO_InitStruct);
}

void setPinDataIN(){ // Voir si mieux avec MODER
    GPIO_InitStruct.Pin = DATA_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(DATA_PORT, &GPIO_InitStruct);
}
*/


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
void init(){
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


// Je n'ai pas compris grand chose a la fonction -> copier coller de adafruit
/* Remplis une zone (definie avec setAddrWindow()) d'une couleur donné en parametre */
void flood(uint16_t color, uint32_t len) {  
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



void setLR(void) {
  LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
  writeRegisterPair(HX8347G_COLADDREND_HI, HX8347G_COLADDREND_LO, TFTWIDTH - 1);
  writeRegisterPair(HX8347G_ROWADDREND_HI, HX8347G_ROWADDREND_LO, TFTHEIGHT - 1);
  LL_GPIO_SetOutputPin(CS_PORT,CS_PIN);
}



void fillScreen(uint16_t color){
    setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1);
    flood(color, (long)TFTWIDTH * (long)TFTHEIGHT);
}

void drawPixel(int16_t x, int16_t y, uint16_t color){
    if ((x < 0) || (y < 0) || (x >= TFTWIDTH) || (y >= TFTHEIGHT))
        return;

    LL_GPIO_ResetOutputPin(CS_PORT,CS_PIN);
    setAddrWindow(x, y, TFTWIDTH - 1, TFTHEIGHT - 1);
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

    setAddrWindow(x1, y1, x2, y2);
    flood(fillcolor, (uint32_t)w * (uint32_t)h);
    setLR();
}