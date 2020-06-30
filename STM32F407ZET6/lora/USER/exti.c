#include "exti.h"
#include "key.h"
#include "stm32f4xx.h"
#include "led.h"
#include "relay.h"
#include "delay.h"
#include  "usart.h"
#include  "lora.h"
void EXTI_init()
{
  EXTI_InitTypeDef EXTI_InitStruct; 
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	KEY_Init();
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);
	EXTI_InitStruct.EXTI_Line=EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
  EXTI_Init(&EXTI_InitStruct);
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);
	EXTI_InitStruct.EXTI_Line=EXTI_Line2;
	EXTI_Init(&EXTI_InitStruct);
	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource3);
	EXTI_InitStruct.EXTI_Line=EXTI_Line3;
	EXTI_Init(&EXTI_InitStruct);  
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_Line=EXTI_Line0;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
  NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel=EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=4;
	NVIC_Init(&NVIC_InitStruct);
	
	
}

void EXTI0_IRQHandler()	
{ int i;
	int length=0;
	uint8_t relayStatus_message[9]={0};
	delay_ms(10);
	  if(WK_UP==1)
		{PF8=~PF8;
		LED0=~LED0;
		length=loraStatusCreate(relayStatus_message,8);
	  for(i=0;i<length;i++)
	  usart_sendbyte(USART6,relayStatus_message[i]);
		printf("return message:");		
		for(i=0;i<length;i++)
		printf("%02x \t",relayStatus_message[i]);
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	
}

void EXTI2_IRQHandler()
{ int i;
	int length=0;
	uint8_t relayStatus_message[9]={0}; 
	delay_ms(10);
	if(KEY2==0)
	  {PF7=~PF7;
	  LED0=~LED0;
	  length=loraStatusCreate(relayStatus_message,7);
	  for(i=0;i<length;i++)
	  usart_sendbyte(USART6,relayStatus_message[i]);	
		printf("return message:");		
		for(i=0;i<length;i++)
		printf("%02x \t",relayStatus_message[i]);
		}
		EXTI_ClearITPendingBit(EXTI_Line2);

}

void EXTI3_IRQHandler()
{   
	int i;
	int length=0;
	uint8_t relayStatus_message[9]={0};
	 delay_ms(10);
	  if(KEY1==0)
		{
		PF6=~PF6;
		PF4=~PF4;
		LED1=~LED1;
		length=loraStatusCreate(relayStatus_message,6);
	  for(i=0;i<length;i++)
	  usart_sendbyte(USART6,relayStatus_message[i]);
		printf("return message:");		
		for(i=0;i<length;i++)
		printf("%02x \t",relayStatus_message[i]);
		}
		EXTI_ClearITPendingBit(EXTI_Line3);

}

void EXTI4_IRQHandler()
{int i;
	int length=0;
	uint8_t relayStatus_message[9]={0};
	  delay_ms(10);
		if(KEY0==0)
	  {PF5=~PF5;
		LED1=~LED1;
	length=loraStatusCreate(relayStatus_message,5);
	for(i=0;i<length;i++)
	usart_sendbyte(USART6,relayStatus_message[i]);	
		printf("return message:");		
		for(i=0;i<length;i++)
		printf("%02x \t",relayStatus_message[i]);}
	  EXTI_ClearITPendingBit(EXTI_Line4);
	  
}






