#include "main.h"

void init_flash()
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  SPI_InitTypeDef SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS signal controled by soft
  SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_256; //Ԥ��Ƶ256
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  SPI_Cmd(SPI2, ENABLE);
}
u8 spi2_writeread(u8 data)
{
    u8 recedata=0;
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);
    SPI_I2S_SendData(SPI2, data);
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
    recedata = SPI_I2S_ReceiveData(SPI2);
    return recedata;
}
void enable_w25()
{
   GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}
void disenable_w25()
{
   GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
u8 W25_readSR()
{
  u8 data;
  enable_w25();
  spi2_writeread(0x05);
  data=spi2_writeread(0xFF);
  disenable_w25();
  return data;
}
void W25_wait_busy()
{
  while((W25_readSR()&0x01)==0x01);  	
}

void W25_writenable()
{
  enable_w25();
  spi2_writeread(W25_WRITE_ENABLE_INSTRUC);
  disenable_w25();
}
//��ָ������д����Ϊ1��256���ֽڣ���д�ĵ�ַ��Ԥ�����ݱ���ΪFFh������д���ɹ��� 
//д����ǰ�������ȷ���дʹ������
//����ȴ�ִbusy ��Ϊ0ʱ��д��Ž���
void W25_write_page(u8 *buffer,u32 start_address,u32 write_num)
{
  u8 address1,address2,address3;
  address1=(u8)(start_address>>16);
  address2=(u8)(start_address>>8);
  address3=(u8)(start_address);
  W25_writenable();
  enable_w25();
  spi2_writeread(W25_WRITE_PAGE_INSTRUC);
  spi2_writeread(address1);
  spi2_writeread(address2);
  spi2_writeread(address3);
  for(int i=0;i<write_num;i++){
    spi2_writeread(buffer[i]);
    //spi2_writeread(i);
  }
  disenable_w25();
  W25_wait_busy();
}

//��������������Ҫ��ִ��дʹ�����������ĵ�ַ����Ϊ0xFF,ÿ������4K��С,�ȴ�busyΪ0ʱ�������Ž���
void W25_sectorerase(u32 erase_address)
{
  u8 address1,address2,address3;
  address1=(u8)(erase_address>>16);
  address2=(u8)(erase_address>>8);
  address3=(u8)(erase_address);
  W25_writenable();
  enable_w25();
  spi2_writeread(W25_ERASE_SECTOR_INSTRUC);
  spi2_writeread(address1);
  spi2_writeread(address2);
  spi2_writeread(address3);
  disenable_w25();
  W25_wait_busy();
}
//W25��ĳ��ַ������ȡ����
void W25_read(u8 *buffer,u32 start_address,u32 read_num)
{
  u8 address1,address2,address3;
  address1=(u8)(start_address>>16);
  address2=(u8)(start_address>>8);
  address3=(u8)(start_address);
  enable_w25();
  spi2_writeread(W25_READ_DATA_INSTRUC);
  spi2_writeread(address1);
  spi2_writeread(address2);
  spi2_writeread(address3);
  for(int i=0;i<read_num;i++){
    buffer[i]=spi2_writeread(0xFF);
  }
  disenable_w25();
}
