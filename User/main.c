#include "all header.h"
 
uint8_t KeyNum;
int8_t Speed;
uint8_t ifstart=0; 
uint8_t mode;

int main(void)
{
	all init();

	while(1)
	{
		if(Key_Check(KEY_1,KEY_SINGLE))
		{
			if(ifstart==0)//是否发车判断为0时
			{
				ifstart=1;//改为发车信号
				mode=straight;//直行模式
				sensor_control();//直行运动
			}else if(ifstart==1)//是否发车判断为1时
			{
				ifstart=0;//改为停车信号
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