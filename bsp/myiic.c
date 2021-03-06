#include "myiic.h"
#include "delay.h"


/********** simulate IIC **********/
static uint8_t IIC_InitDone = 0;
// initialize the IIC pins -- you should adapt to fact
void IIC_Init(void)
{
    if (IIC_InitDone == 0) {
        IIC_InitDone = 1;
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**I2C GPIO Configuration
        PF1     ------> I2C1_SCL
        PF0     ------> I2C1_SDA
        */
        /*Configure GPIO pins : PF0 PF1 */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

        IIC_Stop();          //pull up the SDA and SCL
        HAL_Delay(1);
    }
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
// return??1:not acknowledge
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
//1??have acknowledge
//0??not  acknowledge
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
    return 0;
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
/**
 * @brief IIC continuous reading
 *        -- enable register Auto-Increment
 * @bug   when buf[] is local variables,the IIC_Read_Multi_Bytes() can't read correctly in last two  bytes
 * @return
 */
uint8_t IIC_Read_Multi_Bytes(uint8_t slave_addr,uint8_t control_addr,uint8_t len,uint8_t *buf)
{
    /*IIC_Start();
    IIC_Send_Byte((slave_addr<<1) | 0);  //slave address + Write Bit
    if(IIC_Wait_Ack())                          //wait for acknowledge from slave
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(control_addr);             //send the control address
    IIC_Wait_Ack();                          //wait for acknowledge from slave
    IIC_Start();
    IIC_Send_Byte((slave_addr << 1) | 1);
    IIC_Wait_Ack();
    while(len)
    {
        if(len==1)*buf=IIC_Read_Byte(0);//??????,????nACK
        else *buf=IIC_Read_Byte(1);		//??????,????ACK
        len--;
        buf++;
    }
    IIC_Stop();
    return 0;*/
    for (;  ; ) {
        *buf = IIC_Read_One_Byte(slave_addr,control_addr);
        len --;
        buf ++;
        control_addr ++;
        if(len == 0)
            return 0;
    }
}
/**
 * @brief IIC continuous reading
 *        -- enable register Auto-Increment
 * @return
 */
uint8_t IIC_Write_Multi_Bytes(uint8_t slave_addr,uint8_t control_addr,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    IIC_Start();
    IIC_Send_Byte((slave_addr<<1) | 0);  //slave address + Write Bit
    if(IIC_Wait_Ack())	                     //wait for acknowledge
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(control_addr);             //send the control address
    IIC_Wait_Ack(); 	                     //wait for acknowledge
    for(i=0;i<len;i++)
    {
        IIC_Send_Byte(buf[i]);	            //send data
        if(IIC_Wait_Ack())
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_Stop();
    return 0;
    for (;  ; ) {
        if (len == 1){
            IIC_Write_One_Byte(slave_addr,control_addr,*buf);
            return 0;
        }
        else
        {
            IIC_Write_One_Byte(slave_addr,control_addr,*buf);
            len --;
            buf ++;
            control_addr ++;
        }
    }
}























