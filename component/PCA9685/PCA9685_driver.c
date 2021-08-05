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
 * @brief   initialize the PCA9685 and IIC bus--restart mode
 * @return  0 : initialization is right
 *          1 : initialization is false
 */
uint8_t PCA9685_Restart()
{
    uint8_t bit7_RESTART,res=0Xf1;
    IIC_Init();
    res = IIC_Read_One_Byte(PCA9685_ADDR,MODE1);
    HAL_UART_Transmit_DMA(&huart1,&res,1);             ///debug
    if((res >> 7) == 1) {                                  //check the
        IIC_Write_One_Byte(PCA9685_ADDR,MODE1,0X10);  //clear the SLEEP bit from 1 to 0
        HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_SET);
    }
    HAL_Delay(1);                                         // Allow time for oscillator to stabilize
    if(IIC_Write_One_Byte(PCA9685_ADDR,MODE1,0X80)!= 0)    //Write logic 1 to bit 7(RESTART)
    {                                                           //if needs EXTCLK
        HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
        return 1;
    }
    return 0;
}
/**
 * @brief   take a software reset for PCA9685 to reset all the devices to the power-up state
 * @return  0 : reset is right
 *          1 : reset is false
 */
uint8_t PCA9685_SoftWareReset()
{
    IIC_Init();
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

/**
 * @brief   set MODE1 's AI Bit to 1 for enabling Auto-Increment
 * @return  0: right
 *          1: false
 */
uint8_t PCA9685_setAutoIncrement()
{
    uint8_t res;
    IIC_Write_One_Byte(PCA9685_ADDR,MODE1,0X20);
    res = IIC_Read_One_Byte(PCA9685_ADDR,MODE1);
    //HAL_UART_Transmit_DMA(&huart1,&res,1);          ///debug
    if((res >> 5) == 1)
        return 0;
    else
        HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
    return 1;
}

uint8_t PCA9685_setEXTCLK()
{
    uint8_t res;
    res = IIC_Read_One_Byte(PCA9685_ADDR,MODE1);
    HAL_UART_Transmit_DMA(&huart1,&res,1);          ///debug
    IIC_Write_One_Byte(PCA9685_ADDR,MODE1,(0X50 | res));   //Write logic 1s to both the SLEEP and EXTCLK bits in MODE1.

}