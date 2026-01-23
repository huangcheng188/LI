/**
  ******************************************************************************
  * @file    hal_i2c.c
  * @brief   HAL I2C wrapper (simple read/write + mem/reg access)
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-11
  ******************************************************************************
  */

#include "hal_i2c.h"

static I2C_HandleTypeDef *s_hi2c = NULL;

static inline uint16_t i2c_addr8(uint16_t dev_addr_7bit)
{
  return (uint16_t)(dev_addr_7bit << 1);
}

void hal_i2c_bind(I2C_HandleTypeDef *hi2c)
{
  s_hi2c = hi2c;
}

HAL_StatusTypeDef hal_i2c_write(uint16_t dev_addr_7bit,
                               const uint8_t *data,
                               uint16_t len,
                               uint32_t timeout_ms)
{
  if (s_hi2c == NULL) {
    return HAL_ERROR;
  }
  return HAL_I2C_Master_Transmit(s_hi2c, i2c_addr8(dev_addr_7bit), (uint8_t *)data, len, timeout_ms);
}

HAL_StatusTypeDef hal_i2c_read(uint16_t dev_addr_7bit,
                              uint8_t *data,
                              uint16_t len,
                              uint32_t timeout_ms)
{
  if (s_hi2c == NULL) {
    return HAL_ERROR;
  }
  return HAL_I2C_Master_Receive(s_hi2c, i2c_addr8(dev_addr_7bit), data, len, timeout_ms);
}

HAL_StatusTypeDef hal_i2c_mem_write(uint16_t dev_addr_7bit,
                                   uint16_t mem_addr,
                                   uint16_t mem_addr_size,
                                   const uint8_t *data,
                                   uint16_t len,
                                   uint32_t timeout_ms)
{
  if (s_hi2c == NULL) {
    return HAL_ERROR;
  }
  return HAL_I2C_Mem_Write(s_hi2c, i2c_addr8(dev_addr_7bit), mem_addr, mem_addr_size, (uint8_t *)data, len, timeout_ms);
}

HAL_StatusTypeDef hal_i2c_mem_read(uint16_t dev_addr_7bit,
                                  uint16_t mem_addr,
                                  uint16_t mem_addr_size,
                                  uint8_t *data,
                                  uint16_t len,
                                  uint32_t timeout_ms)
{
  if (s_hi2c == NULL) {
    return HAL_ERROR;
  }
  return HAL_I2C_Mem_Read(s_hi2c, i2c_addr8(dev_addr_7bit), mem_addr, mem_addr_size, data, len, timeout_ms);
}

HAL_StatusTypeDef hal_i2c_reg_write8(uint16_t dev_addr_7bit,
                                    uint8_t reg,
                                    uint8_t value,
                                    uint32_t timeout_ms)
{
  return hal_i2c_reg_write(dev_addr_7bit, reg, &value, 1, timeout_ms);
}

HAL_StatusTypeDef hal_i2c_reg_read8(uint16_t dev_addr_7bit,
                                   uint8_t reg,
                                   uint8_t *value,
                                   uint32_t timeout_ms)
{
  return hal_i2c_reg_read(dev_addr_7bit, reg, value, 1, timeout_ms);
}

HAL_StatusTypeDef hal_i2c_reg_write(uint16_t dev_addr_7bit,
                                   uint8_t reg,
                                   const uint8_t *data,
                                   uint16_t len,
                                   uint32_t timeout_ms)
{
  if (s_hi2c == NULL) {
    return HAL_ERROR;
  }
  if (data == NULL || len == 0) {
    return HAL_OK;
  }

  /* Common device style: [REG][DATA0..] in one transfer */
  if (len <= 32U) {
    uint8_t buf[33];
    buf[0] = reg;
    for (uint16_t i = 0; i < len; i++) {
      buf[1U + i] = data[i];
    }
    return HAL_I2C_Master_Transmit(s_hi2c, i2c_addr8(dev_addr_7bit), buf, (uint16_t)(len + 1U), timeout_ms);
  }

  /* Fallback for long writes */
  return hal_i2c_mem_write(dev_addr_7bit, reg, I2C_MEMADD_SIZE_8BIT, data, len, timeout_ms);
}

HAL_StatusTypeDef hal_i2c_reg_read(uint16_t dev_addr_7bit,
                                  uint8_t reg,
                                  uint8_t *data,
                                  uint16_t len,
                                  uint32_t timeout_ms)
{
  if (s_hi2c == NULL) {
    return HAL_ERROR;
  }
  if (data == NULL || len == 0) {
    return HAL_OK;
  }

  /* Common device style: write reg then read */
  HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(s_hi2c, i2c_addr8(dev_addr_7bit), &reg, 1, timeout_ms);
  if (ret != HAL_OK) {
    return ret;
  }
  return HAL_I2C_Master_Receive(s_hi2c, i2c_addr8(dev_addr_7bit), data, len, timeout_ms);
}


