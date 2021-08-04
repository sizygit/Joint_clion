//
// Created by szy on 2021/8/2.
//

#include "PCA9685_driver.h"
#include "myiic.h"

#ifndef PCA9685
#define PCA9685
#endif
extern UART_HandleTypeDef huart1;
/**
 * @brief   initialize the PCA9685 and IIC bus
 * @return  0 : initialization is right
 *          1 : initialization is false
 */
uint8_t PCA9685_Init()
{
    uint8_t bit7_RESTART,res=0Xff;
    IIC_Init();
    HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET);
    res = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0);//IIC_Read_One_Byte(PCA9685_ADDR,MODE1);
    HAL_UART_Transmit_DMA(&huart1,&res,1);
    if((res << 7) == 1) {                                  //check the
        IIC_Write_One_Byte(PCA9685_ADDR,MODE1,0X10);  //clear the SLEEP bit from 1 to 0
        HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_RESET);
    }
    HAL_Delay(1);                  // Allow time for oscillator to stabilize
    IIC_Write_One_Byte(PCA9685_ADDR,MODE1,0X80);      //Write logic 1 to bit 7(RESTART)
}
/**
 * @brief   take a software reset for PCA9685 to reset all the devices to the power-up state
 * @return  0 : reset is right
 *          1 : reset is false
 */
uint8_t PCA9685_SoftWareReset()
{
    IIC_Start();
    IIC_Send_Byte((MODE1 << 1) | 0);  // SWRST Call sequence
    if(IIC_Wait_Ack()){
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(SWRST_Data_Byte);
    if(IIC_Wait_Ack()){
        IIC_Stop();
        return 1;
    }
    else
        IIC_Stop();
    return 0;
}
