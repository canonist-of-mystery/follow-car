#include "all header.h"
 
#define KEY_PRESSED 1
#define KEY_UNPRESSED 0

uint8_t ifstart=0; 
uint8_t mode;
uint8_t straight;
uint8_t stop;

int main(void)
{
	all_init();

	while(1)
	{
		if (Key_GetState() == KEY_PRESSED)
		{
			if(ifstart==0)//是否发车判断为0时
			{
				ifstart=1;//改为发车信号
				OLED_ShowString(1, 1, "Start");
				mode=straight;//直行模式
				sensor_control();//直行运动
			}else if(ifstart==1)//是否发车判断为1时
			{
				ifstart=0;//改为停车信号
				OLED_ShowString(1, 1, "Stop");
				mode=stop;//停车模式
				sensor_control();//停车静止
			}
		}
		sensor_control();
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