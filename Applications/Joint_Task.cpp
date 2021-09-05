//
// Created by szy on 2021/7/31.
//

#include "Joint_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "LX-16A_driver.h"
extern UART_HandleTypeDef huart1;

LX16A lx16A(LX16A::SERVO_240);
#define LX16A_EN
//#define PCA9685_EN
int16_t angle1;
int16_t angle2;
void Joint_Task(void const * argument)
{
#ifdef PCA9685_EN
    if(PCA9685_SoftWareReset() != 0)
        HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
    PCA9685_setFrequency(50);
    PCA9685_Restart();
    PCA9685_setPWM(0,0,366);
    PCA9685_setPWM(1,0,356);
    PCA9685_setPWM(2,0,326);
    PCA9685_setPWM(3,0,396);
#endif
#ifdef LX16A_EN
    lx16A.Init();
    lx16A.SetServoExpAngle(300,2000,2);
    lx16A.SetServoExpAngle(600,2000,1);
    HAL_Delay(10);
    lx16A.UpdateServoData(1,SERVO_POS_READ);
    angle1 = lx16A.angleReal;
    lx16A.UpdateServoData(2,SERVO_POS_READ);
    angle2 = lx16A.angleReal;


#endif
    if(HAL_TIM_Base_Start(&htim1) == HAL_OK)
        if(HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1) == HAL_OK)
            HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_RESET);


    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1300);

    for(;;)
    {
        osDelay(1);
    }
}