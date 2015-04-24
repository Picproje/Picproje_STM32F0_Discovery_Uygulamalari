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
* Description    : Ana program burada doner
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
    NVIC_InitTypeDef  NVIC_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

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

    TIM_TimeBaseInitStruct.TIM_Prescaler = 0xFFFF;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    //TIM_TimeBaseInitStruct.TIM_Period = 3662;   // 5 saniye için
    TIM_TimeBaseInitStruct.TIM_Period = 732; //1 saniye için
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE ;

    NVIC_Init(&NVIC_InitStruct);

    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM6,ENABLE);
}

