/**
 * @file bsp.c
 *
 * @brief sample bsp support
 */
#include "bsp.h"

#include "hal_i2c.h"

/* Use your board's hardware I2C (already debugged elsewhere) */
extern I2C_HandleTypeDef hi2c1;

extern uint8 LogicOutputSel;
extern UART_HandleTypeDef huart1;
#if AvEnableKeyInput
static uint8 KeyValue;
static uint8 KeyDelay;
#endif

AvRet ManI2cWrite(uint32 devAddress, uint32 regAddress, uint8 *i2cdata, uint16 count, uint8 index, uint8 Flag16bit);
AvRet ManI2cRead(uint32 devAddress, uint32 regAddress, uint8 *i2cdata, uint16 count, uint8 index, uint8 Flag16bit);

/**
 * @brief  init bsp
 * @return AvOk - success
 */
AvRet BspInit(void)
{
    AvRet ret = AvOk;
    /* Bind HAL I2C handle for BSP I2C access */
    hal_i2c_bind(&hi2c1);
    return ret;
}

/**
 * @brief  bsp i2c read function, support bus/dev address, 8/16 register address
 * @return AvOk - success
 */
AvRet BspI2cRead(uint32 devAddress, uint32 regAddress, uint8 *data, uint16 count)
{
    AvRet ret = AvOk;
    uint8 deviceAddress = (uint8)AvGetI2cDeviceAddress(devAddress);
    uint8 busAddress = (uint8)AvGetI2cBusAddress(devAddress);
    uint8 regAddressWidth = (uint8)AvGetRegAddressWidth(devAddress);
    regAddressWidth = (regAddressWidth == 0) ? 0 : 1;
    uint16 regAdress = (uint32)((AvGetRegAddress(devAddress)<<8) | AvGetRegAddress(regAddress));
    ret = ManI2cRead(deviceAddress, regAdress, data, count, busAddress, regAddressWidth);
    return ret;
}

/**
 * @brief  bsp i2c write function, support bus/dev address, 8/16 register address
 * @return AvOk - success
 */
AvRet BspI2cWrite(uint32 devAddress, uint32 regAddress, uint8 *data, uint16 count)
{
    AvRet ret = AvOk;
    uint8 deviceAddress = (uint8)AvGetI2cDeviceAddress(devAddress);
    uint8 busAddress = (uint8)AvGetI2cBusAddress(devAddress);
    uint8 regAddressWidth = (uint8)AvGetRegAddressWidth(devAddress);
    regAddressWidth = (regAddressWidth == 0) ? 0 : 1;
    uint16 regAdress = (uint32)((AvGetRegAddress(devAddress)<<8) | AvGetRegAddress(regAddress));
    ret = ManI2cWrite(deviceAddress, regAdress, data, count, busAddress, regAddressWidth);
    return ret;
}
HAL_StatusTypeDef UART_Receive(uint8_t *pData, uint16_t Size)
{
    while(Size > 0)
    {
      Size--;
      if(USART1->ISR&(1<<5))
      {
         *pData++ = USART1->RDR;
         return HAL_OK;
      }
    }
    return HAL_BUSY;
}

void UART_Transmit(uint8_t *pData, uint16_t Size)
{
    while(Size > 0)
    {
     Size--;
     while((USART1->ISR&0X40)==0);
      USART1->TDR = (*pData++);
    }
  /* Check that a Tx process is not already ongoing */
}
/**
 * @brief  send one byte from uart
 * @return AvOk - success
 */
AvRet BspUartSendByte(uint8 *data, uint16 size)
{
    AvRet ret = AvOk;
#if GSV_HAL_COMPRESS
   UART_Transmit(data, size);
#else
   HAL_UART_Transmit(&huart1, data, size, 100);
#endif
    return ret;
}

/**
 * @brief  get one byte from uart
 * @return AvOk - success
 */
AvRet BspUartGetByte(uint8 *data)
{
    AvRet ret = AvOk;
#if GSV_HAL_COMPRESS
    if(UART_Receive(data,1) != HAL_OK)
        ret = AvError;
#else
    if(HAL_UART_Receive(&huart1, data, 1, 0) != HAL_OK)
        ret = AvError;
#endif
    return ret;
}

/**
 * @brief  get current time in ms
 * @return AvOk - success
 */
AvRet BspGetMilliSecond(uint32 *ms)
{
    AvRet ret = AvOk;
    *ms = HAL_GetTick();
    return ret;
}

AvRet BspGetKey(uint8 *data)
{
    AvRet ret = AvNotAvailable;
#if AvEnableKeyInput
    uint8 value;
    uint8 CombinedValue = 0;

    /* Adapt SDK KEY0 to board KEY_1 (PB12), press = high level */
    value = (uint8)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
    CombinedValue = CombinedValue | ((value == GPIO_PIN_SET) ? (1u<<0) : 0u);

    if(CombinedValue == 0)
    {
        if(KeyValue != 0)
        {
            *data = KeyValue;
            ret = AvOk;
        }
        KeyValue = 0;
        KeyDelay = 0;
    }
    else if(KeyValue != CombinedValue)
    {
        KeyValue = CombinedValue;
        KeyDelay = 0;
    }
    else if(KeyValue == CombinedValue)
    {
        if(KeyDelay <= AvKeyDelayThreshold)
            KeyDelay = KeyDelay + 1;
        if(KeyDelay == AvKeyDelayThreshold)
        {
            /* Reset System */
            HAL_NVIC_SystemReset();
        }
    }

#endif
    return ret;
}
AvRet BspIrdaGetByte(uint8 *data)
{
    AvRet ret = AvOk;
    return ret;
}

/* -------------------------------------------------------------------------- */
/* Hardware-I2C implementation (replace bit-banged i2c.c)                      */
/* devAddress: 8-bit (7-bit << 1), regAddress: 8/16-bit by Flag16bit           */
/* -------------------------------------------------------------------------- */
AvRet ManI2cWrite(uint32 devAddress,
                  uint32 regAddress,
                  uint8 *i2cdata,
                  uint16 count,
                  uint8 index,
                  uint8 Flag16bit)
{
    (void)index;
    /* Safe: bind even if BspInit() wasn't called yet */
    hal_i2c_bind(&hi2c1);

    const uint16_t dev7 = (uint16_t)((devAddress >> 1) & 0x7Fu);
    const uint16_t memSize = (Flag16bit != 0) ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT;

    HAL_StatusTypeDef st = hal_i2c_mem_write(dev7,
                                            (uint16_t)regAddress,
                                            memSize,
                                            (const uint8_t *)i2cdata,
                                            (uint16_t)count,
                                            200u);
    return (st == HAL_OK) ? AvOk : AvError;
}

AvRet ManI2cRead(uint32 devAddress,
                 uint32 regAddress,
                 uint8 *i2cdata,
                 uint16 count,
                 uint8 index,
                 uint8 Flag16bit)
{
    (void)index;
    /* Safe: bind even if BspInit() wasn't called yet */
    hal_i2c_bind(&hi2c1);

    const uint16_t dev7 = (uint16_t)((devAddress >> 1) & 0x7Fu);
    const uint16_t memSize = (Flag16bit != 0) ? I2C_MEMADD_SIZE_16BIT : I2C_MEMADD_SIZE_8BIT;

    HAL_StatusTypeDef st = hal_i2c_mem_read(dev7,
                                           (uint16_t)regAddress,
                                           memSize,
                                           (uint8_t *)i2cdata,
                                           (uint16_t)count,
                                           200u);
    return (st == HAL_OK) ? AvOk : AvError;
}
