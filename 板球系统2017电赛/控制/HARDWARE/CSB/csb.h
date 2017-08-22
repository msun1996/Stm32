#ifndef _CSB_H
#define _CSB_H

#include "sys.h"
#include "delay.h"
#include "csb.h"
#include "oled.h"


#define TRIG_Send  PBout(8)
#define ECHO_Reci  PBin(9)

void  CH_SR04_Init(void);

#endif
