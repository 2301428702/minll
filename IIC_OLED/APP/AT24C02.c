#include "stm8s.h"
#include "codetab.h"


#define I2C_SPEED              200000
#define I2C_SLAVE_ADDRESS7     0X78

/*******************************************************************************
**函数名称：void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**功能描述：大概延时
**入口参数：unsigned int ms   输入大概延时数值
**输出：无
*******************************************************************************/
void Delayms(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  通过一定周期循环进行延时*/
    for(y = 3000 ; y > 0 ; y--);
}
/*******************************************************************************
**函数名称:void IIC_Init() 
**功能描述:初始化IIC1接口
**入口参数:
**输出:无
*******************************************************************************/
void IIC_Init()
{					


  I2C_Cmd(ENABLE);
  
  /* sEE_I2C configuration after enabling it */
  I2C_Init(I2C_SPEED, I2C_SLAVE_ADDRESS7, 
           I2C_DUTYCYCLE_16_9, I2C_ACK_CURR,
           I2C_ADDMODE_7BIT, 16);
}


/*******************************************************************************
**函数名称:void IIC_Write(unsigned char subaddr , unsigned char Byte_addr , unsigned char *buffer , unsigned short num)
**功能描述:向IIC器件写数据
**入口参数:
          subaddr :  从器件地址
          Byte_addr : 确定器件写地址的起始地址
          *buffer   : 写数据的起址地址
          num				: 要写数据的个数
**输出:无
*******************************************************************************/
void IIC_Write(unsigned char subaddr , unsigned char Byte_addr , unsigned char buffer , unsigned short num)
{

    //发送起始信号/*!< Send STRAT condition */
    I2C_GenerateSTART( ENABLE);  
    
    /*!< Test on EV5 and clear it *///等待起始信号产生
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT));
    
      /*!< Send EEPROM address for write *///发送器件地地址，并清除SB标志位
    I2C_Send7bitAddress((u8)subaddr, I2C_DIRECTION_TX);
    /*!< Test on EV6 and clear it *///等待器件地址发送完成并清除发送器件地址标志位
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    /*!< Send the EEPROM's internal address to write to : LSB of the address */
    I2C_SendData((u8)(Byte_addr));//发送器件存储首地址
    /*!< Test on EV8 and clear it *///等待移位发送器发送完成
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      
    
    /*!< Send the byte to be written *///发送器件存储首地址
    I2C_SendData(buffer);
        /*!< Test on EV8 and clear it *///发送等待移位器发送完成
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED));	
    /*!< Send STOP condition *///发送停止信号结束数据传输
    I2C_GenerateSTOP(ENABLE);
}

void WriteCmd(unsigned char I2C_Command) //写命令
{
	IIC_Write(0x78,0x00,I2C_Command,1);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
	IIC_Write(0x78,0x40, I2C_Data,1);
}
//OLED初始化
void OLED_Init(void)
{
	Delayms(100); //?aà?μ??óê±oü??òa
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //áá?èμ÷?ú 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}


void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}


void OLED_ON(void)
{
	WriteCmd(0X8D);  //éè??μ?oé±?
	WriteCmd(0X14);  //?a??μ?oé±?
	WriteCmd(0XAF);  //OLED??D?
}


void OLED_OFF(void)
{
	WriteCmd(0X8D);  //éè??μ?oé±?
	WriteCmd(0X10);  //1?±?μ?oé±?
	WriteCmd(0XAE);  //OLEDDY??
}


void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}


void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}


void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

void OLED_CLS(void)//
{
	OLED_Fill(0x00);
}
