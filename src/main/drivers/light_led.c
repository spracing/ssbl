#include "platform.h"

#ifdef LED_INVERTED
#define LED_ON_LEVEL                     GPIO_PIN_SET
#define LED_OFF_LEVEL                    GPIO_PIN_RESET
#define LED_GPIO_MODE                    GPIO_MODE_OUTPUT_PP

#else
#define LED_ON_LEVEL                     GPIO_PIN_RESET
#define LED_OFF_LEVEL                    GPIO_PIN_SET
#define LED_GPIO_MODE                    GPIO_MODE_OUTPUT_OD
#endif

void BSP_LED_Init(Led_TypeDef Led)
{
    /* On RevB board, LED1 and LED3 are connected to GPIOs */

    GPIO_InitTypeDef  gpio_initstruct;
    GPIO_TypeDef*     gpio_led;

    if (Led == LED1) {
        gpio_led = LED1_GPIO_PORT;
        /* Enable the GPIO_LED clock */
        LED1_GPIO_CLK_ENABLE();
        /* Configure the GPIO_LED pin */
        gpio_initstruct.Pin = LED1_PIN;

        gpio_initstruct.Mode = LED_GPIO_MODE;
        gpio_initstruct.Pull = GPIO_PULLUP;
        gpio_initstruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

        HAL_GPIO_Init(gpio_led, &gpio_initstruct);

        /* By default, turn off LED */
        HAL_GPIO_WritePin(gpio_led, LED1_PIN, LED_OFF_LEVEL);
    }
}

void BSP_LED_DeInit(Led_TypeDef Led)
{
    GPIO_InitTypeDef  gpio_init_structure;
    GPIO_TypeDef*     gpio_led;


    if (Led == LED1)
    {
        gpio_led = LED1_GPIO_PORT;
        /* Turn off LED */
        HAL_GPIO_WritePin(gpio_led, LED1_PIN, LED_OFF_LEVEL);
        /* Configure the GPIO_LED pin */
        gpio_init_structure.Pin = LED1_PIN;
        HAL_GPIO_DeInit(gpio_led, gpio_init_structure.Pin);
    }
}

void BSP_LED_On(Led_TypeDef Led)
{
    if (Led == LED1)
    {
      HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, LED_ON_LEVEL);
    }
}


void BSP_LED_Off(Led_TypeDef Led)
{
    if (Led == LED1)
    {
      HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, LED_OFF_LEVEL);
    }
}


void BSP_LED_Toggle(Led_TypeDef Led)
{

    if (Led == LED1)
    {
      HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    }

}
