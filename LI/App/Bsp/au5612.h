/**
  ******************************************************************************
  * @file    au5612.h
  * @brief   AU5612 Clock Chip Driver Implementation
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-01
  ******************************************************************************
  */

#ifndef __AU5612_H
#define __AU5612_H

#include "main.h"
#include "stm32f0xx_hal_i2c.h"
#include <string.h>
#include <stdint.h>

typedef enum{
  AU5612_PLL_LOCK = 0,
  AU5612_PLL_UNLOCK,
  AU5612_PLL_MAX
}au5612_pll_lock_t;

void au5612_init(void);
void au5612_apply_generated_config(void);
void au5612_pll_lock(au5612_pll_lock_t pll_lock);
au5612_pll_lock_t au5612_get_pll_lock(void);

/**
 * @brief Read CLKMON PLL lock status from STATUS_1_CLKMON.
 * @return 1 if locked, 0 if not locked, 0xFF on I2C error.
 *
 * From AU5612 RegMap:
 * - Page select: 0xFF = 0x06 (CLKMON page)
 * - STATUS_1_CLKMON: reg 0x06
 *   - bit0: 0 = locked, 1 = not locked
 */
uint8_t au5612_clkmon_is_locked(void);

#endif

