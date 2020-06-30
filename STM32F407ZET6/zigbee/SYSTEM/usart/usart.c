#include "sys.h"
#include "usart.h"	
#include "relay.h"
#include "zigbee.h"
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
#define ZIGBEE_REC_LENGTH 0x09
#define ON  0X01
#define OF  0X00
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART6_RX_BUF[USART_REC_LEN];   
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
u16 USART6_RX_STA=0;  

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
  } 

} 

void Usart6_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
  

  GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);      //UART4_TX
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);        //UART4_RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStructure);
  

  
  USART_Cmd(USART6, ENABLE);
	#if EN_USART6_RX	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart6 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}


void USART6_IRQHandler(void)                	//����1�жϷ������
{
	int i=0;
	u8 res;
  u8 operation;               //����1������0
	uint16_t port;
	uint8_t relayStatus_message[9]={0};
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)//���յ�����
	{	 	
	  res =USART_ReceiveData(USART6);//;��ȡ���յ�������USART2->DR
		if(USART6_RX_STA<20)
		{
			USART6_RX_BUF[USART6_RX_STA]=res;		//��¼���յ���ֵ
			USART6_RX_STA++;						//������������1 
		} 
		if(USART6_RX_BUF[0]!=0xFE)  //��ͷ��������Ч��Ϣ�Ž��к�����������buffer��0
		{ 
			USART6_RX_STA=0;
		}
		if(res==0xFF&&USART6_RX_BUF[0]==0xFE)
	 {
		 printf("zigbee received bag:");
		 for(i=0;i<ZIGBEE_REC_LENGTH;i++)
		 printf("%02x \t",USART6_RX_BUF[i]);
		 printf("\r\n");
		 port=0xFF&USART6_RX_BUF[7];
      if(USART6_RX_BUF[6]==0x00)
		  {switch(USART6_RX_BUF[7])
			{
				case 0x00:
				PF0=1;
				break;
				case 0x02:
				PF2=1;
				break;
				case 0x03:
				PF3=1;
				break;
				case 0x04:
				PF4=1;
				break;
				case 0x05:
				PF5=1;
				break;
		    case 0x06:
				PF6=1;
				break;
				case 0x07:
				PF7=1;
				break;
			  case 0x08:
				PF8=1;
				break;
			}
	zigbeeStatusCreate(relayStatus_message,port);
	for(i=0;i<9;i++)
	usart_sendbyte(USART6,relayStatus_message[i]);
			
					 }
			
	    else if(USART6_RX_BUF[6]==0x01)
		{
			PFout(port);
		  switch(USART6_RX_BUF[7])
			{
				case 0x00:
				PF0=0;
				break;
				case 0x02:
				PF2=0;
				break;
				case 0x03:
				PF3=0;
				break;
				case 0x04:
				PF4=0;
				break;
				case 0x05:
				PF5=0;
				break;
		    case 0x06:
				PF6=0;
				break;
				case 0x07:
				PF7=0;
				break;
			  case 0x08:
				PF8=0;
				break;
			 }
	zigbeeStatusCreate(relayStatus_message,port);
	for(i=0;i<9;i++)
  usart_sendbyte(USART6,relayStatus_message[i]);
      }
		else if(USART6_RX_BUF[6]==0x03)
		{
		zigbeeStatusCreate(relayStatus_message,port);
	  for(i=0;i<9;i++)
    usart_sendbyte(USART6,relayStatus_message[i]);
		}
		  USART6_RX_STA=0;

		}
	 }
	}

	
	void usart_sendbyte(USART_TypeDef* USARTx,uint8_t data)
{
USART_SendData( USARTx, data);
while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

