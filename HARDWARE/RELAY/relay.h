
#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x_gpio.h"
#include "sys.h"

#define RELAY PBout(2)


void Relay_Init (void);


#endif



