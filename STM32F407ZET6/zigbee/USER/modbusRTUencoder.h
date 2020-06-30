#ifndef _MODBUSRTUENCODER_H
#define _MODBUSRTUENCODER_H
#include <stdint.h>
uint8_t modbusMessageCheck(uint16_t* message,uint16_t messageLength);
void co2_modbusMessageCreate(uint16_t *message);
void light_modbusMessageCreate(uint16_t *message);
void pressure_modbusMessageCreate(uint16_t *message);
void soilPH_modbusMessageCreate(uint16_t *message);
void conductivity_modbusMessageCreate(uint16_t *message);
#endif
