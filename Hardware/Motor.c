#include "all header.h"

void Motor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

    PWM_Init();
}

void Motor_SetSpeed(int8_t Speed)//控制正反速度的变量，负为反方向
{
    if(Speed>=0)//正转逻辑
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_4);//这两步是设置方向
        GPIO_ReSetBits(GPIOA,GPIO_Pin_5);//设置一个正，一个负，是谁并不重要，关键在一正一负
        PWM_SetCompare3(Speed);
    }
    else
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_5);//反转
        GPIO_ReSetBits(GPIOA,GPIO_Pin_4);
        PWM_SetCompare3(-Speed);//传入必须要传入整数，此处Speed为负数，所以加负号
    }
}