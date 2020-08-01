/*
 * This program is driver for QUADSPI.
 * Copyright (C) 2020  Igor Misic, igy1000mb@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 *
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quadspi.h"
#include "error.h"

#define QUADSPI_DEFAULT_TIMEOUT 200

bool QuadSpi_Init(QSPI_HandleTypeDef *hqspi, uint8_t flashSize)
{
    bool success = true;
    hqspi->Instance = QUADSPI;
    hqspi->Init.ClockPrescaler      = QUADSPI_CLOCK_INITIALISATION;
    hqspi->Init.FifoThreshold       = 1;
    hqspi->Init.SampleShifting      = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
    hqspi->Init.FlashSize           = flashSize;                        // (2^flashSize + 1) = flash size in bytes
    hqspi->Init.ChipSelectHighTime  = QSPI_CS_HIGH_TIME_1_CYCLE;
    hqspi->Init.ClockMode           = QSPI_CLOCK_MODE_0;
    hqspi->Init.FlashID             = QSPI_FLASH_ID_1;
    hqspi->Init.DualFlash           = QSPI_DUALFLASH_DISABLE;

    if (HAL_QSPI_Init(hqspi) != HAL_OK)
    {
        success = false;
    }
    return success;
}

void QuadSpiSetDivisor(QSPI_HandleTypeDef *hqspi, uint16_t divisor)
{
    if (HAL_QSPI_DeInit(hqspi) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    hqspi->Init.ClockPrescaler = divisor;

    HAL_QSPI_Init(hqspi);
}

bool QuadSpiInstruction(QSPI_HandleTypeDef *hqspi, uint8_t instruction)
{
    bool success = false;

    HAL_StatusTypeDef status;
    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_NONE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_NONE;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction         = instruction;
    cmd.DummyCycles         = 0u;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);

    if(status == HAL_OK){
        success = true;
    }

    return success;
}

bool QuadSpiInstructionWithAddress(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint32_t address, uint32_t addressSize)
{
    bool success = false;

    HAL_StatusTypeDef status;
    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_1_LINE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_NONE;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction         = instruction;
    cmd.Address             = address;
    cmd.AddressSize         = addressSize;
    cmd.DummyCycles         = 0u;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);

    if(status == HAL_OK){
        success = true;
    }

    return success;
}

bool QuadSpiReceive1Line(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint8_t dummyCycles, uint8_t *in, uint16_t length)
{
    HAL_StatusTypeDef status;

    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_NONE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_1_LINE;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction         = instruction;
    cmd.DummyCycles         = dummyCycles;
    cmd.NbData              = length;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);
    bool timeout = (status != HAL_OK);
    if (!timeout) {
        status = HAL_QSPI_Receive(hqspi, in, QUADSPI_DEFAULT_TIMEOUT);

        timeout = (status != HAL_OK);
    }

    if (timeout) {
        return false;
    }

    return true;
}

bool QuadSpiReceive4Lines(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint8_t dummyCycles, uint8_t *in, uint16_t length)
{
    HAL_StatusTypeDef status;

    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_4_LINES;
    cmd.AddressMode         = QSPI_ADDRESS_NONE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_4_LINES;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction         = instruction;
    cmd.DummyCycles         = dummyCycles;
    cmd.NbData              = length;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);
    bool timeout = (status != HAL_OK);
    if (!timeout) {
        status = HAL_QSPI_Receive(hqspi, in, QUADSPI_DEFAULT_TIMEOUT);

        timeout = (status != HAL_OK);
    }

    if (timeout) {
        return false;
    }

    return true;
}

bool QuadSpiReceiveWithAddress1Line(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint8_t dummyCycles, uint32_t address, uint8_t addressSize, uint8_t *in, int length)
{
    HAL_StatusTypeDef status;

    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_1_LINE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_1_LINE;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction         = instruction;
    cmd.DummyCycles         = dummyCycles;
    cmd.Address             = address;
    cmd.AddressSize         = addressSize;
    cmd.NbData              = length;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);
    bool timeout = (status != HAL_OK);
    if (!timeout) {
        status = HAL_QSPI_Receive(hqspi, in, QUADSPI_DEFAULT_TIMEOUT);
        timeout = (status != HAL_OK);
    }

    if (timeout) {
        return false;
    }

    return true;
}

bool QuadSpiReceiveWithAddress4Lines(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint8_t dummyCycles, uint32_t address, uint8_t addressSize, uint8_t *in, int length)
{
    HAL_StatusTypeDef status;

    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_4_LINES;
    cmd.AddressMode         = QSPI_ADDRESS_4_LINES;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_4_LINES;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction         = instruction;
    cmd.DummyCycles         = dummyCycles;
    cmd.Address             = address;
    cmd.AddressSize         = addressSize;
    cmd.NbData              = length;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);
    bool timeout = (status != HAL_OK);
    if (!timeout) {
        status = HAL_QSPI_Receive(hqspi, in, QUADSPI_DEFAULT_TIMEOUT);
        timeout = (status != HAL_OK);
    }

    if (timeout) {
        return false;
    }

    return true;
}

bool QuadSpiTransmit1Line(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint8_t dummyCycles, const uint8_t *out, uint16_t length)
{
    HAL_StatusTypeDef status;


    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_NONE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_NONE;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    cmd.DummyCycles         = dummyCycles;
    cmd.Instruction         = instruction;
    cmd.NbData              = length;

    if (out) {
        cmd.DataMode        = QSPI_DATA_1_LINE;
    }

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);
    bool timeout = (status != HAL_OK);
    if (!timeout) {
        if (out && length > 0) {
            status = HAL_QSPI_Transmit(hqspi, (uint8_t *)out, QUADSPI_DEFAULT_TIMEOUT);
            timeout = (status != HAL_OK);
        }
    }

    if (timeout) {
        return false;
    }

    return true;
}

bool QuadSpiTransmitWithAddress1Line(
        QSPI_HandleTypeDef *hqspi,
        uint8_t instruction,
        uint8_t dummyCycles,
        uint32_t address,
        uint32_t addressSize,
        const uint8_t *out,
        int length
        )
{
    HAL_StatusTypeDef status;

    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_1_LINE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_1_LINE;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction         = instruction;
    cmd.DummyCycles         = dummyCycles;
    cmd.Address             = address;
    cmd.AddressSize         = addressSize;
    cmd.NbData              = length;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);
    bool timeout = (status != HAL_OK);

    if (!timeout) {
        status = HAL_QSPI_Transmit(hqspi, (uint8_t *)out, QUADSPI_DEFAULT_TIMEOUT);
        timeout = (status != HAL_OK);
    }

    if (timeout) {
        return false;
    }

    return true;
}

bool QuadSpiTransmitWithAddress4Line(
        QSPI_HandleTypeDef *hqspi,
        uint8_t instruction,
        uint8_t dummyCycles,
        uint32_t address,
        uint32_t addressSize,
        const uint8_t *out,
        int length
        )
{
    HAL_StatusTypeDef status;

    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_1_LINE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_4_LINES;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    cmd.Instruction         = instruction;
    cmd.DummyCycles         = dummyCycles;
    cmd.Address             = address;
    cmd.AddressSize         = addressSize;
    cmd.NbData              = length;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);
    bool timeout = (status != HAL_OK);

    if (!timeout) {
        status = HAL_QSPI_Transmit(hqspi, (uint8_t *)out, QUADSPI_DEFAULT_TIMEOUT);
        timeout = (status != HAL_OK);
    }

    if (timeout) {
        return false;
    }

    return true;
}

bool QuadSpiInstructionWithAddress1LINE(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint8_t dummyCycles, uint32_t address, uint8_t addressSize)
{
    HAL_StatusTypeDef status;

    QSPI_CommandTypeDef cmd;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AddressMode         = QSPI_ADDRESS_1_LINE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_NONE;
    cmd.DummyCycles         = dummyCycles;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_ONLY_FIRST_CMD;

    cmd.Instruction         = instruction;
    cmd.Address             = address;
    cmd.AddressSize         = QSPI_ADDRESS_24_BITS;
    cmd.NbData              = 0;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);
    bool timeout = (status != HAL_OK);


    if (timeout) {
        return false;
    }

    return true;
}

bool QuadSpiReceiveWithAddress4LINES(
        QSPI_HandleTypeDef *hqspi,
        uint8_t instruction,
        uint8_t dummyCycles,
        uint32_t address,
        uint32_t addressSize,
        uint8_t *in,
        int length)
{
    HAL_StatusTypeDef status;
    QSPI_CommandTypeDef cmd;

    cmd.AddressMode         = QSPI_ADDRESS_4_LINES;
    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_4_LINES;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.SIOOMode            = QSPI_SIOO_INST_ONLY_FIRST_CMD;

    cmd.Instruction         = instruction;
    cmd.DummyCycles         = dummyCycles;
    cmd.Address             = address;
    cmd.AddressSize         = addressSize;
    cmd.NbData              = length;

    status = HAL_QSPI_Command(hqspi, &cmd, QUADSPI_DEFAULT_TIMEOUT);
    bool timeout = (status != HAL_OK);
    if (!timeout) {
        status = HAL_QSPI_Receive(hqspi, in, QUADSPI_DEFAULT_TIMEOUT);
        timeout = (status != HAL_OK);
    }

    if (timeout) {
        return false;
    }

    return true;
}

