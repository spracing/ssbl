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

#ifndef __W25Q_H
#define __W25Q_H

#include <stdbool.h>
#include <stdint.h>

#include "stm32h7xx_hal.h"

#define W25Q_MANUFACTURER_ID    0xEF //!< MF7 - MF0
#define W25Q_DEVICE_ID_1_IQ     0x40 //!< W25Q128JV-IM - first part of ID15 - ID0 (4018h)
#define W25Q_DEVICE_ID_1_IM     0x70 //!< W25Q128JV-IQ - first part of ID15 - ID0 (7018h)
#define W25Q_DEVICE_ID_2        0x18 //!< W25Q128JV-Ix - second part of ID15 - ID0 (4018h or 7018h)

// Device size parameters
#define W25Q_PAGE_SIZE          256         //!< Bytes
#define W25Q_SECTOR_SIZE        4096        //!< Bytes (4KB)
#define W25Q_BLOCK_SIZE         65536       //!< Bytes (64KB)
#define W25Q_CHIP_SIZE          16777216    //!< Bytes (16MB)
#define W25Q_PAGES_PER_SECTOR   16
#define W25Q_PAGES_PER_BLOCK    256

#define W25Q_INSTR_DEVICE_RESET                     0xFF
#define W25Q_INSTR_JEDEC_ID                         0x9F
#define W25Q_INSTR_WRITE_ENABLE                     0x06
#define W25Q_INSTR_WRITE_DISABLE                    0x04
#define W25Q_INSTR_SECTOR_ERASE                     0x20
#define W25Q_INSTR_32K_BLOCK_ERASE                  0x52
#define W25Q_INSTR_64K_BLOCK_ERASE                  0xD8
#define W25Q_CHIP_ERASE                             0xC7
#define W25Q_INSTR_FAST_READ_QUAD                   0xEB
#define W25_INSTR_PAGE_PROGRAM                      0x02
#define W25_INSTR_QUAD_INPUT_PAGE_PROGRAM           0x32

#define W25Q_ZERO_DUMMY_CYCLES                      0
#define W25Q_DUMMY_CYCLES_FAST_READ_QUAD            6

#define W25Q_DUMMY_BITS_FAST_READ_QUAD_BUFFER       4
#define W25Q_DUMMY_BITS_FAST_READ_QUAD_CONT         12

// Config/status register instructions
#define W25Q_INSTR_READ_STATUS_REG1     0x05
#define W25Q_INSTR_READ_STATUS_REG2     0x35
#define W25Q_INSTR_READ_STATUS_REG3     0x15
#define W25Q_INSTR_WRITE_STATUS_REG1    0x01
#define W25Q_INSTR_WRITE_STATUS_REG2    0x31
#define W25Q_INSTR_WRITE_STATUS_REG3    0x11

#define W25Q_STATUS_REG_CLEAR_ALL       0

// Bits in config/status register 1
#define W25Q_STATUS_REG1_SRP0           (1 << 7)    //!< Status Register Protect 0 (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG1_SEC            (1 << 6)    //!< Sector Protect Bit (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG1_TB             (1 << 5)    //!< Top/Bottom Protect Bit (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG1_BP2            (1 << 4)    //!< Block Protect Bit 2 (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG1_BP1            (1 << 3)    //!< Block Protect Bit 1 (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG1_BP0            (1 << 2)    //!< Block Protect Bit 0 (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG1_WEL            (1 << 1)    //!< Write Enable Latch (Status-Only)
#define W25Q_STATUS_REG1_BUSY           (1 << 0)    //!< Erase/Write In Progress (Status-Only)

// Bits in config/status register 2
#define W25Q_STATUS_REG2_SUS            (1 << 7)    //!< Suspend Status (Status-Only)
#define W25Q_STATUS_REG2_CMP            (1 << 6)    //!< Complement Protect (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG2_LB3            (1 << 5)    //!< Security Register Lock Bit 3 (Volatile/Non-Volatile OTP Writable)
#define W25Q_STATUS_REG2_LB2            (1 << 4)    //!< Security Register Lock Bit 2 (Volatile/Non-Volatile OTP Writable)
#define W25Q_STATUS_REG2_LB1            (1 << 3)    //!< Security Register Lock Bit 1 (Volatile/Non-Volatile OTP Writable)
#define W25Q_STATUS_REG2_QE             (1 << 1)    //!< Quad Enable (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG2_SRP1           (1 << 0)    //!< Status Register Protect 1 (Volatile/Non-Volatile Writable)

// Bits in config/status register 3
#define W25Q_STATUS_REG3_HOLD_RST       (1 << 7)    //!< /HOLD or /RESET Function  (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG3_DRV1           (1 << 6)    //!< Output driver strength 1 (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG3_DRV2           (1 << 5)    //!< Output driver strength 2 (Volatile/Non-Volatile Writable)
#define W25Q_STATUS_REG3_WPS            (1 << 2)    //!< Write Protect Selection (Volatile/Non-Volatile Writable)

void W25q_readJedec(QSPI_HandleTypeDef *hqspi, uint8_t* idBuffer);
bool W25q_writeEnable(QSPI_HandleTypeDef *hqspi);
bool W25q_quadEnable(QSPI_HandleTypeDef *hqspi);
bool W25q_readStatusRegister(QSPI_HandleTypeDef *hqspi, uint8_t instruction, uint8_t* statusRegister);
bool W25q_writeStatusRegister(QSPI_HandleTypeDef *hqspi, uint8_t reg, uint8_t data);
void W25q_waitForReady(QSPI_HandleTypeDef *hqspi);
bool W25q_readBytes(QSPI_HandleTypeDef *hqspi, uint32_t address, uint8_t *buffer, uint32_t length);
bool W25q_sectorErase(QSPI_HandleTypeDef *hqspi, uint32_t address);
bool W25q_blockErase32k(QSPI_HandleTypeDef *hqspi, uint32_t address);
bool W25q_blockErase64k(QSPI_HandleTypeDef *hqspi, uint32_t address);
bool W25q_chipErase(QSPI_HandleTypeDef *hqspi);
bool W25q_quadPageProgram(QSPI_HandleTypeDef *hqspi, uint32_t address, uint8_t *buffer, uint32_t length);
bool W25q_memoryMappedModeEnable(QSPI_HandleTypeDef *hqspi);

#endif /* __W25Q_H */
