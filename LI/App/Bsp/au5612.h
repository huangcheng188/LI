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

typedef enum{
  AU5612_PLL_LOCK = 0,
  AU5612_PLL_UNLOCK,
  AU5612_PLL_MAX
}au5612_pll_lock_t;

void au5612_init(void);
void au5612_pll_lock(au5612_pll_lock_t pll_lock);
au5612_pll_lock_t au5612_get_pll_lock(void);

#endif

