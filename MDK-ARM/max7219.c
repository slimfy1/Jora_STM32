#include "max7219.h"

uint8_t aTxBuf[1]={0};
extern SPI_HandleTypeDef hspi3;
char dg=8;

#define cs_set() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)
#define cs_reset() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET)

void Send_7219 (uint8_t rg, uint8_t dt)
{
	cs_set();
	aTxBuf[0]=rg;
	HAL_SPI_Transmit (&hspi3, (uint8_t*)aTxBuf, 1, 5000);
	aTxBuf[0]=dt;
	HAL_SPI_Transmit (&hspi3, (uint8_t*)aTxBuf, 1, 5000);
	cs_reset();
}
//------------------------------------------------------
void Clear_7219 (void)
{
	uint8_t i=dg;
	do
	{
          Send_7219(i,0x0);//������ �������
	} while (--i);
}
//------------------------------------------------------
void Number_7219 (volatile long n)
{
	uint8_t ng=0;//���������� ��� ������
	if(n<0)
	{
          ng=1;
          n*=-1;
	}
	uint8_t i=1;
	do
	{
          Send_7219(++i,n%10);//������ �����
          n/=10;
	} while(n);
	if(ng)
	{
          Send_7219(i+1,0x0A);//������ -
	}
}
//-------------------------------------------------------
void Init_7219 (void)
{
  Send_7219(0x09,0x00);//������� ����� �������������
  Send_7219(0x0B,0x07);//���-�� ������������ ��������
  Send_7219(0x0A,0x01);//������������� ��������
  Send_7219(0x0C,0x01);//������� ���������
  Clear_7219();
}
