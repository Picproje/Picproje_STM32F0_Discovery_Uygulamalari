/*****************************************************************************

* File Name        : main.h

* Description      : Header file for main.c

********************************************************************************

* Additional Information : Fonksiyon yapıları ve global-local değişkenleri barındırır

*******************************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

#define LED_Green   GPIO_Pin_9

extern uint32_t Buton_State;

void CPU_Init(void);

#endif
