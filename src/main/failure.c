
#include <stdbool.h>
#include <stdint.h>

#include "platform.h"

#include "drivers/light_led.h"

#include "failure.h"

static void indicate(uint8_t count, uint16_t duration)
{
    if (!count) {
        return;
    }
    LED0_OFF;
    LED1_ON;

    while (count--) {
        LED0_TOGGLE;
        LED1_TOGGLE;
        BEEP_ON;
        HAL_Delay(duration);

        LED0_TOGGLE;
        LED1_TOGGLE;
        BEEP_OFF;
        HAL_Delay(duration);
    }
}

void indicateFailure(failureMode_e mode, int codeRepeatsRemaining)
{
    while (codeRepeatsRemaining--) {
        indicate(WARNING_FLASH_COUNT, WARNING_FLASH_DURATION_MS);

        HAL_Delay(WARNING_PAUSE_DURATION_MS);

        indicate(mode + 1, WARNING_CODE_DURATION_LONG_MS);

        HAL_Delay(1000);
    }
}

__attribute__ ((noreturn)) void failureMode(failureMode_e mode)
{
    while(1) {
        indicateFailure(mode, 1);
    }
}
