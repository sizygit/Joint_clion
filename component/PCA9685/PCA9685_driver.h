//
// Created by szy on 2021/8/2.
//

#ifndef JOINT_CLION_PCA9685_DRIVER_H
#define JOINT_CLION_PCA9685_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PCA9685_register.h"

uint8_t PCA9685_Init();
uint8_t PCA9685_SoftWareReset();
uint8_t PCA9685_setAutoIncrement();
#ifdef __cplusplus
}
#endif

#endif //JOINT_CLION_PCA9685_DRIVER_H
