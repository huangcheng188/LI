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
#include <stdint.h>

typedef enum
{
  KEY_1 = 0,
  KEY_2,
  KEY_NUM_MAX
} key_num_t;

void key_init(void);
uint8_t key_is_pressed(key_num_t key);

/**
 * @brief 按下并抬起触发一次（按键按下为高电平）
 * @note  需要周期性调用（建议 1~10ms 调用一次），返回 1 表示本次检测到一次“按下->抬起”事件
 */
uint8_t key_press_release_event(key_num_t key);

#endif

