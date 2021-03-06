/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../Inc/ILI9341_Driver.h"
#include "../Inc/board.h"
#include "../Inc/piece.h"
#include "../Inc/shapes.h"

#include "stdlib.h"
#include "time.h"
#include "math.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* Buttons variables */
extern int bUP ;
extern int bDOWN ;
extern int bA ;
extern int bLEFT ;
extern int bRIGHT ;
extern int bSELECT ;
extern int bSTART ;
extern int bB ;

/* Buttons functions */
extern void (*fUP)(void);
extern void (*fDOWN)(void);
extern void (*fA)(void);
extern void (*fLEFT)(void);
extern void (*fRIGHT)(void);
extern void (*fSELECT)(void);
extern void (*fSTART)(void);
extern void (*fB)(void);

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void setAfunction(void (*f)(void));
void setBfunction(void (*f)(void));
void setUPfunction(void (*f)(void));
void setDOWNfunction(void (*f)(void));
void setRIGHTfunction(void (*f)(void));
void setLEFTfunction(void (*f)(void));
void setSTARTfunction(void (*f)(void));
void setSELECTfunction(void (*f)(void));

void emptyFunc(void);

void saveHighScore(int score);
int  getHighScore();
void updateHighScore(int score);

void initSystem();
void setSync(int us);

/* These functions must be called each loop iteration */
void SystemCheckUp(void);
void sync();

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
