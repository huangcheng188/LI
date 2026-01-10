/**
  ******************************************************************************
  * @file    key.h
  * @brief   Key Driver Implementation
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-01
  ******************************************************************************
  */

#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define LED_GPIO_PORT GPIOB
#define LED1_PIN GPIO_PIN_14
#define LED2_PIN GPIO_PIN_15

typedef enum
{
    LED_1 = 0,
    LED_2,
    LED_NUM_MAX
} led_num_t;


void led_init(void);
void led_on(led_num_t led_num);
void led_off(led_num_t led_num);

#endif

