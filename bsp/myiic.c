#include "myiic.h"
#include "delay.h"


/********** simulate IIC **********/

// initialize the IIC pins -- you should adapt to fact
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**I2C GPIO Configuration
    PF1     ------> I2C1_SCL
    PF0     ------> I2C1_SDA
    */
    /*Configure GPIO pins : PF0 PF1 */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    while(1);
    IIC_Stop();          //pull up the SDA and SCL
    HAL_Delay(50);
}


// IIC delay function
void IIC_Delay(void)
{
    delay_us(3);
}


// Generating the IIC start signal
void IIC_Start(void)
{
	SDA_H;	  	  
	SCL_H;
	IIC_Delay();
 	SDA_L;    //START:when CLK is high,DATA change form high to low
	IIC_Delay();
	SCL_L;
}

// Generate IIC stop signal
void IIC_Stop(void)
{
	SCL_L;
	SDA_L;    //STOP:when CLK is high DATA change form low to high
 	IIC_Delay();
	SCL_H; 
	SDA_H;    //Sending the I2C bus end signal
	IIC_Delay();							   	
}
// wait for acknowledge
// return：1:not acknowledge
//         0: acknowledge
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_H;IIC_Delay();	   
	SCL_H;IIC_Delay();	 
	while(SDA_read)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL_L;    // pull down the scl
	return 0;  
} 
//  generate an acknowledge
void IIC_Ack(void)
{
	SCL_L;
	SDA_L;
	IIC_Delay();
	SCL_H;
	IIC_Delay();
	SCL_L;
}
//  not generate an acknowledge
void IIC_NAck(void)
{
	SCL_L;
	SDA_H;
	IIC_Delay();
	SCL_H;
	IIC_Delay();
	SCL_L;
}
//IIC send a byte
//return have acknowledge or not
//1，have acknowledge
//0，not  acknowledge
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;
    SCL_L;      //Pull down the clock to start data transfer
    for(t=0;t<8;t++)
    {
       if((txd&0x80)>>7 == 1)
           SDA_H;
       else
           SDA_L;
        txd<<=1; 	  
		    SCL_H;
		    IIC_Delay(); 
		    SCL_L;	
		    IIC_Delay();
    }	 
} 	    
//read a byte; ack = 1, send ACK; ack = 0 , send nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
    for(i=0;i<8;i++ )
	{
        SCL_L; 
        IIC_Delay();
		SCL_H;
        receive<<=1;
        if(SDA_read)receive++;
		IIC_Delay(); 
    }					 
    if (!ack)
        IIC_NAck();//send nACK
    else
        IIC_Ack(); //send ACK
    return receive;
}


//  the function which can use a full transfer data (a byte)
uint8_t IIC_Write_One_Byte(uint8_t slave_addr,uint8_t control_addr,uint8_t data)
{
    IIC_Start();
    IIC_Send_Byte((slave_addr<<1) | 0); //slave address + Write Bit
    if(IIC_Wait_Ack())                      //wait for acknowledge from slave
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(control_addr);            //send the control address
    IIC_Wait_Ack();
    IIC_Send_Byte(data);                    //write the data to the control address
    if(IIC_Wait_Ack())
    {
        IIC_Stop();
        return 1;
    }
    IIC_Stop();
}
// the function which can use a full read data (a byte)
uint8_t IIC_Read_One_Byte(uint8_t slave_addr,uint8_t control_addr)
{
    uint8_t res;
    IIC_Start();
    IIC_Send_Byte((slave_addr<<1) | 0);  //slave address + Write Bit
    IIC_Wait_Ack();                          //wait for acknowledge from slave
    IIC_Send_Byte(control_addr);             //send the control address
    IIC_Wait_Ack();                          //change the direction
    IIC_Start();
    IIC_Send_Byte((slave_addr<<1) | 1);  //slave address + Read Bit
    IIC_Wait_Ack();
    res = IIC_Read_Byte(0);              //read data and send nACK to stop the transfer
    IIC_Stop();
    return res;
}
/*******************************************************************************
* Function Name  : I2C_ReadByte
* Description    : 检测设备工作正常并且通讯成功
* Input          : DeviceAddress: 设备地址
* Output         : None
* Return         : 返回为:=1成功,=0失败，并且亮起警示灯
* Attention	: None
*******************************************************************************/
uint8_t checkDevice(uint8_t DeviceAddress){
    uint8_t haveAck;
    IIC_Start();
    IIC_Send_Byte(DeviceAddress & 0xFFFE);
    haveAck = IIC_Wait_Ack();
    if(haveAck)HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    IIC_Stop();
    return haveAck;
}






















