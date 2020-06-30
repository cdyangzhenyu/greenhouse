#ifndef __ZIGBEE_H
#define __ZIGBEE_H
#include <stdint.h>
extern uint16_t air_temperature,air_light,air_co2,air_pressure,soil_temperature,soil_water,soil_conductivity,soil_PH,air_humidity,soil_TDS; 
uint16_t zigbeeMessageCreate(uint8_t* zigbeeMessage,uint8_t* info,int infolength);
uint16_t  zigbeeInfoCreate(uint8_t* info);
uint16_t  zigbeeStatusCreate(uint8_t* zigbeeMessage,uint16_t outPort);
#endif
