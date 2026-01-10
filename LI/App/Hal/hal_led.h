/**
  ******************************************************************************
  * @file    hal_led.h
  * @brief   HAL LED Driver Implementation
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-01
  ******************************************************************************
  */

#ifndef __HAL_LED_H
#define __HAL_LED_H

#include "main.h"

typedef enum
{
  HAL_LED_1 = 0,
  HAL_LED_2,
  HAL_LED_NUM_MAX
} hal_led_num_t;

typedef struct
{
  void (*led_init)(void);
  void (*led_on)(hal_led_num_t led_num);
  void (*led_off)(hal_led_num_t led_num);
} hal_led_t;

void hal_led_init(void);
void hal_led_on(hal_led_num_t hal_led_num);
void hal_led_off(hal_led_num_t hal_led_num);

#endif
