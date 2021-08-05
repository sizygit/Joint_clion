//
// Created by szy on 2021/8/2.
//
#ifndef JOINT_CLION_PCA9685_REGISTER_H
#define JOINT_CLION_PCA9685_REGISTER_H
#include "main.h"
/***********  PCA9685 Register Map  ************/
const uint8_t SWRST_Data_Byte = 0X06;
#define INCLOCKFRE   25000000.0f
#define PCA9685_ADDR 0X40       //// 1 A5 A4 A3 A2 A1 A0
#define MODE1       0X00        /**  Mode register 1  **/
/**   Bit[7]    RESTART    R: Shows state of RESTART logic.  W:(0*):Restart disabled (1):Restart enabled
 *    Bit[6]    EXTCLK     R/W: (0*):Use internal clock.  (1): Use EXTCLK pin clock
 *    Bit[5]    AI         R/W: (0*):Register Auto-Increment disabled  (1):Register Auto-Increment enabled.
 *    Bit[4]    SLEEP      R/W: (0) :Normal mode[    (1*): Low power mode. Oscillator off
 *    Bit[3]    SUB1       R/W: (0*):PCA9685 does not respond to I2C-bus subaddress n
 *    Bit[2]    SUB2            (1) :PCA9685 responds to I2C-bus subaddress n
 *    Bit[1]    SUB3
 *    Bit[0]    ALLCALL    R/W: (0) : PCA9685 does not respond to LED All Call I2C-bus address.
 *                              (1*):  PCA9685 responds to LED All Call I2C-bus address.
 */
#define MODE2       0X01        /**  Mode register 2  **/

#define SUBADR1     0X02        /** I2C-bus subaddress 1  Bit[7:1]: 1110 001* **/
#define SUBADR2     0X03        /** I2C-bus subaddress 2  Bit[7:1]: 1110 010* **/
#define SUBADR3     0X04        /** I2C-bus subaddress 3  Bit[7:1]: 1110 100* **/
#define ALLCALLADR  0X05        /** LED All Call I2C-bus address Bit[7:1]: 1110 000* **/
#define LED0_ON_L   0X06
#define LED0_ON_H   0X07
#define LED0_OFF_L  0X08
#define LED0_OFF_H  0X09
#define LED1_ON_L   0X0A    /** LEDn_ON_L   Bit [7:0]: LEDn_ON count for LED0, 8 LSBs **/
#define LED1_ON_H   0X0B    /** LEDn_ON_H   Bit [7:5]: reserved **/
#define LED1_OFF_L  0X0C    /**             Bit [4]  : LEDn full ON **/
#define LED1_OFF_H  0X0D    /**             Bit [3:0]: LEDn_ON count for LED0, 4 MSBs **/
#define LED2_ON_L   0X0E    /** LEDn_OFF_L  Bit [7:0]: LEDn_OFF count for LED0, 8 LSBs **/
#define LED2_ON_H   0X0F    /** LEDn_OFF_H  Bit [7:5]: reserved **/
#define LED2_OFF_L  0X10    /**             Bit [4]  : LEDn full OFF **/
#define LED2_OFF_H  0X11    /**             Bit [3:0]: LEDn_OFF count for LED1, 4 MSBs **/
#define LED3_ON_L   0X12
#define LED3_ON_H   0X13
#define LED3_OFF_L  0X14
#define LED3_OFF_H  0X15
#define LED4_ON_L   0X16
#define LED4_ON_H   0X17
#define LED4_OFF_L  0X18
#define LED4_OFF_H  0X19
#define LED5_ON_L   0X1A
#define LED5_ON_H   0X1B
#define LED5_OFF_L  0X1C
#define LED5_OFF_H  0X1D
#define LED6_ON_L   0X1E
#define LED6_ON_H   0X1F
#define LED6_OFF_L  0X20
#define LED6_OFF_H  0X21
#define LED7_ON_L   0X22
#define LED7_ON_H   0X23
#define LED7_OFF_L  0X24
#define LED7_OFF_H  0X25
#define LED8_ON_L   0X26
#define LED8_ON_H   0X27
#define LED8_OFF_L  0X28
#define LED8_OFF_H  0X29
#define LED9_ON_L   0X2A
#define LED9_ON_H   0X2B
#define LED9_OFF_L  0X2C
#define LED9_OFF_H  0X2D
#define LED10_ON_L  0X2E
#define LED10_ON_H  0X2F
#define LED10_OFF_L 0X30
#define LED10_OFF_H 0X31
#define LED11_ON_L  0X32
#define LED11_ON_H  0X33
#define LED11_OFF_L 0X34
#define LED11_OFF_H 0X35
#define LED12_ON_L  0X36
#define LED12_ON_H  0X37
#define LED12_OFF_L 0X38
#define LED12_OFF_H 0X39
#define LED13_ON_L  0X3A
#define LED13_ON_H  0X3B
#define LED13_OFF_L 0X3C
#define LED13_OFF_H 0X3D
#define LED14_ON_L  0X3E
#define LED14_ON_H  0X3F
#define LED14_OFF_L 0X40
#define LED14_OFF_H 0X41
#define LED15_ON_L  0X42
#define LED15_ON_H  0X43
#define LED15_OFF_L 0X44
#define LED15_OFF_H 0X45

#define ALL_LED_ON_L  0XFA
#define ALL_LED_ON_H  0XFB
#define ALL_LED_OFF_L 0XFC
#define ALL_LED_OFF_H 0XFD
#define PRE_SCALE     0XFE    /** prescaler to program the PWM output frequency (default is 200 Hz) **/
                              /** max PWM frequence 1526HZ --0X03h   min PWM frequence 24HZ --0XFFh**/
#define TestMode      0XFF    // defines the test mode to be entered



#endif //JOINT_CLION_PCA9685_REGISTER_H
