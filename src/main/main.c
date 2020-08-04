/**
  ******************************************************************************
  * @file    USB_Device/DFU_Standalone/Src/main.c
  * @author  MCD Application Team
  * @brief   STM32H7xx HAL API Template project
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "platform.h"

#include "main.h"
#include "error.h"
#include "drivers/light_led.h"
#include "drivers/quadspi.h"
#include "drivers/flash_w25q128.h"
#include "failure.h"
#include "string.h"

#include "stm32h7xx.h"

/* Private typedef ----------------------------------------------------------- */
/* Private define ------------------------------------------------------------ */
/* Private macro ------------------------------------------------------------- */
/* Private variables --------------------------------------------------------- */

// configured via linker script when building binaries, usually 0x90000000 for memory mapped QSPI.
extern uint8_t __quad_spi_start;

#define FIRMWARE_OFFSET 0x00100000U
#define FIRMWARE_ADDRESS ((uint32_t)(&__quad_spi_start) + FIRMWARE_OFFSET)

QSPI_HandleTypeDef hqspi;

typedef  void (*pFunction)(void);

pFunction JumpToApplication;
uint32_t JumpAddress;

/* Private function prototypes ----------------------------------------------- */
static void SystemClock_Config(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

/* Private functions --------------------------------------------------------- */

static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = { 0 };


  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  UNUSED(GPIO_InitStruct);
}

__attribute__ ((noreturn)) void launchApplication(void)
{
    pFunction JumpToApplication;
    uint32_t JumpAddress;

    SCB_DisableICache();
    SCB_DisableDCache();

    LED0_ON;

    // See http://www.keil.com/support/docs/3913.htm

//    if( CONTROL_nPRIV_Msk & __get_CONTROL( ) )
//      {  /* not in privileged mode */
//        EnablePrivilegedMode( ) ;
//      }

    NVIC->ICER[ 0 ] = 0xFFFFFFFF ;
    NVIC->ICER[ 1 ] = 0xFFFFFFFF ;
    NVIC->ICER[ 2 ] = 0xFFFFFFFF ;
    NVIC->ICER[ 3 ] = 0xFFFFFFFF ;
    NVIC->ICER[ 4 ] = 0xFFFFFFFF ;
    NVIC->ICER[ 5 ] = 0xFFFFFFFF ;
    NVIC->ICER[ 6 ] = 0xFFFFFFFF ;
    NVIC->ICER[ 7 ] = 0xFFFFFFFF ;

    NVIC->ICPR[ 0 ] = 0xFFFFFFFF ;
    NVIC->ICPR[ 1 ] = 0xFFFFFFFF ;
    NVIC->ICPR[ 2 ] = 0xFFFFFFFF ;
    NVIC->ICPR[ 3 ] = 0xFFFFFFFF ;
    NVIC->ICPR[ 4 ] = 0xFFFFFFFF ;
    NVIC->ICPR[ 5 ] = 0xFFFFFFFF ;
    NVIC->ICPR[ 6 ] = 0xFFFFFFFF ;
    NVIC->ICPR[ 7 ] = 0xFFFFFFFF ;

    SysTick->CTRL = 0 ;
    SysTick->LOAD = 0; // Needed?
    SysTick->VAL  = 0; // Needed?
    SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk ;

    SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk | \
                     SCB_SHCSR_BUSFAULTENA_Msk | \
                     SCB_SHCSR_MEMFAULTENA_Msk ) ;

    SCB->VTOR = (uint32_t)(FIRMWARE_ADDRESS);

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t *) (FIRMWARE_ADDRESS)); // the main app's startup code (Reset_Handler) should do this anyway, but lets be safe.

    /* Jump to user application */
    JumpAddress = *(__IO uint32_t *) (FIRMWARE_ADDRESS + 4);

    JumpToApplication = (pFunction) JumpAddress;

    JumpToApplication();
    while (1) {
        // NOP
    };
}


bool isFirmwareRunnable(void)
{
    // first vector table entry is stack pointer, second is reset handler.
    uint32_t *vectorTable = (uint32_t *)FIRMWARE_ADDRESS;
    uint32_t firmwareStackPointer = *vectorTable;
    bool firmwareStackPointerIsInRam = (firmwareStackPointer & 0x2FFF0000) >= 0x20000000;

    return firmwareStackPointerIsInRam;
}

#define W25Q128_FLASH_SIZE      23u // 2^(23+1) = 16777216 B (16 MB) FLASH

static bool memoryMappedModeEnabled = false;

void detectFlashAndEnableMemoryMap(void)
{
    uint8_t response[3];

    QuadSpi_Init(&hqspi, 23); // Flash size unknown until chip detected.

    HAL_Delay(10);

    W25q_readJedec(&hqspi, response);

    bool haveJedecId = (response[0] == 0xEF && response[1] == 0x70 && response[2] == 0x18 );

    if (!haveJedecId) {
        return;
    }

    HAL_QSPI_DeInit(&hqspi);

    QuadSpi_Init(&hqspi, W25Q128_FLASH_SIZE);

    QuadSpiSetDivisor(&hqspi, QUADSPI_CLOCK_ULTRAFAST);

    memoryMappedModeEnabled = W25q_memoryMappedModeEnable(&hqspi);
}

bool haveFlashWithMemoryMapSupport(void)
{
    return memoryMappedModeEnabled;
}


