/*  ��ӿ⺯��ͷ�ļ� */
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
**�������ƣ�void delay(unsigned int ms)     Name: void delay(unsigned int ms)
**���������������ʱ
**��ڲ�����unsigned int ms   ��������ʱ��ֵ
**�������
*******************************************************************************/
void delay(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  ͨ��һ������ѭ��������ʱ*/
    for(y = 3000 ; y > 0 ; y--);
}

/*    ������    */
int main(void)
{
  
  unsigned char i;
  extern const unsigned char BMP1[];
  disableInterrupts();                    //�ر�ϵͳ���ж�
  //�ڲ�ʱ��Ϊ1��Ƶ = 16Mhz
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);//CPUDIV = 1 HSIDIV = 1  �ڲ�ʱ�� = 16Mhz            
  IIC_Init();
  OLED_Init();//����I2Cģ���ʼ������                     
  enableInterrupts();            //��ϵͳ���ж�                
  //printf("\r\n\r\n\r\nAT24C02__���Գɹ�----------------->>>>>!!");
  while(1)                        //������ѭ�����ȴ����ڽ����ж�
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