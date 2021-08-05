//
// Created by szy on 2021/8/2.
//

#include "PCA9685_driver.h"
#include "myiic.h"
#include "math.h"
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
    if((res >> 7) == 1) {                                  //check the RESTART bit
        IIC_Write_One_Byte(PCA9685_ADDR,MODE1,res & 0X6F);  //clear the SLEEP bit from 1 to 0
        HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin,GPIO_PIN_SET);
        HAL_Delay(1);                                        // Allow time for oscillator to stabilize
    }
    if(IIC_Write_One_Byte(PCA9685_ADDR,MODE1,0Xa0)!= 0)  //Write logic 1 to bit 7(RESTART) and AI no ALLCALL
    {                                                          //if needs EXTCLK
        HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
        return 1;
    }
    HAL_Delay(1);                                       // Allow time for oscillator to stabilize
    res = IIC_Read_One_Byte(PCA9685_ADDR,MODE1);             // 0X20 0010 0000
    HAL_UART_Transmit_DMA(&huart1,&res,1);             ///debug
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
 *          --AI can be set in PCA9685_setFrequency(） and  PCA9685_Restart()
 * @return  0: right
 *          1: false
 */
uint8_t PCA9685_setAutoIncrement()
{
    uint8_t res;
    res = IIC_Read_One_Byte(PCA9685_ADDR,MODE1);
    if((res & 0X20) != 0)
        return 0;
    IIC_Write_One_Byte(PCA9685_ADDR,MODE1,0X20);
    res = IIC_Read_One_Byte(PCA9685_ADDR,MODE1);
    //HAL_UART_Transmit_DMA(&huart1,&res,1);          ///debug
    if((res & 0X20) != 0)
        return 0;
    else
        HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
    return 1;
}
/**
 * @brief   set EXTCLK
 *          -- I don't use this  NO Guarantee
 * @return  none
 */
uint8_t PCA9685_setEXTCLK()
{
    uint8_t res;
    res = IIC_Read_One_Byte(PCA9685_ADDR,MODE1);
    HAL_UART_Transmit_DMA(&huart1,&res,1);          ///debug
    IIC_Write_One_Byte(PCA9685_ADDR,MODE1,(0X50 | res));   //Write logic 1s to both the SLEEP and EXTCLK bits in MODE1.
}
/**
 * @brief   set the PRE_SCALE register to get the expected frequency(24 Hz to 1526 Hz -- 0X03 to 0XFF)
 *          --25 MHz typical internal  --External 50 MHz (max.) clock input
 *          --The PRE_SCALE register can only be set when the SLEEP bit of MODE1 register is set to logic 1
 *          --at the same time, AI is configured in this function
 * @param   freq: the output modulation frequency required
 * @return  0: right
 *          1: false
 */
uint8_t PCA9685_setFrequency(uint16_t freq)
{
    uint8_t res;
    IIC_Init();
    if(freq < 24 || freq > 1526)                            //error frequency
        return 1;
    float prescaleval = (INCLOCKFRE / 4096.0f) / freq - 1;
    uint8_t pre_scale = floor(prescaleval + 0.5);
    res = IIC_Read_One_Byte(PCA9685_ADDR,MODE1);            //read the MODE1 register
    /*The PRE_SCALE register can only be set when SLEEP(Bit 4) is logic 1*/
    if((res & 0x10) == 0)
        IIC_Write_One_Byte(PCA9685_ADDR,MODE1,(res & 0x7F) | 0x10); //set SLEEP to 1
    HAL_Delay(1);
    if(IIC_Write_One_Byte(PCA9685_ADDR,PRE_SCALE,pre_scale))  return 1;
    IIC_Write_One_Byte(PCA9685_ADDR,MODE1,res);
    HAL_Delay(1);
    IIC_Write_One_Byte(PCA9685_ADDR,MODE1,res | 0xa1);       //set Restart and AI Bit  ALLCALL
    HAL_Delay(1);
    return 0;
}

uint8_t PCA9685_setPWM(uint8_t pin, uint16_t led_on, uint16_t led_off)
{
    // pin 0 -15       count 0-4095
    if(pin < 0 || pin > 15)
        return 1;
    uint8_t  buffer[4];
    buffer[0] = led_on;
    buffer[1] = led_on >> 8;
    buffer[2] = led_off;
    buffer[3] = led_off >> 8;
    IIC_Write_Multi_Bytes(PCA9685_ADDR,LED0_ON_L + 4 * pin,4,buffer);
    uint8_t res[4];                                                 ///debug
    IIC_Read_Multi_Bytes(PCA9685_ADDR,LED0_ON_L + 4 * pin,4,res);   ///debug
    HAL_UART_Transmit_DMA(&huart1,res,4);                           ///debug
}
