#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"
#include "modbusRTUencoder.h"
#include <stdio.h>
#if EN_USART2_RX   		//���ʹ���˽���   	  
//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;   
void USART2_IRQHandler(void)
{
	int i=0;
	float temp;
	int pressure,temperature,humidity;
	int light;
	int co2;                                                     //������������������.h�ļ���extern����
	int PH;
	int soil_Temperature,soil_Water,soil_Conductivity;
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
	{	 	
	  res =USART_ReceiveData(USART2);//;��ȡ���յ�������USART2->DR
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
			RS485_RX_CNT++;						//������������1 
		} 
		if(RS485_RX_BUF[0]!=0x01&&RS485_RX_BUF[0]!=0x02&&RS485_RX_BUF[0]!=0x05&&RS485_RX_BUF[0]!=0x06&&RS485_RX_BUF[0]!=0x04)  //��ͷ��������Ч��Ϣ�Ž��к�����������buffer��0
		{ 
			RS485_RX_CNT=0;
		}
		if(RS485_RX_CNT==CO2_RET_LENGTH&&RS485_RX_BUF[0]==0x01)
	 {

			
		co2=(RS485_RX_BUF[7]<<8)+RS485_RX_BUF[8];
			 air_co2=co2;
		//	 printf("������̼Ũ�ȣ� %dppm\r\n",air_co2);
			 humidity=(RS485_RX_BUF[3]<<8)+RS485_RX_BUF[4];
			 air_humidity=humidity;
		//	 printf("����ʪ�ȣ� %.1f%%\r\n",(float)air_humidity/10);
		
		 RS485_RX_CNT=0;
	 }
	 else if(RS485_RX_CNT==PRESSRUE_RET_LENGTH&&RS485_RX_BUF[0]==0x02)                          //����ѹģ���ַΪ2���������ѹ����
	 {
			 pressure=(RS485_RX_BUF[3]<<8)+RS485_RX_BUF[4];
			 air_pressure= pressure;
			 temperature=(RS485_RX_BUF[5]<<8)+RS485_RX_BUF[6];
			 air_temperature=temperature;
			 temp=(float)pressure/10;
			 printf("����ѹǿ�� %.1f Kpa  \r\n",temp);
			 temp=(float)temperature/10;
			 printf("�����¶� �� %.1f C\r\n",temp);
		
		 RS485_RX_CNT=0;
	 }
	 
	  else if(RS485_RX_CNT==LIGHT_RET_LENGTH&&RS485_RX_BUF[0]==0x05)                          //����ģ���ַΪ5�������������
	 {		
		light=(RS485_RX_BUF[15]<<8)+RS485_RX_BUF[16];
		air_light=light;
		printf("����ǿ�ȣ� %dLUX\r\n",light);
		 RS485_RX_CNT=0;
	 } 
   if(RS485_RX_CNT==TDS_RET_LENGTH&&RS485_RX_BUF[0]==0x06)
	 {


			soil_Temperature=(RS485_RX_BUF[5]<<8)+RS485_RX_BUF[6];
			 soil_temperature=soil_Temperature;
			 temp=(float)soil_Temperature/10;
		  printf("�����¶�is %.1f  C\r\n",temp);
       soil_Water=(RS485_RX_BUF[3]<<8)+RS485_RX_BUF[4];
			 soil_water=soil_Water;
	     temp=(float)soil_Water/10;
			 printf("������ˮ�� %.1f  %%\r\n",temp);
			 soil_Conductivity=(RS485_RX_BUF[7]<<8)+RS485_RX_BUF[8];
			 soil_conductivity=soil_Conductivity;
			 soil_TDS=soil_Conductivity/2;
		//	 printf("�����絼�� %d  us/com \r\n",soil_conductivity);
		//	 printf("���������� %d  ppm \r\n",soil_TDS);
		 RS485_RX_CNT=0;
	 }
	 
	 if(RS485_RX_CNT==SOILPH_RET_LENGTH&&RS485_RX_BUF[0]==0x04)
	 {

		PH=RS485_RX_BUF[4];
		soil_PH=PH;
		temp=(float)PH/10;
		//printf("PHֵ�� %.1f  \r\n",temp);
		 RS485_RX_CNT=0;
	 }	 
	 
		
		
		
	}  											 
} 
#endif										 
//��ʼ��IO ����2
//bound:������	  
void RS485_Init(u32 bound)
{  	 
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	
  //����2���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2
	
	//USART2    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
	//PA8���������485ģʽ����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIOA8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PG8
	

   //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ��� 2
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���������ж�

	//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif	
	
	RS485_TX_EN=0;				//Ĭ��Ϊ����ģʽ	
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u16 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
    USART_SendData(USART2,buf[t]); //��������
	}	 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
	RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}