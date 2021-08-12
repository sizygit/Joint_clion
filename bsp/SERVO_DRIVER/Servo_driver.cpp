//
// Created by szy on 2021/8/10.
//

#include "Servo_driver.h"

SERVO::SERVO(uint16_t p_angleMax, uint16_t p_angleMin, int16_t p_speedMax, int16_t p_speedMin,uint8_t p_tempMax) {
    angleMax = p_angleMax;
    angleMin = p_angleMin;
    speedMax = p_speedMax;
    speedMin = p_speedMin;
    tempMax  = p_tempMax;
}

