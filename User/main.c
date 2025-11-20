#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
 
unit8_t KeyNum;
int8_t Speed;

int main(void)
{
	OLED_Init();
	Motor_Init();//初始化
	Key_Init();
	OLED_Init();

	OLED_ShowString(1,1,"Speed:");

	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)
		{
			Speed += 20;
			if(Angle>100)//到100变回-100
			{
				Speed=-100;
			}
		}
		Motor_SetSpeed(Speed);//输入速度即可
		OLED_ShowSignedNum(1,7,Speed,3);//OLED屏幕显示
	}
}

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
	{
		Key_Tick();
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}