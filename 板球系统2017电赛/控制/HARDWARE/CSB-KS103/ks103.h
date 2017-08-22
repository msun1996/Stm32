#ifndef __KS103_H
#define __KS103_H

#include "sys.h" 

void KS103_Init(void);
u8 KS103_ReadOneByte(u8 address, u8 reg);
void KS103_WriteOneByte(u8 address,u8 reg,u8 command);

void KS103_Send(void);
u16 KS103_Get_Date(void);

#endif
