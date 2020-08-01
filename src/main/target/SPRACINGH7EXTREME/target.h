#ifndef __SPRACINGH7EXTREME_H
#define __SPRACINGH7EXTREME_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32h7xx_hal.h"

typedef enum
{
LED1 = 0,
LED_STATUS = LED1,
}Led_TypeDef;

#define LEDn                             ((uint8_t)1)

#define LED1_GPIO_PORT                   GPIOE
#define LED1_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()
#define LED1_PIN                         GPIO_PIN_3

#define USE_FLASH_W25Q128
#define USE_QUADSPI
#define USE_QUADSPI_FLASH_1

#endif
