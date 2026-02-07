#include "av_key_cmd.h"
#include "av_config.h"
#include "hal.h"
#include "kapi.h"

/* Board LED mapping (2 physical LEDs -> SDK expects 3)
 * - LED0 -> PB14
 * - LED1 -> PB15
 * - LED2 -> (LED0|LED1) 组合灯：同时两灭/两亮
 */
#ifndef LED_GPIO_PORT
#define LED_GPIO_PORT GPIOB
#endif
#ifndef LED0_Pin
#define LED0_Pin GPIO_PIN_14
#endif
#ifndef LED1_Pin
#define LED1_Pin GPIO_PIN_15
#endif
#ifndef LED2_Pin
#define LED2_Pin (GPIO_PIN_14 | GPIO_PIN_15)
#endif

extern uint8  EdidHdmi2p0;
extern uint8  LogicOutputSel;

#if AvEnableKeyInput /* Enable UART */

extern uchar AudioStatus;

void ProcessKey(AvPort *port);
#define FoundKeyPress(p) uChar&p

#endif /* Enable UART */

void ProcessKey(AvPort *port)
{
#if AvEnableKeyInput
    uint8 value = 0x00;
    uint8 uChar = 0x00;
    if(AvHalGetKey(&uChar) == AvNotAvailable)
        return;

    /* Logic Video/Audio Direction */
    value = (uChar>>0) & 0x01;
    if(value != 0x00)
    {
        LogicOutputSel = 1 - LogicOutputSel;
    }
    /* Logic LED */
    LogicLedOut(LogicOutputSel);
#endif
}

void ListenToKeyCommand(AvPort *port)
{
#if AvEnableKeyInput
    ProcessKey(port);
#endif
}

void RxInLedOut(uint8 enable)
{
    if(enable == 1)
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED0_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED0_Pin, GPIO_PIN_SET);
}

void TxOutLedOut(uint8 index, uint8 enable)
{
    (void)index;
    if(enable == 1)
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_Pin, GPIO_PIN_SET);
}

void LogicLedOut(uint8 enable)
{
    if(enable == 1)
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_Pin, GPIO_PIN_SET);
}
