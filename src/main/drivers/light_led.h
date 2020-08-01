#pragma once

#include "common/sugar.h"

#define LED0_TOGGLE              BSP_LED_Toggle(LED1)
#define LED0_OFF                 BSP_LED_Off(LED1)
#define LED0_ON                  BSP_LED_On(LED1)

#define LED1_TOGGLE              NOOP
#define LED1_OFF                 NOOP
#define LED1_ON                  NOOP

#define LED2_TOGGLE              NOOP
#define LED2_OFF                 NOOP
#define LED2_ON                  NOOP

#define BEEP_OFF                 NOOP
#define BEEP_ON                  NOOP

void             BSP_LED_Init(Led_TypeDef Led);
void             BSP_LED_DeInit(Led_TypeDef Led);
void             BSP_LED_On(Led_TypeDef Led);
void             BSP_LED_Off(Led_TypeDef Led);
void             BSP_LED_Toggle(Led_TypeDef Led);
