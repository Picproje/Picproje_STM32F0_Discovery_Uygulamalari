/*****************************************************************************

* File Name        : main.c

* Description      : Timer6_Uygulaması

********************************************************************************

* Additional Information : Timer ile interrupt olusturma

*******************************************************************************/
#include "stm32f0xx_conf.h"
#include "main.h"

uint32_t Buton_State = 0;

/***********************************************************
* Function Name  : main
* Description    : Ana program burada döner
* Input          : void
* Return         : void
***********************************************************/
void main(void)
{
    CPU_Init();

    while(1)
    {
        if(Buton_State)
            GPIO_SetBits(GPIOC,LED_Green);
        else
            GPIO_ResetBits(GPIOC,LED_Green);
    }
}

void CPU_Init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOC,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOA,&GPIO_InitStructure);

    // Systic Klavuzu

  /* Setup SysTick Timer for 1 msec interrupts.
	 ------------------------------------------
	1. The SysTick_Config() function is a CMSIS function which configure:
	   - The SysTick Reload register with value passed as function parameter.
	   - Configure the SysTick IRQ priority to the lowest value (0x0F).
	   - Reset the SysTick Counter register.
	   - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
	   - Enable the SysTick Interrupt.
	   - Start the SysTick Counter.

	2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
	   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
	   SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
	   inside the misc.c file.

	3. You can change the SysTick IRQ priority by calling the
	   NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function
	   call. The NVIC_SetPriority() is defined inside the core_cm0.h file.

	4. To adjust the SysTick time base, use the following formula:

		 Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)

	   - Reload Value is the parameter to be passed for SysTick_Config() function
	   - Reload Value should not exceed 0xFFFFFF
   */

	/****************************************
	 *SystemFrequency/1000      1ms         *
	 *SystemFrequency/100000    10us        *
	 *SystemFrequency/1000000   1us         *
	 *****************************************/
    SysTick_Config(SystemCoreClock / 10);       //100 ms
}

