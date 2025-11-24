#include "all header.h"

uint8_t left1;
uint8_t left2;
uint8_t right1;
uint8_t right2;
extern uint8_t mode;
extern uint8_t straight;
uint8_t turnleft;
uint8_t turnright;
uint8_t leftvertical;
uint8_t rightvertical;
extern uint8_t stop;

#define straight 1
#define turnleft 2
#define turnright 3
#define leftvertical 4
#define rightvertical 5
#define stop 6

void sensor_init(void)//红外模块用到的四个引脚的初始化
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 |GPIO_Pin_12;      // 建议新手只打开需要的引脚，避免影响其它外设
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13 |GPIO_Pin_14;      // 建议新手只打开需要的引脚，避免影响其它外设
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t sensor_check(void)//设置红外模块的状态
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
        mode=straight;//直行
    }
    else if(left1==0 && left2==1 && right1==0 && right2==0)
    {
        mode=turnleft;//左转
    }else if(left1==0 && left2==0 && right1==1 && right2==0)
    {
        mode=turnright;//右转
    }else if(left1==1 && left2==1 && right1==1 && right2==0)
    {
        mode=leftvertical;//左直角
    }else if(left1==0 && left2==1 && right1==1 && right2==1)
    {
        mode=rightvertical;//右直角
    }
    
    return mode;
}

void sensor_control(void)//根据红外模块的状态控制小车运动
{
    mode = sensor_check();
    
    switch(mode)
    {
        case straight:
            car_go_straight();
            break;
        case turnleft:
            car_turn_left();
            break;
        case turnright:
            car_turn_right();
            break;
        case leftvertical:
            car_left_vertical();
            break;
        case rightvertical:
            car_right_vertical();
            break;
        case stop:
            car_stop();
            break;
        default:
            car_go_straight();
            break;
    }
}