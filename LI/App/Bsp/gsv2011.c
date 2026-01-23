/**
  ******************************************************************************
  * @file    gsv2011.c
  * @brief   GSV2011 HDMI2.0 Repeater Driver Implementation
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-01
  ******************************************************************************
  */

#include "gsv2011.h"
#include "hal_i2c.h"

extern I2C_HandleTypeDef hi2c1;

/* From scan result: GSV2011 candidate addr7 = 0x58 */
#define GSV2011_ADDR_7BIT  0x58

HAL_StatusTypeDef gsv2011_read8(uint16_t reg16, uint8_t *val)
{
  return hal_i2c_mem_read(GSV2011_ADDR_7BIT, reg16, I2C_MEMADD_SIZE_16BIT, val, 1, 200);
}

HAL_StatusTypeDef gsv2011_write8(uint16_t reg16, uint8_t val)
{
  return hal_i2c_mem_write(GSV2011_ADDR_7BIT, reg16, I2C_MEMADD_SIZE_16BIT, &val, 1, 200);
}

void gsv2011_init(void)
{
  hal_i2c_bind(&hi2c1);

  printf("GSV2011 I2C test start... addr7=0x%02X\r\n", (unsigned)GSV2011_ADDR_7BIT);

  if (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(GSV2011_ADDR_7BIT << 1), 3, 100) != HAL_OK)
  {
    printf("GSV2011 I2C NACK/NOT READY\r\n");
    return;
  }

  /* Verified protocol: 16-bit reg address + repeated-start (Mem_Read 16bit) */
  for (uint16_t reg = 0x0000; reg <= 0x0006; reg++)
  {
    uint8_t v = 0x00;
    HAL_StatusTypeDef ret = gsv2011_read8(reg, &v);
    if (ret == HAL_OK)
    {
      printf("GSV2011 reg[0x%04X]=0x%02X\r\n", (unsigned)reg, v);
    }
    else
    {
      printf("GSV2011 read reg[0x%04X] FAIL ret=%d\r\n", (unsigned)reg, (int)ret);
      break;
    }
  }

  printf("GSV2011 I2C test done.\r\n");
}


