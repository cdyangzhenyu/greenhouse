#include "sys.h"		    
#include "rs485.h"	 
#include "delay.h"
#include "modbusRTUencoder.h"
#include <stdio.h>
#if EN_USART2_RX   		//如果使能了接收   	  
//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;   
void USART2_IRQHandler(void)
{
	int i=0;
	float temp;
	int pressure,temperature,humidity;
	int light;
	int co2;                                                     //在主函数里生命，在.h文件里extern声明
	int PH;
	int soil_Temperature,soil_Water,soil_Conductivity;
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
	  res =USART_ReceiveData(USART2);//;读取接收到的数据USART2->DR
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//记录接收到的值
			RS485_RX_CNT++;						//接收数据增加1 
		} 
		if(RS485_RX_BUF[0]!=0x01&&RS485_RX_BUF[0]!=0x02&&RS485_RX_BUF[0]!=0x05&&RS485_RX_BUF[0]!=0x06&&RS485_RX_BUF[0]!=0x04)  //包头必须是有效信息才进行后续处理，否则buffer清0
		{ 
			RS485_RX_CNT=0;
		}
		if(RS485_RX_CNT==CO2_RET_LENGTH&&RS485_RX_BUF[0]==0x01)
	 {

			
		co2=(RS485_RX_BUF[7]<<8)+RS485_RX_BUF[8];
			 air_co2=co2;
		//	 printf("二氧化碳浓度： %dppm\r\n",air_co2);
			 humidity=(RS485_RX_BUF[3]<<8)+RS485_RX_BUF[4];
			 air_humidity=humidity;
		//	 printf("空气湿度： %.1f%%\r\n",(float)air_humidity/10);
		
		 RS485_RX_CNT=0;
	 }
	 else if(RS485_RX_CNT==PRESSRUE_RET_LENGTH&&RS485_RX_BUF[0]==0x02)                          //大气压模块地址为2，处理大气压数据
	 {
			 pressure=(RS485_RX_BUF[3]<<8)+RS485_RX_BUF[4];
			 air_pressure= pressure;
			 temperature=(RS485_RX_BUF[5]<<8)+RS485_RX_BUF[6];
			 air_temperature=temperature;
			 temp=(float)pressure/10;
			 printf("大气压强： %.1f Kpa  \r\n",temp);
			 temp=(float)temperature/10;
			 printf("空气温度 ： %.1f C\r\n",temp);
		
		 RS485_RX_CNT=0;
	 }
	 
	  else if(RS485_RX_CNT==LIGHT_RET_LENGTH&&RS485_RX_BUF[0]==0x05)                          //光照模块地址为5，处理光照数据
	 {		
		light=(RS485_RX_BUF[15]<<8)+RS485_RX_BUF[16];
		air_light=light;
		printf("光照强度： %dLUX\r\n",light);
		 RS485_RX_CNT=0;
	 } 
   if(RS485_RX_CNT==TDS_RET_LENGTH&&RS485_RX_BUF[0]==0x06)
	 {


			soil_Temperature=(RS485_RX_BUF[5]<<8)+RS485_RX_BUF[6];
			 soil_temperature=soil_Temperature;
			 temp=(float)soil_Temperature/10;
		  printf("土壤温度is %.1f  C\r\n",temp);
       soil_Water=(RS485_RX_BUF[3]<<8)+RS485_RX_BUF[4];
			 soil_water=soil_Water;
	     temp=(float)soil_Water/10;
			 printf("土壤含水量 %.1f  %%\r\n",temp);
			 soil_Conductivity=(RS485_RX_BUF[7]<<8)+RS485_RX_BUF[8];
			 soil_conductivity=soil_Conductivity;
			 soil_TDS=soil_Conductivity/2;
		//	 printf("土壤电导率 %d  us/com \r\n",soil_conductivity);
		//	 printf("土壤含盐量 %d  ppm \r\n",soil_TDS);
		 RS485_RX_CNT=0;
	 }
	 
	 if(RS485_RX_CNT==SOILPH_RET_LENGTH&&RS485_RX_BUF[0]==0x04)
	 {

		PH=RS485_RX_BUF[4];
		soil_PH=PH;
		temp=(float)PH/10;
		//printf("PH值： %.1f  \r\n",temp);
		 RS485_RX_CNT=0;
	 }	 
	 
		
		
		
	}  											 
} 
#endif										 
//初始化IO 串口2
//bound:波特率	  
void RS485_Init(u32 bound)
{  	 
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
	
  //串口2引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
	
	//USART2    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3
	
	//PA8推挽输出，485模式控制
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //GPIOA8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PG8
	

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
  USART_Cmd(USART2, ENABLE);  //使能串口 2
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接受中断

	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif	
	
	RS485_TX_EN=0;				//默认为接收模式	
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u16 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{
	  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
    USART_SendData(USART2,buf[t]); //发送数据
	}	 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
	RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//设置为接收模式	
}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
	}
}