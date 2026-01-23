/**
  ******************************************************************************
  * @file    hal_i2c.h
  * @brief   HAL I2C wrapper (simple read/write + mem/reg access)
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-11
  ******************************************************************************
  */

#ifndef __HAL_I2C_H
#define __HAL_I2C_H

#include "main.h"
#include <stdint.h>
#include <stddef.h>

/* Default timeout (ms) for blocking I2C transactions */
#ifndef HAL_I2C_TIMEOUT_MS
#define HAL_I2C_TIMEOUT_MS 100U
#endif

/**
 * @brief Bind an initialized HAL I2C handle (e.g. &hi2c1 after MX_I2C1_Init()).
 */
void hal_i2c_bind(I2C_HandleTypeDef *hi2c);

/**
 * @brief Raw master transmit/receive (7-bit device address).
 * @note dev_addr_7bit is the 7-bit address (e.g. 0x50), NOT left-shifted.
 */
HAL_StatusTypeDef hal_i2c_write(uint16_t dev_addr_7bit,
                               const uint8_t *data,
                               uint16_t len,
                               uint32_t timeout_ms);

HAL_StatusTypeDef hal_i2c_read(uint16_t dev_addr_7bit,
                              uint8_t *data,
                              uint16_t len,
                              uint32_t timeout_ms);

/**
 * @brief Memory/register access helpers (HAL_I2C_Mem_*).
 * @note mem_addr_size should be I2C_MEMADD_SIZE_8BIT or I2C_MEMADD_SIZE_16BIT.
 */
HAL_StatusTypeDef hal_i2c_mem_write(uint16_t dev_addr_7bit,
                                   uint16_t mem_addr,
                                   uint16_t mem_addr_size,
                                   const uint8_t *data,
                                   uint16_t len,
                                   uint32_t timeout_ms);

HAL_StatusTypeDef hal_i2c_mem_read(uint16_t dev_addr_7bit,
                                  uint16_t mem_addr,
                                  uint16_t mem_addr_size,
                                  uint8_t *data,
                                  uint16_t len,
                                  uint32_t timeout_ms);

/* Common 8-bit register helpers */
HAL_StatusTypeDef hal_i2c_reg_write8(uint16_t dev_addr_7bit,
                                    uint8_t reg,
                                    uint8_t value,
                                    uint32_t timeout_ms);

HAL_StatusTypeDef hal_i2c_reg_read8(uint16_t dev_addr_7bit,
                                   uint8_t reg,
                                   uint8_t *value,
                                   uint32_t timeout_ms);

HAL_StatusTypeDef hal_i2c_reg_write(uint16_t dev_addr_7bit,
                                   uint8_t reg,
                                   const uint8_t *data,
                                   uint16_t len,
                                   uint32_t timeout_ms);

HAL_StatusTypeDef hal_i2c_reg_read(uint16_t dev_addr_7bit,
                                  uint8_t reg,
                                  uint8_t *data,
                                  uint16_t len,
                                  uint32_t timeout_ms);

#endif


