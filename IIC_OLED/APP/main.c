/*  添加库函数头文件 */
#include "stm8s.h"
#include "AT24C02.h"


void OLED_Fill();
void OLED_ShowCN();
void OLED_ShowStr();
void OLED_CLS();
void OLED_OFF();
void OLED_ON();
void OLED_DrawBMP();
void OLED_Init();
/*******************************************************************************
**函数名称：void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**功能描述：大概延时
**入口参数：unsigned int ms   输入大概延时数值
**输出：无
*******************************************************************************/
void delay(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  通过一定周期循环进行延时*/
    for(y = 3000 ; y > 0 ; y--);
}

/*    主函数    */
int main(void)
{
  
  unsigned char i;
  extern const unsigned char BMP1[];
  disableInterrupts();                    //关闭系统总中断
  //内部时钟为1分频 = 16Mhz
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);//CPUDIV = 1 HSIDIV = 1  内部时钟 = 16Mhz            
  IIC_Init();
  OLED_Init();//调用I2C模块初始化函数                     
  enableInterrupts();            //开系统总中断                
  //printf("\r\n\r\n\r\nAT24C02__测试成功----------------->>>>>!!");
  while(1)                        //进入死循环，等待串口接收中断
  {
      OLED_Fill(0xFF);
      delay(2000);
      OLED_Fill(0x00);
      delay(2000);
      for(i=0;i<5;i++)
      {
              OLED_ShowCN(22+i*16,0,i);
      }
      delay(2000);
      OLED_ShowStr(0,3,"HelTec Automation",1);
      OLED_ShowStr(0,4,"Hello Tech",2);				
      delay(2000);
      OLED_CLS();
      OLED_OFF();
      delay(2000);
      OLED_ON();
      OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);
  }
}