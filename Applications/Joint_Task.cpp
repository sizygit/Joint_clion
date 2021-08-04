//
// Created by szy on 2021/7/31.
//

#include "Joint_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"



void Joint_Task(void const * argument)
{
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