/*****************************************************************************

* File Name        : main.c

* Description      : Buton ve LED uyguamasý

********************************************************************************

* Additional Information : SYSTICK ile Buton ve LED uygulaması

*******************************************************************************/
#include "stm32f0xx_conf.h"
#include "stdbool.h"

#define Button      GPIO_Pin_0
#define LED_Blue    GPIO_Pin_8
#define LED_Green   GPIO_Pin_9

void Delay(uint32_t);
void GPIO_IO_Init(void);
void LED_Change(bool, uint16_t);

static int timerValue;
static bool LED = false;
static bool LED_State = false;
static bool Button_State = false;

/***********************************************************
* Function Name  : main
* Description    : Ana fonksiyondur.Program burada döner.
* Input          : int
* Return         : void
***********************************************************/
int main(void){
    GPIO_IO_Init();
    TIM2_Init();

    while(1){

        if(GPIO_ReadInputDataBit(GPIOA,Button)){
            if(Button_State){
                LED = true;
                Button_State = false;
            }
            else{
                LED = false;
                Button_State = true;
            }

        }
        timerValue = TIM_GetCounter(TIM2);
        if (timerValue == 500)
            LED_State = true;
        else if(timerValue == 1000)
            LED_State = false;

        if(LED){
            if(GPIO_ReadOutputDataBit(GPIOC, LED_Green))
                GPIO_ResetBits(GPIOC, LED_Green);
            LED_Change(LED_State, LED_Blue);
        }
        else{
            if(GPIO_ReadOutputDataBit(GPIOC, LED_Blue))
                GPIO_ResetBits(GPIOC, LED_Blue);
            LED_Change(LED_State, LED_Green);
        }
    }
}

/***********************************************************
* Function Name  : LED_ChangeState
* Description    : Giriş bool'una göre LED durumunu değiştirir
* Input          : bool
* Return         : void
***********************************************************/
void LED_Change(bool state, uint16_t GPIO_Pin){
    if(state)
        GPIO_SetBits(GPIOC, GPIO_Pin);
    else
        GPIO_ResetBits(GPIOC, GPIO_Pin);
}

/***********************************************************
* Function Name  : GPIO_IO_Init
* Description    : GPIO Pinlerini hazırlar
* Input          : void
* Return         : void
***********************************************************/
void GPIO_IO_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);

    GPIO_InitStructure.GPIO_Pin = Button;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;

    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_Green | LED_Blue;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;

    GPIO_Init(GPIOC,&GPIO_InitStructure);
}

/***********************************************************
* Function Name  : TIM2_Init
* Description    : Timer ayarları yapılır.
* Input          : void
* Return         : void
***********************************************************/
void TIM2_Init(void){
    TIM_TimeBaseInitTypeDef Timer_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    Timer_InitStructure.TIM_Prescaler = 100000;
    Timer_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    Timer_InitStructure.TIM_Period = 1000;
    Timer_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    Timer_InitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &Timer_InitStructure);
    TIM_Cmd(TIM2, ENABLE);
}

/***********************************************************
* Function Name  : Delay
* Description    : Ýnput kadar bekler
* Input          : uint32_t
* Return         : void
***********************************************************/
void Delay(__IO uint32_t nCount){
    while(nCount--){
    }
}

