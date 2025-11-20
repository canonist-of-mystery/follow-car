#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // Enable clock for TIM2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);


    TIM_InternalClockConfig(TIM2); // Use internal clock
    //set timebase
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=100-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse=0;         
    TIM_OC1Init(TIM2,&TIM_OCInitStructure);
    TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	     

    TIM_Cmd(TIM2,ENABLE); // Start TIM2

//TIM3_Init
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // Enable clock for TIM3
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);


    TIM_InternalClockConfig(TIM3); // Use internal clock
    //set timebase
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period=100-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse=0;         
    TIM_OC1Init(TIM3,&TIM_OCInitStructure);
    TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	     

    TIM_Cmd(TIM3,ENABLE); // Start TIM3

}

void PWM_SetCompare1_left(uint8_t Compare)
{
	TIM_SetCompare1(TIM2,Compare);
}

void PWM_SetCompare2_left(uint8_t Compare)
{
	TIM_SetCompare2(TIM2,Compare);
}

void PWM_SetCompare1_right(uint8_t Compare)
{
    TIM_SetCompare1(TIM3,Compare);
}

void PWM_SetCompare2_right(uint8_t Compare)
{
    TIM_SetCompare2(TIM3,Compare);
}