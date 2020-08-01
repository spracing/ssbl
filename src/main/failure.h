#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    FAILURE_DEVELOPER = 0,

    FAILURE_EXTERNAL_FLASH_NOT_DETECTED,
    FAILURE_FIRMWARE_INVALID,
} failureMode_e;

#define WARNING_FLASH_DURATION_MS 25
#define WARNING_FLASH_COUNT 20
#define WARNING_PAUSE_DURATION_MS 500
#define WARNING_CODE_DURATION_LONG_MS 250
#define WARNING_CODE_DURATION_SHORT_MS 25

// failure
void indicateFailure(failureMode_e mode, int repeatCount);
void failureMode(failureMode_e mode);
