#pragma once

#include "stm32h7xx_hal.h"

// Chip Unique ID on H7
#define U_ID_0 (*(uint32_t*)0x1FF1E800)
#define U_ID_1 (*(uint32_t*)0x1FF1E804)
#define U_ID_2 (*(uint32_t*)0x1FF1E808)

#include "target.h"

#ifdef DEBUG
#define FILENAME __FILE__
#define LINENUMBER __LINE__
#else
#define FILENAME ""
#define LINENUMBER __LINE__
#endif
