//
// Created by szy on 2021/8/10.
//

#include "LX-16A_driver.h"
#include "usart.h"
#define SERVO_HUART huart1

extern LX16A lx16A;
uint8_t lx16A_rx_buf[LX16A::protocolData_LEN_MAX];
uint8_t lx16A_tx_buf[LX16A::protocolData_LEN_MAX];

LX16A::LX16A(WORKMODE p_mode):SERVO(1000,0,-1000,1000,85)
{
    workMode = p_mode;           //set the relevant LX-16A 's working mode
    servoType = BUS_SERVO;       //LX-16A is a bus servo
}

void LX16A::Init() {
    for (uint8_t id = 1; id <= servoNum ; ++id) {
        SetServoUnload(id, LOAD_SERVO);   // load the torque output
        WorkModeupdate(id,SERVO_240,0); //set motro mode
    }
    while((HAL_UART_GetState(&SERVO_HUART)) == HAL_UART_STATE_BUSY_TX) ; //wait for Uart DMA transfer's end
}

void LX16A::WorkModeupdate(uint8_t id , WORKMODE p_workmode , int16_t motor_speed) {
    workMode = p_workmode;
    if (motor_speed > 1000)  motor_speed = 1000;
    if (motor_speed < -1000) motor_speed = -1000;
    speedExp = motor_speed;
    while((HAL_UART_GetState(&SERVO_HUART)) == HAL_UART_STATE_BUSY_TX) ; //wait for Uart DMA transfer's end
    lx16A_tx_buf[0] = LXServoProtocolTypedef.codeStart;
    lx16A_tx_buf[1] = LXServoProtocolTypedef.codeStart;
    lx16A_tx_buf[2] = id;
    lx16A_tx_buf[3] = 7;
    lx16A_tx_buf[4] = SERVO_OR_MOTOR_MODE_WRITE;
    lx16A_tx_buf[5] = p_workmode;
    lx16A_tx_buf[6] = 0;
    lx16A_tx_buf[7] = (uint8_t)motor_speed;         //if use servo, this parameter is unuseful
    lx16A_tx_buf[8] = (uint8_t)(motor_speed >> 8);
    lx16A_tx_buf[9] = LobotCheckSum(lx16A_tx_buf);
    HAL_UART_Transmit_DMA(&SERVO_HUART,lx16A_tx_buf,10);
}

void LX16A::SetServoUnload(uint8_t id, uint8_t load_or_unload) {
    while((HAL_UART_GetState(&SERVO_HUART)) == HAL_UART_STATE_BUSY_TX) ; //wait for Uart DMA transfer's end
    lx16A_tx_buf[0] = LXServoProtocolTypedef.codeStart;
    lx16A_tx_buf[1] = LXServoProtocolTypedef.codeStart;
    lx16A_tx_buf[2] = id;
    lx16A_tx_buf[3] = 4;
    lx16A_tx_buf[4] = SERVO_LOAD_OR_UNLOAD_WRITE;
    if (load_or_unload == LOAD_SERVO)
        lx16A_tx_buf[5] = LOAD_SERVO;
    if (load_or_unload == UNLOAD_SERVO)
        lx16A_tx_buf[5] = UNLOAD_SERVO;
    lx16A_tx_buf[6] = LobotCheckSum(lx16A_tx_buf);
    HAL_UART_Transmit_DMA(&SERVO_HUART,lx16A_tx_buf,lx16A_tx_buf[3]+3);
}

void LX16A::SetServoExpAngle(uint16_t p_angleExp,uint16_t p_movetime_ms,uint8_t id) {
    if(p_angleExp > 1000)
        p_angleExp = 1000;
    if(p_movetime_ms > 30000)
        p_movetime_ms = 30000;
    while((HAL_UART_GetState(&SERVO_HUART)) == HAL_UART_STATE_BUSY_TX) ; //wait for Uart DMA transfer's end
    lx16A_tx_buf[0] = LXServoProtocolTypedef.codeStart;
    lx16A_tx_buf[1] = LXServoProtocolTypedef.codeStart;
    lx16A_tx_buf[2] = id;
    lx16A_tx_buf[3] = 7;
    lx16A_tx_buf[4] = SERVO_MOVE_TIME_WRITE;
    lx16A_tx_buf[5] = (uint8_t)p_angleExp;
    lx16A_tx_buf[6] = p_angleExp >> 8;
    lx16A_tx_buf[7] = (uint8_t)p_movetime_ms;
    lx16A_tx_buf[8] = p_movetime_ms >> 8;
    lx16A_tx_buf[9] = LobotCheckSum(lx16A_tx_buf);
    HAL_UART_Transmit_DMA(&SERVO_HUART,lx16A_tx_buf,10);
}

uint8_t rx_complete = 0;
void LX16A::UpdateServoData(uint8_t id, uint8_t read_type) {
    while((HAL_UART_GetState(&SERVO_HUART)) == HAL_UART_STATE_BUSY_TX) ; //wait for Uart DMA transfer's end
    lx16A_tx_buf[0] = LXServoProtocolTypedef.codeStart;
    lx16A_tx_buf[1] = LXServoProtocolTypedef.codeStart;
    lx16A_tx_buf[2] = id;
    lx16A_tx_buf[3] = 3;
    lx16A_tx_buf[4] = read_type;
    lx16A_tx_buf[5] = LobotCheckSum(lx16A_tx_buf);
    HAL_UART_Transmit(&SERVO_HUART,lx16A_tx_buf,lx16A_tx_buf[3]+3,15);
    HAL_UART_Receive_IT(&SERVO_HUART,lx16A_rx_buf,8);              //open the IT
    while ((HAL_UART_GetState(&SERVO_HUART)) != HAL_UART_STATE_READY)
        HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin,GPIO_PIN_RESET);

}

uint8_t LX16A::UnpackData(uint8_t *rx_databuff) {
    if (rx_databuff[0] == LXServoProtocolTypedef.codeStart && rx_databuff[1] == LXServoProtocolTypedef.codeStart){
        uint8_t id = rx_databuff[2];
        uint8_t len = rx_databuff[3];
        if (rx_databuff[len+2] == LobotCheckSum(rx_databuff)){
            switch (rx_databuff[4]) {
                case SERVO_MOVE_TIME_READ:
                case SERVO_POS_READ:
                    angleLast = angleReal;
                    angleReal = (int16_t)(rx_databuff[5] | (rx_databuff[6] << 8)); //angle can < 0
                default:
                    for (unsigned char & i : lx16A_rx_buf)
                        i = 0;
                    return id;                               // return the relevant cmd
            }
        }
    }
    return 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &SERVO_HUART)
    {
        lx16A.UnpackData(lx16A_rx_buf);
        HAL_GPIO_TogglePin(LED_B_GPIO_Port,LED_B_Pin);
        rx_complete = 1;
    }


}

uint8_t LobotCheckSum(uint8_t buf[])
{
    uint8_t i;
    uint16_t temp = 0;
    for (i = 2; i < buf[3] + 2; i++) {
        temp += buf[i];
    }
    temp = ~temp;
    i = (uint8_t)temp;
    return i;
}


