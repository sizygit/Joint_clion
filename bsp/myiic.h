#ifndef __MYIIC_H
#define __MYIIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

#include "main.h"

/* Private define--you should adapt to fact------------------------------*/
#define SCL_GPIO      GPIOB
#define SCL_GPIO_PIN  GPIO_PIN_6
#define SDA_GPIO      GPIOB
#define SDA_GPIO_PIN  GPIO_PIN_7
/* ----------------------------------------------------------------------*/
/*********** simulate IIC **************/
#define SCL_H         SCL_GPIO->BSRR = SCL_GPIO_PIN	                    /* GPIO_SetBits(GPIOB , GPIO_Pin_6)   */
#define SCL_L         SCL_GPIO->BSRR = (uint32_t)SCL_GPIO_PIN << 16u;   /* GPIO_ResetBits(GPIOB , GPIO_Pin_6) */

#define SDA_H         SDA_GPIO->BSRR = SDA_GPIO_PIN	                    /* GPIO_SetBits(GPIOB , GPIO_Pin_7)   */
#define SDA_L         SDA_GPIO->BSRR = (uint32_t)SDA_GPIO_PIN << 16u;    /* GPIO_ResetBits(GPIOB , GPIO_Pin_7) */

#define SCL_read      SCL_GPIO->IDR  & SCL_GPIO_PIN                      /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_6) */
#define SDA_read      SDA_GPIO->IDR  & SDA_GPIO_PIN	                    /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_7) */
/*----------------------------------------------------------*/
uint8_t IIC_Write_One_Byte(uint8_t slave_addr,uint8_t control_addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t slave_addr,uint8_t control_addr);
void IIC_Init(void);
/*-----------------------------------------------------------*/
void IIC_Delay(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

uint8_t checkDevice(uint8_t DeviceAddress);

#ifdef __cplusplus
}
#endif

#endif
















