/**
  ******************************************************************************
  * @file    led.c
  * @brief   LED Driver Implementation
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-01
  ******************************************************************************
  */

#include "led.h"

void led_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN|LED2_PIN, GPIO_PIN_SET);

  /*Configure GPIO pins : PB14 PB15 */
  GPIO_InitStruct.Pin = LED1_PIN|LED2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

void led_on(led_num_t led_num)
{
  switch (led_num)
  {
    case LED_1:
      HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
      break;
    case LED_2:
      HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
      break;
  }
}

void led_off(led_num_t led_num)
{
  switch (led_num)
  {
    case LED_1:
      HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
      break;
    case LED_2:
      HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
      break;
  }
}
