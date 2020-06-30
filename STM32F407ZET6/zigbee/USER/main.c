#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "rs485.h"
#include "modbusRTUencoder.h"
#include <stdio.h>
#include "zigbee.h"
#include "relay.h"
#include "key.h"
#include "exti.h"

uint16_t air_temperature,air_light,air_co2,air_pressure,soil_temperature,soil_water,soil_conductivity,soil_PH,air_humidity,soil_TDS; //所需的全局变量





int main(void)
{ 
	u8 rs485buf[20]; 
	u8 key;
	int i=0;
	int zigbee_messagelength=0;
	int zigbee_datalength=0;
	uint16_t co2_message[8]={0};                            //初始化阶段
	uint16_t light_message[8]={0};
	uint16_t pressure_message[8]={0};
	uint16_t soilPH_message[8]={0};
	uint16_t soilConductivity_message[8]={0};
	uint8_t zigbee_message[200]={0};
	uint8_t relayStatus_message[9]={0};
	uint8_t dataArray[40];
	zigbee_datalength=zigbeeInfoCreate(dataArray);
  zigbee_messagelength=zigbeeMessageCreate(zigbee_message,dataArray,zigbee_datalength);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);   //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	Usart6_Init();
	LED_Init();					//初始化LED 
	RS485_Init(4800);		//初始化RS485串口2
	relay_init();
	EXTI_init();          //KEY_INIT在EXTI_init里面做了
  co2_modbusMessageCreate(co2_message);                            //生成485问询报文
	light_modbusMessageCreate(light_message);
	pressure_modbusMessageCreate(pressure_message);
	conductivity_modbusMessageCreate(soilConductivity_message);
  soilPH_modbusMessageCreate(soilPH_message);
  printf("初始化完成\r\n");
 									  
	while(1)
	{
	/*	printf("请按键\r\n");
		key=KEY_Scan(0);
		if(key)
		{
		  switch(key)
			{case WKUP_PRES:
				PF5=~PF5;
				LED1=~LED1;
				break;
		    case KEY0_PRES:
				PF6=~PF6;
				LED0=~LED0;
				break;
				case KEY1_PRES:
				PF7=~PF7;
				break;
			  case KEY2_PRES:
				PF8=~PF8;
				break;
			}
		}else delay_ms(10); */
   	RS485_Send_Data(soilConductivity_message,8);
		RS485_Receive_Data(rs485buf,&key);
		delay_ms(100);
		RS485_Send_Data(co2_message,8);
		RS485_Receive_Data(rs485buf,&key);
		delay_ms(100);
	  RS485_Send_Data(light_message,8);
		RS485_Receive_Data(rs485buf,&key);
		delay_ms(100);
		RS485_Send_Data(pressure_message,8);
		RS485_Receive_Data(rs485buf,&key);
		delay_ms(100);
		RS485_Send_Data(soilPH_message,8);
		RS485_Receive_Data(rs485buf,&key);
		delay_ms(100);
		zigbee_datalength=zigbeeInfoCreate(dataArray);
    zigbee_messagelength=zigbeeMessageCreate(zigbee_message,dataArray,zigbee_datalength);
		/*printf("zigbee will send message:");
	  for(i=0;i<zigbee_messagelength;i++)
		{
		printf("%02x \t",zigbee_message[i]);
		}
		printf("\r\n");	*/
		delay_ms(1000);
		
		zigbeeStatusCreate(relayStatus_message,5);
	/*	printf("PF5 STATUS:");
		for(i=0;i<9;i++)
		printf("%02x \t",relayStatus_message[i]);
		printf("\r\n"); */
		
		zigbeeStatusCreate(relayStatus_message,6);
			/*printf("PF6 STATUS:");
		for(i=0;i<9;i++)
		printf("%02x \t",relayStatus_message[i]);
		printf("\r\n");*/
		
		zigbeeStatusCreate(relayStatus_message,7);
		/*	printf("PF7 STATUS:");
		for(i=0;i<9;i++)
		printf("%02x \t",relayStatus_message[i]);
		printf("\r\n");*/
		
		zigbeeStatusCreate(relayStatus_message,8);
			/*printf("PF8 STATUS:");
		for(i=0;i<9;i++)
		printf("%02x \t",relayStatus_message[i]);
		printf("\r\n");*/
		
		delay_ms(1000);
		for(i=0;i<zigbee_messagelength;i++)
		{
		usart_sendbyte(USART6,zigbee_message[i]);
	  }
	}  
}
