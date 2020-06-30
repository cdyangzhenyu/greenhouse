#include<stdio.h>
#include<string.h>
#include<stdint.h> 
#include "sys.h"
#include "LORA.h"
#include "stm32f4xx.h"
#define PORTNUM  4
#define ADDRESS2 0X0002
#define ADDRESS1 0x0001
#define CHANNEL  0x01
#define SENSOR_TYPE 0x00
#define STATUS_TYPE 0X01
#define STATUS_LENGTH 0x00
/*lora�������ɺ���*/


// 00 01����ַ�� 01(�ŵ�) FE 05(����) 00 01(��ַ) 01����Ϣ���ͣ� aa bb(�غ�) FF


uint16_t  loraMessageCreate(uint8_t* loraMessage,uint8_t* info,int infolength)
{       
		int arrayIndex=0;
		int i;
		uint16_t loraMessageLength = infolength+3;
	  loraMessage[arrayIndex++]=(ADDRESS1&0XFF00)>>8;
	  loraMessage[arrayIndex++]=ADDRESS1&0X00FF;
	  loraMessage[arrayIndex++]=CHANNEL;
		loraMessage[arrayIndex++]=0xfe;
		loraMessage[arrayIndex++]=loraMessageLength; 
	  loraMessage[arrayIndex++]=(ADDRESS2&0XFF00)>>8;
	  loraMessage[arrayIndex++]=ADDRESS2&0X00FF;
		loraMessage[arrayIndex++]=SENSOR_TYPE;
		for(i=0;i<infolength;i++)
		{
			loraMessage[arrayIndex+i]=info[i];

		}
		arrayIndex+=infolength;
		loraMessage[arrayIndex++]=0xff;
		return arrayIndex;
}

//lora�������ɺ��������е�����ͨ��main.c��������ȫ�ֱ�����USART6���жϷ������и�ֵ
//ͨ��lora�������������ݣ���ʽ����
//�¶ȣ�2byte��20(�ո�) ����ʪ�ȣ�2byte��20(�ո�) ���գ�2byte�� 20(�ո�) CO2Ũ�ȣ�2byte�� 20(�ո�) ����ѹ��2byte�� 20(�ո�)
//�����¶ȣ�2byte�� 20(�ո�) ������������2byte�� 20(�ո�) �����絼�ʣ�2Byte��20 ������ˮ��(2byte) 20����PH��2byte�� 20(�ո�)

uint16_t loraInfoCreate(uint8_t* info)
{
  int arrayIndex=0;
	info[arrayIndex++]=(air_temperature&0xFF00)>>8;
  info[arrayIndex++]=air_temperature&0x00FF;
	info[arrayIndex++]=0x20;
	info[arrayIndex++]=(air_humidity&0xFF00)>>8;
  info[arrayIndex++]=air_humidity&0x00FF;
	info[arrayIndex++]=0x20;
  info[arrayIndex++]=(air_light&0xFF00)>>8;
  info[arrayIndex++]=air_light&0x00FF;
	info[arrayIndex++]=0x20;
	info[arrayIndex++]=(air_co2&0xFF00)>>8;
  info[arrayIndex++]=air_co2&0x00FF;
	info[arrayIndex++]=0x20;
	info[arrayIndex++]=(air_pressure&0xFF00)>>8;
  info[arrayIndex++]=air_pressure&0x00FF;
	info[arrayIndex++]=0x20;
	info[arrayIndex++]=(soil_temperature&0xFF00)>>8;
  info[arrayIndex++]=soil_temperature&0x00FF;
	info[arrayIndex++]=0x20;
	info[arrayIndex++]=(soil_conductivity&0xFF00)>>8;
  info[arrayIndex++]=soil_conductivity&0x00FF;
	info[arrayIndex++]=0x20;
	info[arrayIndex++]=(soil_TDS&0xFF00)>>8;
  info[arrayIndex++]=soil_TDS&0x00FF;
	info[arrayIndex++]=0x20;
	info[arrayIndex++]=(soil_water&0xFF00)>>8;
  info[arrayIndex++]=soil_water&0x00FF;
	info[arrayIndex++]=0x20;
	info[arrayIndex++]=(soil_PH&0xFF00)>>8;
  info[arrayIndex++]=soil_PH&0x00FF;
	info[arrayIndex++]=0x20;
	return arrayIndex;
	}

uint16_t  loraStatusCreate(uint8_t* loraMessage,uint16_t outPort)
{       
		int arrayIndex=0;
	  loraMessage[arrayIndex++]=(ADDRESS1&0XFF00)>>8;
	  loraMessage[arrayIndex++]=ADDRESS1&0X00FF;
	  loraMessage[arrayIndex++]=CHANNEL;
		loraMessage[arrayIndex++]=0xfe;
		loraMessage[arrayIndex++]=0x05;
	  loraMessage[arrayIndex++]=(ADDRESS2&0XFF00)>>8;
	  loraMessage[arrayIndex++]=ADDRESS2&0X00FF;
		loraMessage[arrayIndex++]=STATUS_TYPE;
		loraMessage[arrayIndex++]=!PFout(outPort);
   	loraMessage[arrayIndex++]=outPort;
		loraMessage[arrayIndex++]=0xff;
		return arrayIndex;
}
//FE 06 93 90 01 00 01��PF5���� 00(PF6) 00(PF7) 01(PF8) FF
uint16_t  loraAllStatusCreate(uint8_t* loraMessage)
{       
		int arrayIndex=0;
		loraMessage[arrayIndex++]=(ADDRESS1&0XFF00)>>8;
	  loraMessage[arrayIndex++]=ADDRESS1&0X00FF;
	  loraMessage[arrayIndex++]=CHANNEL;
		loraMessage[arrayIndex++]=0xfe;
		loraMessage[arrayIndex++]=PORTNUM+3;
		loraMessage[arrayIndex++]=(ADDRESS2&0XFF00)>>8;
	  loraMessage[arrayIndex++]=ADDRESS2&0X00FF;
	  loraMessage[arrayIndex++]=STATUS_TYPE;
		loraMessage[arrayIndex++]=!PFout(5);
   	loraMessage[arrayIndex++]=!PFout(6);
	  loraMessage[arrayIndex++]=!PFout(7);
	  loraMessage[arrayIndex++]=!PFout(8);
		loraMessage[arrayIndex++]=0xff;
		return arrayIndex;
}


