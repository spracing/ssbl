/*
 * This program is driver for Winbond W25Q128x Serial flash memory.
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

#include "drivers/flash_w25q128.h"
#include "drivers/quadspi.h"

#define W25Q_LINEAR_TO_PAGE(laddr) ((laddr) & 0xFFFFFF)
#define W25Q_LINEAR_TO_BLOCK(laddr) (W25Q_LINEAR_TO_PAGE(laddr) / W25Q_PAGES_PER_BLOCK)
#define W25Q_BLOCK_TO_PAGE(block) ((block) * W25Q_PAGES_PER_BLOCK)
#define W25Q_BLOCK_TO_LINEAR(block) (W25Q_BLOCK_TO_PAGE(block) * W25Q_PAGE_SIZE)

void W25q_readJedec(QSPI_HandleTypeDef *hqspi, uint8_t* idBuffer) {
    QuadSpiReceive1Line(hqspi, W25Q_INSTR_JEDEC_ID, 0, idBuffer, 3);
}

bool W25q_writeEnable(QSPI_HandleTypeDef *hqspi)
{
    uint8_t statusReg;

    W25q_waitForReady(hqspi);
    bool success = false;

    success = QuadSpiInstruction(hqspi, W25Q_INSTR_WRITE_ENABLE);

    if(success) {
        W25q_readStatusRegister(hqspi, W25Q_INSTR_READ_STATUS_REG1, &statusReg);

        if(!(statusReg & W25Q_STATUS_REG1_WEL)) {
            success = false;
        }
    }

    return success;
}

bool W25q_quadEnable(QSPI_HandleTypeDef *hqspi) {

    bool success = false;
    uint8_t dummyCycles = 0;
    uint16_t length = 1u;
    uint8_t statusRegister;

    success = W25q_writeEnable(hqspi);

    if (success) {
        statusRegister = W25Q_STATUS_REG2_QE;
        success = QuadSpiTransmit1Line(hqspi, W25Q_INSTR_WRITE_STATUS_REG2,  dummyCycles, &statusRegister, length);
    }

    if(success) {
        success = W25q_readStatusRegister(hqspi, W25Q_INSTR_READ_STATUS_REG2, &statusRegister);
    }

    if(success && (statusRegister & W25Q_STATUS_REG2_QE)) {
        success = true;
    } else {
        success = false;
    }

    return success;
}

bool W25q_readStatusRegister(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint8_t* statusRegister)
{
    bool success = false;
    uint16_t length = 1u;

    success = QuadSpiReceive1Line(hqspi, instruction, W25Q_ZERO_DUMMY_CYCLES, statusRegister, length);

    return success;
}

bool W25q_writeStatusRegister(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint8_t statusRegister)
{
    bool success = false;
    uint16_t length = 1u;

    success = W25q_writeEnable(hqspi);
    if (success) {
        success = QuadSpiTransmit1Line(
                hqspi,
                instruction,
                W25Q_ZERO_DUMMY_CYCLES,
                &statusRegister,
                length
                );
    }

    return success;
}

void W25q_waitForReady(QSPI_HandleTypeDef *hqspi)
{
    uint8_t statusReg;
    W25q_readStatusRegister(hqspi, W25Q_INSTR_READ_STATUS_REG1, &statusReg);

    while (statusReg & W25Q_STATUS_REG1_BUSY) {
        W25q_readStatusRegister(hqspi, W25Q_INSTR_READ_STATUS_REG1, &statusReg);
    }
}

bool W25q_readBytes(QSPI_HandleTypeDef *hqspi, uint32_t address, uint8_t *buffer, uint32_t length)
{
    bool success = false;
    uint32_t pageAddress = W25Q_LINEAR_TO_PAGE(address);

    W25q_waitForReady(hqspi);

    success = QuadSpiReceiveWithAddress4LINES(
            hqspi,
            W25Q_INSTR_FAST_READ_QUAD,
            W25Q_DUMMY_CYCLES_FAST_READ_QUAD,
            pageAddress,
            QSPI_ADDRESS_24_BITS,
            buffer,
            length
            );

    return success;
}

bool W25q_sectorErase(QSPI_HandleTypeDef *hqspi, uint32_t address)
{
    bool success = false;

    uint32_t pageAddress = W25Q_LINEAR_TO_PAGE(address);

    success = W25q_writeEnable(hqspi);
    W25q_waitForReady(hqspi);

    if(success) {
        success = QuadSpiInstructionWithAddress(
                hqspi,
                W25Q_INSTR_SECTOR_ERASE,
                pageAddress,
                QSPI_ADDRESS_24_BITS
                );
    }

    return success;
}


bool W25q_blockErase32k(QSPI_HandleTypeDef *hqspi, uint32_t address)
{
    bool success = false;

    uint32_t pageAddress = W25Q_LINEAR_TO_PAGE(address);

    success = W25q_writeEnable(hqspi);
    W25q_waitForReady(hqspi);

    if(success) {
        success = QuadSpiInstructionWithAddress(
                hqspi,
                W25Q_INSTR_32K_BLOCK_ERASE,
                pageAddress,
                QSPI_ADDRESS_24_BITS
                );
    }

    return success;
}

bool W25q_blockErase64k(QSPI_HandleTypeDef *hqspi, uint32_t address)
{
    bool success = false;

    uint32_t pageAddress = W25Q_LINEAR_TO_PAGE(address);

    success = W25q_writeEnable(hqspi);
    W25q_waitForReady(hqspi);

    if(success) {
        success = QuadSpiInstructionWithAddress(
                hqspi,
                W25Q_INSTR_64K_BLOCK_ERASE,
                pageAddress,
                QSPI_ADDRESS_24_BITS
                );
    }

    return success;
}

bool W25q_chipErase(QSPI_HandleTypeDef *hqspi)
{
    bool success = false;

    success = W25q_writeEnable(hqspi);
    W25q_waitForReady(hqspi);

    if(success){
        success = QuadSpiInstruction(hqspi, W25Q_CHIP_ERASE);
    }

    return success;
}

bool W25q_quadPageProgram(QSPI_HandleTypeDef *hqspi, uint32_t address, uint8_t *buffer, uint32_t length)
{
    bool success = false;

    if(length <= W25Q_PAGE_SIZE) {

        uint32_t pageAddress = W25Q_LINEAR_TO_PAGE(address);

        success = W25q_writeEnable(hqspi);

        W25q_waitForReady(hqspi);

        if(success) {
            success = QuadSpiTransmitWithAddress4Line(
                    hqspi,
                    W25_INSTR_QUAD_INPUT_PAGE_PROGRAM,
                    W25Q_ZERO_DUMMY_CYCLES,
                    pageAddress,
                    QSPI_ADDRESS_24_BITS,
                    buffer,
                    length
                    );
        }

    } else {
        success = false;
    }

    return success;
}

bool W25q_memoryMappedModeEnable(QSPI_HandleTypeDef *hqspi)
{
    bool success = true;
    QSPI_CommandTypeDef cmd;
    QSPI_MemoryMappedTypeDef memMappedCfg;

    cmd.InstructionMode     = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction         = W25Q_INSTR_FAST_READ_QUAD;
    cmd.AlternateByteMode   = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode            = QSPI_DATA_4_LINES;
    cmd.DdrMode             = QSPI_DDR_MODE_DISABLE;
    cmd.DdrHoldHalfCycle    = QSPI_DDR_HHC_ANALOG_DELAY;
    cmd.AddressMode         = QSPI_ADDRESS_4_LINES;
    cmd.AddressSize         = QSPI_ADDRESS_24_BITS;
    cmd.DummyCycles         = W25Q_DUMMY_CYCLES_FAST_READ_QUAD;
    cmd.SIOOMode            = QSPI_SIOO_INST_EVERY_CMD;

    memMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    memMappedCfg.TimeOutPeriod = 0;

    W25q_waitForReady(hqspi);

    if (HAL_QSPI_MemoryMapped(hqspi, &cmd, &memMappedCfg) != HAL_OK)
    {
        success = false;
    }
    return success;
}
