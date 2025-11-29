#include "all header.h"
 
#define KEY_PRESSED 1
#define KEY_UNPRESSED 0

volatile uint8_t ifstart=0; 
uint8_t mode;
static uint8_t KeyNum=0;

#define straight 1
#define turnleft 2
#define turnright 3
#define leftvertical 4
#define rightvertical 5
#define corleft 6
#define corright 7
#define stop 8
#define straight_fast 9  // 新增：直行加速模式

// 添加全局变量来跟踪连续直行次数
static uint8_t consecutive_straight_count = 0;

int main(void)
{
	all_init();
	mode=stop;//初始状态为停车
	OLED_ShowString(3, 1, "Follow Car");
	car_stop();
	
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if (KeyNum == 1)
		{
			ifstart=1-ifstart;
		}
		if(ifstart==0)//是否发车判断为0时
			{
				OLED_ShowString(1, 1, "     ");
				OLED_ShowString(2, 1, "Stop ");
				mode=stop;//停车模式
				car_stop();//停车静止
			}else if(ifstart==1)//是否发车判断为1时
			{
				OLED_ShowString(2, 1, "     ");
				OLED_ShowString(1, 1, "Start");
			}
			//  if(mode=leftvertical)
			// {
			// car_left_vertical();
			// }
			//  if(mode=rightvertical)
			// {
			// car_right_vertical();
			// }
		Delay_ms(50);
	}
}

void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
	{
		if(ifstart==1)
		{
			// uint8_t new_mode = sensor_check(); // 先读取新模式
			
			// // 连续直行检测逻辑
			// if(new_mode == straight)
			// {
			// 	consecutive_straight_count++;
			// 	// 如果连续2次检测到直行，则切换到加速模式
			// 	if(consecutive_straight_count >= 50)
			// 	{
			// 		mode = straight_fast;
			// 	}
			// 	else
			// 	{
			// 		mode = straight;
			// 	}
			// }
			// else
			// {
			// 	// 如果不是直行模式，重置计数器
			// 	consecutive_straight_count = 0;
			// 	mode = new_mode;
			// }
			mode= sensor_check();
			sensor_control(mode);
			
		}
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}


