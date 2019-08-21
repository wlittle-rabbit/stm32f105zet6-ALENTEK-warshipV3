#include "main.h"
/*PB6---SCL  PB7---SDA*/


void at24cxx_i2c_init()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
  
  I2C_InitTypeDef I2C_InitStructure;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0xA0; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress =I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 200000;
  
  I2C_Cmd(I2C1, ENABLE);
  I2C_Init(I2C1, &I2C_InitStructure);//apply config after enable
  //I2C_AcknowledgeConfig(I2C1,ENABLE);//允许一字节一应答
}
void i2c_write(u8 addr,u8 data)
{
  I2C_AcknowledgeConfig(I2C1,ENABLE);//使能应答
  I2C_GenerateSTART(I2C1,ENABLE);//发送一个开始位
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){;} //等待EV5
  I2C_Send7bitAddress(I2C1,0xA0,I2C_Direction_Transmitter); //发送从地址
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){;} //等待EV6
  I2C_SendData(I2C1,addr); //发送要写入数据的地址
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){;} //等待EV8
  I2C_SendData(I2C1,data); //发送要写入的数据
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){;} //等待EV8
  I2C_GenerateSTOP(I2C1,ENABLE); //发送停止位
}

u8 i2c_read(u8 nAddr)
{
  I2C_AcknowledgeConfig(I2C1,ENABLE); //使能应答
  I2C_GenerateSTART(I2C1,ENABLE); //发送一个开始位
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){;} //等待EV5
  I2C_Send7bitAddress(I2C1,0xA0,I2C_Direction_Transmitter); //发送一个伪写指令
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){;}//等待EV6
  I2C_SendData(I2C1,nAddr);//发送读地址
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){;} //等待EV8

  I2C_GenerateSTART(I2C1,ENABLE); //发送一个开始位
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){;} //等待EV5
  I2C_Send7bitAddress(I2C1,0xA0,I2C_Direction_Receiver); //发送一个读指令
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){;} //等待EV6
  I2C_AcknowledgeConfig(I2C1,DISABLE); //应答使能关闭
  I2C_GenerateSTOP(I2C1,ENABLE); //发送一个停止位
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){;} //等待EV7
  return I2C_ReceiveData(I2C1); //返回读到的数据
}








/*一下为模拟I2C*/
void at24cxx()
{
  I2C_Cmd(I2C1, ENABLE);
  I2C_SendData(I2C1, 0xFF);
  //ReceivedData = I2C_ReceiveData(I2C1);
}



void init_i2c()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);
}
void SDA_HIGH()
{
  GPIO_SetBits(GPIOB,GPIO_Pin_7);
}
void SDA_LOW()
{
  GPIO_ResetBits(GPIOB,GPIO_Pin_7);
}
void SCL_HIGH()
{
  GPIO_SetBits(GPIOB,GPIO_Pin_6);
}
void SCL_LOW()
{
  GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}
void SDA_PPOUT()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SDA_IN()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void i2c_start()//I2C START:clk high,SDA from high to low
{
  SDA_PPOUT();
  SDA_HIGH();
  SCL_HIGH();
  delay_us(4);
  SDA_LOW();
  delay_us(4);
  SCL_LOW();
}
void i2c_stop()//scl为高，sda从低到高跳变
{
  SDA_PPOUT();
  SCL_HIGH();
  SDA_LOW();
  delay_us(4);
  SDA_HIGH();
  delay_us(4);
}
int i2c_waitack()//0 失败 1 接受到应答
{
  int i=0;
  SDA_IN();
  SDA_HIGH();
  delay_us(1);
  SCL_HIGH();
  delay_us(1);
  while((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))==Bit_SET)
  {
    i++;
    if(i>250)
    {
       i2c_stop();
       return 0;
    }
  }
  SCL_LOW();
  return 1;
}
void i2c_ack()
{
  SCL_LOW();
  SDA_PPOUT();
  
}
void send_bit(int bit)
{
  SDA_PPOUT();
  if(bit>0){
    SDA_HIGH();
    delay_us(4);
    SCL_LOW();
    delay_us(4);
    SCL_HIGH();
    delay_us(4);
  }
  else{
    SDA_LOW();
    delay_us(4);
    SCL_LOW();
    delay_us(4);
    SCL_HIGH();
    delay_us(4);
  }
}
void send_bits(char bits)
{
  for(int i=7;i>=0;i--){
    if(bits&(1<<i))
      send_bit(1);
    else
      send_bit(0);
  }
}