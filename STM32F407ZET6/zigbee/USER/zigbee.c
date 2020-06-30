#include<stdio.h>
#include<string.h>
#include<stdint.h> 
#include "sys.h"
#include "zigbee.h"
#include "stm32f4xx.h"
#define SRC_PORT 0X91
#define SRC_PORT2 0X92  
#define DST_PORT 0X90
#define ADDRESS  0x0001
/*zigbee报文生成函数*/


uint16_t  zigbeeMessageCreate(uint8_t* zigbeeMessage,uint8_t* info,int infolength)
{       
		int arrayIndex=0;
		int i;
		uint16_t zigbeeMessageLength = infolength+4;
		zigbeeMessage[arrayIndex++]=0xfe;

		zigbeeMessage[arrayIndex++]=zigbeeMessageLength;

		zigbeeMessage[arrayIndex++]=SRC_PORT;

		zigbeeMessage[arrayIndex++]=DST_PORT;
	
		zigbeeMessage[arrayIndex++]=ADDRESS&0xFF;
		zigbeeMessage[arrayIndex++]=(ADDRESS&0xFF00)>>8;
		for(i=0;i<infolength;i++)
		{
			zigbeeMessage[arrayIndex+i]=info[i];

		}
		arrayIndex+=zigbeeMessageLength-4;
		zigbeeMessage[arrayIndex++]=0xff;
		return arrayIndex;
}

//zigbee数据生成函数，其中的数据通过main.c中声明的全局变量在USART6的中断服务函数中赋值
//通过zigbee发出传感器数据，格式如下
//温度（2byte）20(空格) 空气湿度（2byte）20(空格) 光照（2byte） 20(空格) CO2浓度（2byte） 20(空格) 大气压（2byte） 20(空格)
//土壤温度（2byte） 20(空格) 土壤含盐量（2byte） 20(空格) 土壤电导率（2Byte）20 土壤含水量(2byte) 20土壤PH（2byte） 20(空格)

uint16_t zigbeeInfoCreate(uint8_t* info)
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

uint16_t  zigbeeStatusCreate(uint8_t* zigbeeMessage,uint16_t outPort)
{       
		int arrayIndex=0;
		zigbeeMessage[arrayIndex++]=0xfe;
		zigbeeMessage[arrayIndex++]=0x06;
		zigbeeMessage[arrayIndex++]=SRC_PORT2;
		zigbeeMessage[arrayIndex++]=DST_PORT;
		zigbeeMessage[arrayIndex++]=ADDRESS&0xFF;
		zigbeeMessage[arrayIndex++]=(ADDRESS&0xFF00)>>8;
		zigbeeMessage[arrayIndex++]=!PFout(outPort);
   	zigbeeMessage[arrayIndex++]=outPort;
		zigbeeMessage[arrayIndex++]=0xff;
		return arrayIndex;
}
