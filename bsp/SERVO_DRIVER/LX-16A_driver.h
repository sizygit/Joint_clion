//
// Created by szy on 2021/8/10.
//

#ifndef JOINT_CLION_LX_16A_DRIVER_H
#define  JOINT_CLION_LX_16A_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Servo_driver.h"

class LX16A: public SERVO
{
public:
    enum WORKMODE{  SERVO_240 = 0 ,
                    MOTOR_CTL = 1 };    // 0 is 240 angle servo   1 is 360 angle motor
    struct
    {
        uint8_t codeStart = 0X55;
        uint8_t id;
        uint8_t dataLen;              // id + dataLen + cmd + cmdPara[0] + cmdPara[1] + ...
        uint8_t cmd;
        uint8_t cmdPara[4];           //cmdPara num maximum is 4
        uint8_t checksum;             //Checksum = ~ (ID + Length + Cmd+ Prm1 + ... Prm N) (<=255)
    }LXServoProtocolTypedef;
private:
    uint16_t moveTime;          // 0~30000 ms
    uint8_t  servoID;           // 0~253 254
    uint8_t  angleOffset;       // default value is 0

    uint16_t vinReal;
    uint16_t vinMax = 6500;     // Minimum input voltage  6500~12000 mV
    uint16_t vinMin = 12000;    // Maximum input voltage  6500~12000 mV

    WORKMODE workMode ;                  // general use servo mode
    uint8_t  errorCode;      // 0~7
public:
    static const uint8_t protocolData_LEN_MAX = 10;


    LX16A(WORKMODE p_mode);
    void Init();
    void UpdateServoData(uint8_t id,uint8_t read_type); //read_type is read cmd
    uint8_t UnpackData(uint8_t *rx_databuff);

    void SetServoExpAngle(uint16_t p_angleExp,uint16_t p_movetime_ms,uint8_t id);
    void SetServoUnload(uint8_t id , uint8_t load_or_unload);
    void WorkModeupdate(uint8_t id , WORKMODE p_workmode , int16_t motor_speed); //if use servo, this  motor_speed parameter is unuseful
};


/********************** cmd ***************************/
const uint8_t LOAD_SERVO = 1;
const uint8_t UNLOAD_SERVO = 0;
const uint8_t SERVO_MOVE_TIME_WRITE      = 1;
const uint8_t SERVO_MOVE_TIME_READ       = 2;
const uint8_t SERVO_MOVE_TIME_WAIT_WRITE = 7;
const uint8_t SERVO_MOVE_TIME_WAIT_READ  = 8;
const uint8_t SERVO_MOVE_START           = 11;
const uint8_t SERVO_MOVE_STOP            = 12;
const uint8_t SERVO_ID_WRITE             = 13;
const uint8_t SERVO_ID_READ              = 14;
const uint8_t SERVO_ANGLE_OFFSET_ADJUST  = 17;
const uint8_t SERVO_ANGLE_OFFSET_WRITE   = 18;
const uint8_t SERVO_ANGLE_OFFSET_READ    = 19;
const uint8_t SERVO_ANGLE_LIMIT_WRITE    = 20;
const uint8_t SERVO_ANGLE_LIMIT_READ     = 21;
const uint8_t SERVO_VIN_LIMIT_WRITE      = 22;
const uint8_t SERVO_VIN_LIMIT_READ       = 23;
const uint8_t SERVO_TEMP_MAX_LIMIT_WRITE = 24;
const uint8_t SERVO_TEMP_MAX_LIMIT_READ  = 25;
const uint8_t SERVO_TEMP_READ            = 26;
const uint8_t SERVO_VIN_READ             = 27;
const uint8_t SERVO_POS_READ             = 28;
const uint8_t SERVO_OR_MOTOR_MODE_WRITE  = 29;
const uint8_t SERVO_OR_MOTOR_MODE_READ   = 30;
const uint8_t SERVO_LOAD_OR_UNLOAD_WRITE = 31;
const uint8_t SERVO_LOAD_OR_UNLOAD_READ  = 32;
const uint8_t SERVO_LED_CTRL_WRITE       = 33;
const uint8_t SERVO_LED_CTRL_READ        = 34;
const uint8_t SERVO_LED_ERROR_WRITE      = 35;
const uint8_t SERVO_LED_ERROR_READ       = 36;

uint8_t LobotCheckSum(uint8_t buf[]);

#ifdef __cplusplus
}
#endif

#endif //JOINT_CLION_LX_16A_DRIVER_H
