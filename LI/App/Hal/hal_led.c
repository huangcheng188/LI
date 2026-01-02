/**
  ******************************************************************************
  * @file    hal_led.c
  * @brief   HAL LED Driver Implementation
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-01
  ******************************************************************************
  */

#include "hal_led.h"
#include "led.h"

static hal_led_t hal_led = {
  .led_init = led_init,
  .led_on = led_on,
  .led_off = led_off,
};

void hal_led_init(void)
{
  hal_led.led_init();
}

void hal_led_on(hal_led_num_t led_num)
{
  hal_led.led_on(led_num);
}

void hal_led_off(hal_led_num_t led_num)
{
  hal_led.led_off(led_num);
}


