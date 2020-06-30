#ifndef _CRC16_H
#define _CRC16_H
#include <stdint.h>
void InvertUint8(unsigned char *DesBuf, unsigned char *SrcBuf);
void InvertUint16(unsigned short *DesBuf, unsigned short *SrcBuf)  ;
unsigned short CRC16_MODBUS(uint16_t *puchMsg, unsigned int usDataLen)  ;
#endif

