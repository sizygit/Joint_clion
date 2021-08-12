//
// Created by szy on 2021/8/10.
//

#ifndef JOINT_CLION_SERVO_DRIVER_H
#define JOINT_CLION_SERVO_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
class SERVO
{
protected:
    int16_t speedReal{};
    int16_t speedExp{};
    int16_t speedLast{};
    int16_t speedMax{};
    int16_t speedMin{};

    int16_t current{};

    uint8_t temp{};
    uint8_t tempMax{};


    uint16_t angleExp{};
    uint16_t angleLast{};
    uint16_t angleMax{};
    uint16_t angleMin{};

public:
    uint16_t angleReal{};
    enum SERVO_TYPE
    {
        BUS_SERVO,          // the bus generally use UART
        DIGITAL_SERVO     // the PWM is general usage
    };
    SERVO_TYPE servoType{};
    const static uint8_t servoNum = 1;
    SERVO(uint16_t p_angleMax,uint16_t p_angleMin,int16_t p_speedMax,int16_t p_speedMin,uint8_t p_tempMax);

    virtual void Init() = 0;
    virtual void SetServoExpAngle(uint16_t p_angleExp,uint16_t p_movetime_ms,uint8_t id) = 0;
    virtual void UpdateServoData(uint8_t id,uint8_t read_type) = 0;


};


#ifdef __cplusplus
}
#endif

#endif //JOINT_CLION_SERVO_DRIVER_H
