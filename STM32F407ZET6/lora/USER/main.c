#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "rs485.h"
#include "modbusRTUencoder.h"
#include <stdio.h>
#include "LORA.h"
#include "relay.h"
#include "key.h"
#include "exti.h"

uint16_t air_temperature,air_light,air_co2,air_pressure,soil_temperature,soil_water,soil_conductivity,soil_PH,air_humidity,soil_TDS; //�����ȫ�ֱ���





int main(void)
{ 
	u8 rs485buf[20]; 
	u8 key;
	int i=0;
	int lora_messagelength=0;
	int lora_datalength=0;
	int allStatuslength=0;
	uint8_t status_message[200]={0};
	uint16_t co2_message[8]={0};                            //��ʼ���׶�
	uint16_t light_message[8]={0};
	uint16_t pressure_message[8]={0};
	uint16_t soilPH_message[8]={0};
	uint16_t soilConductivity_message[8]={0};
	uint8_t lora_message[200]={0};
	uint8_t relayStatus_message[9]={0};
	uint8_t dataArray[40];
	lora_datalength=loraInfoCreate(dataArray);
  lora_messagelength=loraMessageCreate(lora_message,dataArray,lora_datalength);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);   //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	Usart6_Init();
	LED_Init();					//��ʼ��LED 
	RS485_Init(4800);		//��ʼ��RS485����2
	relay_init();
	EXTI_init();          //KEY_INIT��EXTI_init��������
  co2_modbusMessageCreate(co2_message);                            //����485��ѯ����
	light_modbusMessageCreate(light_message);
	pressure_modbusMessageCreate(pressure_message);
	conductivity_modbusMessageCreate(soilConductivity_message);
  soilPH_modbusMessageCreate(soilPH_message);
  printf("��ʼ�����\r\n");
 									  
	while(1)
	{
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
		lora_datalength=loraInfoCreate(dataArray);
    lora_messagelength=loraMessageCreate(lora_message,dataArray,lora_datalength);
	  allStatuslength=loraAllStatusCreate(status_message);
		delay_ms(1000);
		delay_ms(1000);
		for(i=0;i<lora_messagelength;i++)
		{
		usart_sendbyte(USART6,lora_message[i]);
	  } 
		for(i=0;i<lora_messagelength;i++)
		{
		printf("%02x\t",lora_message[i]);
		}
		printf("\r\n");
	/*	for(i=0;i<allStatuslength;i++)
		{
		printf("%02x\t",status_message[i]);
		}*/
		printf("\r\n");
/*	delay_ms(300);
		for(i=0;i<allStatuslength;i++)
		{
		usart_sendbyte(USART6,status_message[i]);
	  } */
	}  
}