__attribute__ ((noreturn)) void tryLaunch()
{
    if (!haveFlashWithMemoryMapSupport()) {
        failureMode(FAILURE_EXTERNAL_FLASH_NOT_DETECTED);
    }

    if (!isFirmwareRunnable()) {
        failureMode(FAILURE_FIRMWARE_INVALID);
    }

    // TODO verify flashed firmware using some hash here
/*
    if (!isFirmwareHashOK()) {
        failureMode(FAILURE_FIRMWARE_FAILED_HASH_VERIFICATION);
    }
*/

    launchApplication();
}

int main(void)
{
    HAL_Init();

    SystemClock_Config();
    SystemCoreClockUpdate();

    MPU_Config();

    CPU_CACHE_Enable();

    MX_GPIO_Init();

    BSP_LED_Init(LED1);
    BSP_LED_On(LED1);

    detectFlashAndEnableMemoryMap();

    HAL_PWREx_EnableUSBVoltageDetector();

    tryLaunch();
}

void resetWithoutDisablingCaches(void)
{
    __disable_irq();
    NVIC_SystemReset();
}

void HandleStuckSysTick(void)
{
  uint32_t tickStart = HAL_GetTick();
  uint32_t tickEnd = 0;

  int attemptsRemaining = 80 * 1000;
  while (((tickEnd = HAL_GetTick()) == tickStart) && --attemptsRemaining) {
      // H7 at 400Mhz - attemptsRemaining was reduced by debug build: 5,550, release build: 33,245
  }

  if (tickStart == tickEnd) {
      resetWithoutDisablingCaches();
  }
}

void SystemClock_Config(void)
{
    /**Supply configuration update enable
    */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    /**Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /**Macro to configure the PLL clock source
    */
    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE); // Not in BF

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON; // RCC_HSE_BYPASS ?
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = 2; // 8 / 4 * 400 / 2 = 400
    RCC_OscInitStruct.PLL.PLLQ = 8; // 8 / 4 * 400 / 8 = 100
    RCC_OscInitStruct.PLL.PLLR = 5; // 8 / 4 * 400 / 5 = 160 - CubeMX says "DIV1R not available on this package"
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;

    HAL_StatusTypeDef status = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (status!= HAL_OK) {
        _Error_Handler(FILENAME, LINENUMBER);
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      _Error_Handler(FILENAME, LINENUMBER);
    }



    // Configure PLL2 and PLL3
    // Use of PLL2 and PLL3 are not determined yet.
    // A review of total system wide clock requirements is necessary.


    // Configure SCGU (System Clock Generation Unit)
    // Select PLL as system clock source and configure bus clock dividers.
    //
    // Clock type and divider member names do not have direct visual correspondence.
    // Here is how these correspond:
    //   RCC_CLOCKTYPE_SYSCLK           sys_ck
    //   RCC_CLOCKTYPE_HCLK             AHBx (rcc_hclk1,rcc_hclk2,rcc_hclk3,rcc_hclk4)
    //   RCC_CLOCKTYPE_D1PCLK1          APB3 (rcc_pclk3)
    //   RCC_CLOCKTYPE_PCLK1            APB1 (rcc_pclk1)
    //   RCC_CLOCKTYPE_PCLK2            APB2 (rcc_pclk2)
    //   RCC_CLOCKTYPE_D3PCLK1          APB4 (rcc_pclk4)

    RCC_ClkInitStruct.ClockType = ( \
        RCC_CLOCKTYPE_SYSCLK | \
        RCC_CLOCKTYPE_HCLK | \
        RCC_CLOCKTYPE_D1PCLK1 | \
        RCC_CLOCKTYPE_PCLK1 | \
        RCC_CLOCKTYPE_PCLK2  | \
        RCC_CLOCKTYPE_D3PCLK1);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // = PLL1P = 400
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1; // = PLL1P(400) / 1 = 400
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;   // = SYSCLK(400) / 2 = 200
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  // = HCLK(200) / 2 = 100
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;  // = HCLK(200) / 2 = 100
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;  // = HCLK(200) / 2 = 100
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;  // = HCLK(200) / 2 = 100

    // For HCLK=200MHz with VOS1 range, ST recommended flash latency is 2WS.
    // RM0433 (Rev.5) Table 12. FLASH recommended number of wait states and programming delay

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
      _Error_Handler(FILENAME, LINENUMBER);
    }

//    /* SysTick_IRQn interrupt configuration */
//    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    /*activate CSI clock mondatory for I/O Compensation Cell*/
    __HAL_RCC_CSI_ENABLE() ;

    /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
    __HAL_RCC_SYSCFG_CLK_ENABLE() ;

    /* Enables the I/O Compensation Cell */
    HAL_EnableCompensationCell();

    HandleStuckSysTick();

    HAL_Delay(10);

    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

#if 0
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      _Error_Handler(FILENAME, LINENUMBER);
    }
#endif

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK; // RCC_QSPICLKSOURCE_D1HCLK is default
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      _Error_Handler(FILENAME, LINENUMBER);
    }
}

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  HAL_MPU_Disable();

  // the first stage bootloader may have write protected some memory regions to prevent corruption and catch errors
  // the second stage bootloader does not need this protection.

  // disable all existing regions
  for (uint8_t region = MPU_REGION_NUMBER0; region <= MPU_REGION_NUMBER15; region++) {
      MPU_InitStruct.Enable = MPU_REGION_DISABLE;
      MPU_InitStruct.Number = region;
      HAL_MPU_ConfigRegion(&MPU_InitStruct);
  }

  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t * file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line
   * number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
   * * line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
