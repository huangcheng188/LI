/**
  ******************************************************************************
  * @file    key.c
  * @brief   Key Driver Implementation
  * @author  huangcheng
  * @version 1.0.0
  * @date    2026-01-01
  ******************************************************************************
  */

#include "key.h"

/* 按键按下为高电平（PB12/PB13 下拉输入） */
#define KEY_PRESSED_LEVEL GPIO_PIN_SET
#define KEY_DEBOUNCE_MS   20U

static uint16_t key_pin(key_num_t key)
{
  switch (key) {
    case KEY_1: return GPIO_PIN_12;
    case KEY_2: return GPIO_PIN_13;
    default:    return GPIO_PIN_12;
  }
}

void key_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

uint8_t key_is_pressed(key_num_t key)
{
  GPIO_PinState s = HAL_GPIO_ReadPin(GPIOB, key_pin(key));
  return (s == KEY_PRESSED_LEVEL) ? 1u : 0u;
}

uint8_t key_press_release_event(key_num_t key)
{
  /* 去抖后的稳定电平（0=松开, 1=按下） */
  static uint8_t stable[KEY_NUM_MAX] = {0};
  static uint8_t last_raw[KEY_NUM_MAX] = {0};
  static uint32_t last_change_ms[KEY_NUM_MAX] = {0};
  static uint8_t pressed_seen[KEY_NUM_MAX] = {0};

  const uint8_t raw = key_is_pressed(key);
  const uint32_t now = HAL_GetTick();

  if (raw != last_raw[key]) {
    last_raw[key] = raw;
    last_change_ms[key] = now;
  }

  /* 电平保持稳定超过去抖时间，才更新 stable */
  if ((now - last_change_ms[key]) >= KEY_DEBOUNCE_MS && stable[key] != raw) {
    stable[key] = raw;

    if (stable[key]) {
      /* 稳定按下 */
      pressed_seen[key] = 1u;
    } else {
      /* 稳定松开：如果之前经历过按下，则认为一次“按下->抬起”完成 */
      if (pressed_seen[key]) {
        pressed_seen[key] = 0u;
        return 1u;
      }
    }
  }

  return 0u;
}