#ifndef __LORA_H
#define __LORA_H
#include <stdint.h>
extern uint16_t air_temperature,air_light,air_co2,air_pressure,soil_temperature,soil_water,soil_conductivity,soil_PH,air_humidity,soil_TDS; 
uint16_t loraMessageCreate(uint8_t* zigbeeMessage,uint8_t* info,int infolength);
uint16_t  loraInfoCreate(uint8_t* info);
uint16_t  loraStatusCreate(uint8_t* zigbeeMessage,uint16_t outPort);
uint16_t  loraAllStatusCreate(uint8_t* zigbeeMessage);
#endif
