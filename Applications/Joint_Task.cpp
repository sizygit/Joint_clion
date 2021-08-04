//
// Created by szy on 2021/7/31.
//

#include "Joint_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "PCA9685_driver.h"
extern UART_HandleTypeDef huart1;
uint8_t hello=0xff;
void Joint_Task(void const * argument)
{
    HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
    PCA9685_Init();
    while(1);
    HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
    if(HAL_TIM_Base_Start(&htim1) == HAL_OK)
        if(HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1) == HAL_OK)
            HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_RESET);


    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1100);

    for(;;)
    {
        osDelay(1);
    }
}