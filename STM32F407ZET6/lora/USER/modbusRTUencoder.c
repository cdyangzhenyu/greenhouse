#include "modbusRTUencoder.h"
#include <stdint.h>
#include "CRC16.h"
#define ADDRESS_1 0x01
#define ADDRESS_2 0x02
#define ADDRESS_4 0x04
#define ADDRESS_5 0x05
#define ADDRESS_6 0x06
#define ASK     0x03

#define CO2_REG_ADDR 0X0000
#define CO2_RET_LENGTH 0x0003


#define LIGHT_REG_ADDR 0X0000
#define LIGHT_RET_LENGTH 0x0007


#define PRESSURE_REG_ADDR 0x0000
#define  PRESSURE_RET_LENGTH 0X0002

#define CONDUCTIVITY_REG_ADDR 0x0000
#define CONDUCTIVITY_RET_LENGTH 0x0003

#define SOILPH_REG_ADDR 0x000d
#define SOILPH_RET_LENGTH 0x0001

void co2_modbusMessageCreate(uint16_t *message)
{
	 int arrayIndex=0; 
	 uint16_t tempCRC=0;
   message[arrayIndex++]=ADDRESS_1;
	 message[arrayIndex++]=ASK;
	 message[arrayIndex++]=(CO2_REG_ADDR&0xFF00)>>8;
	 message[arrayIndex++]=(CO2_REG_ADDR&0x00FF);
	 message[arrayIndex++]=(CO2_RET_LENGTH&0xFF00)>>8;
	 message[arrayIndex++]=(CO2_RET_LENGTH&0x00FF);
	 tempCRC=CRC16_MODBUS(message,arrayIndex);
	 tempCRC=tempCRC&0xFFFF;
	 message[arrayIndex++]=tempCRC&0x00FF;
	 message[arrayIndex++]=(tempCRC&0xFF00)>>8;

}


uint8_t modbusMessageCheck(uint16_t* message,uint16_t messageLength)
{
	int arrayIndex=0;
	uint32_t check_temp=0;
	uint32_t check_CRC;
	for(arrayIndex=0;arrayIndex<messageLength-2;arrayIndex++)
  {check_temp=(check_temp<<8)+message[arrayIndex];}
	check_CRC=message[arrayIndex++];
	check_CRC=check_CRC+(message[arrayIndex++]<<8);
	
	if(CRC16_MODBUS(message,arrayIndex))
		return 1;
	else
		return 0;
}

void light_modbusMessageCreate(uint16_t *message)
{
	 int arrayIndex=0; 
	 uint16_t tempCRC=0;
   message[arrayIndex++]=ADDRESS_5;
	 message[arrayIndex++]=ASK;
	 message[arrayIndex++]=(LIGHT_REG_ADDR&0xFF00)>>8;
	 message[arrayIndex++]=(LIGHT_REG_ADDR&0x00FF);
	 message[arrayIndex++]=(LIGHT_RET_LENGTH&0xFF00)>>8;
	 message[arrayIndex++]=(LIGHT_RET_LENGTH&0x00FF);
	 tempCRC=CRC16_MODBUS(message,arrayIndex);
	 tempCRC=tempCRC&0xFFFF;
	 message[arrayIndex++]=tempCRC&0x00FF;
	 message[arrayIndex++]=(tempCRC&0xFF00)>>8;

}


void pressure_modbusMessageCreate(uint16_t *message)
{
	 int arrayIndex=0; 
	 uint16_t tempCRC=0;
   message[arrayIndex++]=ADDRESS_2;
	 message[arrayIndex++]=ASK;
	 message[arrayIndex++]=(PRESSURE_REG_ADDR&0xFF00)>>8;
	 message[arrayIndex++]=(PRESSURE_REG_ADDR&0x00FF);
	 message[arrayIndex++]=(PRESSURE_RET_LENGTH&0xFF00)>>8;
	 message[arrayIndex++]=(PRESSURE_RET_LENGTH&0x00FF);
	 tempCRC=CRC16_MODBUS(message,arrayIndex);
	 tempCRC=tempCRC&0xFFFF;
	 message[arrayIndex++]=tempCRC&0x00FF;
	 message[arrayIndex++]=(tempCRC&0xFF00)>>8;

}

void soilPH_modbusMessageCreate(uint16_t *message)
{
	 int arrayIndex=0; 
	 uint16_t tempCRC=0;
   message[arrayIndex++]=ADDRESS_4;
	 message[arrayIndex++]=ASK;
	 message[arrayIndex++]=(SOILPH_REG_ADDR&0xFF00)>>8;
	 message[arrayIndex++]=(SOILPH_REG_ADDR&0x00FF);
	 message[arrayIndex++]=(SOILPH_RET_LENGTH&0xFF00)>>8;
	 message[arrayIndex++]=(SOILPH_RET_LENGTH&0x00FF);
	 tempCRC=CRC16_MODBUS(message,arrayIndex);
	 tempCRC=tempCRC&0xFFFF;
	 message[arrayIndex++]=tempCRC&0x00FF;
	 message[arrayIndex++]=(tempCRC&0xFF00)>>8;

}

void conductivity_modbusMessageCreate(uint16_t *message)
{
	 int arrayIndex=0; 
	 uint16_t tempCRC=0;
   message[arrayIndex++]=ADDRESS_6;
	 message[arrayIndex++]=ASK;
	 message[arrayIndex++]=(CONDUCTIVITY_REG_ADDR&0xFF00)>>8;
	 message[arrayIndex++]=(CONDUCTIVITY_REG_ADDR&0x00FF);
	 message[arrayIndex++]=(CONDUCTIVITY_RET_LENGTH&0xFF00)>>8;
	 message[arrayIndex++]=(CONDUCTIVITY_RET_LENGTH&0x00FF);
	 tempCRC=CRC16_MODBUS(message,arrayIndex);
	 tempCRC=tempCRC&0xFFFF;
	 message[arrayIndex++]=tempCRC&0x00FF;
	 message[arrayIndex++]=(tempCRC&0xFF00)>>8;

}


