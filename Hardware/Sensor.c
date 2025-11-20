#include "stm32f10x.h"

void sensor_init(void)//红外模块用到的四个引脚的初始化
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 |GPIO_Pin_12;      // 建议新手只打开需要的引脚，避免影响其它外设
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13 |GPIO_Pin_14;      // 建议新手只打开需要的引脚，避免影响其它外设
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void sensor_check(void)//设置红外模块的状态
{
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == 0)
    {
        left1 = 0;
    }else
    {
        left1 = 1;
    }

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0)
    {
        left2 = 0;
    }else
    {
        left2 = 1;
    }

    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
    {
        right1 = 0;
    }else
    {
        right1 = 1;
    }
    
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
    {
        right2 = 0;
    }else
    {
        right2 = 1;
    }

    if(left1==0 && left2==1 && right1==1 && right2==0)
    {
        mode=straight;//检测到黑线
    }
    else if(left1==0 && left2==1 && right1==0 && right2==0)
    {
        mode=turnleft;//检测到黑线
    }else if(left1==0 && left2==0 && right1==1 && right2==0)
    {
        mode=turnright;//检测到黑线
    }else if(left1==1 && left2==1 && right1==1 && right2==0)
    {
        mode=leftvertical;//检测到黑线
    }else if(left1==0 && left2==1 && right1==1 && right2==1)
    {
        mode=rightvertical;//检测到黑线
    }
    
}