/**
  ******************************************************************************
  * @file    gsv2011.h
  * @brief   GSV2011 HDMI2.0 Repeater Driver Implementation
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-01
  ******************************************************************************
  */

#ifndef __GSV2011_H
#define __GSV2011_H

#include "main.h"
#include "stm32f0xx_hal_i2c.h"
#include <string.h>

void gsv2011_init(void);

/* GSV2011 uses 16-bit register addressing (verified) */
HAL_StatusTypeDef gsv2011_read8(uint16_t reg16, uint8_t *val);
HAL_StatusTypeDef gsv2011_write8(uint16_t reg16, uint8_t val);

#endif
