#include "all header.h"

#define KEY_PRESSED 1
#define KEY_UNPRESSED 0

#define KEY_DEBOUNCE_TIME 20

volatile uint8_t g_key_pressed = 0;  // 按键按下标志

// #define KEY_HOLD         0x01//按住不放
// #define KEY_DOWN         0x02//按下时刻
// #define KEY_UP           0x04//松开时刻
// #define KEY_SINGLE       0x08//单击
// #define KEY_DOUBLE       0x10//双击
// #define KEY_LONG         0x20//长按
// #define KEY_REPEAT       0x40//重复

//#define KEY_TIME_DOUBLE 200
// #define KEY_TIME_LONG 2000
// #define KEY_TIME_REPEAT 100

//#define KEY_COUNT     4
// #define KEY_UP        0
// #define KEY_DOWN      1
// #define KEY_ENTER     2
// #define KEY_BACK      3


// uint8_t Key_Flag[KEY_COUNT];

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t Key_GetState(void)
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		return KEY_PRESSED;
	}else
	return KEY_UNPRESSED;
}

// uint8_t Key_Check(uint8_t Flag)
// {
// 	if (Key_Flag[n] & Flag)
// 	{
// 		if (Flag != KEY_HOLD)
// 		{
// 			Key_Flag[n] &= ~Flag;
// 		}
// 		return 1;
// 	}
// 	return 0;
// }

// void Key_Tick(void)
// {
// 	static uint8_t Count, i;
// 	static uint8_t CurrState[KEY_COUNT], PrevState[KEY_COUNT];
// 	static uint8_t S[KEY_COUNT];
// 	static uint16_t Time[KEY_COUNT];
	
// 	for (i = 0; i < KEY_COUNT; i ++)
// 	{
// 		if (Time[i] > 0)
// 		{
// 			Time[i] --;
// 		}
// 	}
	
// 	Count ++;
// 	if (Count >= 20)
// 	{
// 		Count = 0;
		
// 		for (i = 0; i < KEY_COUNT; i ++)
// 		{
// 			PrevState[i] = CurrState[i];
// 			CurrState[i] = Key_GetState(i);
			
// 			if (CurrState[i] == KEY_PRESSED)
// 			{
// 				Key_Flag[i] |= KEY_HOLD;
// 			}
// 			else
// 			{
// 				Key_Flag[i] &= ~KEY_HOLD;
// 			}
			
// 			if (CurrState[i] == KEY_PRESSED && PrevState[i] == KEY_UNPRESSED)
// 			{
// 				Key_Flag[i] |= KEY_DOWN;
// 			}
			
// 			if (CurrState[i] == KEY_UNPRESSED && PrevState[i] == KEY_PRESSED)
// 			{
// 				Key_Flag[i] |= KEY_UP;
// 			}
			
// 			if (S[i] == 0)
// 			{
// 				if (CurrState[i] == KEY_PRESSED)
// 				{
// 					Time[i] = KEY_TIME_LONG;
// 					S[i] = 1;
// 				}
// 			}
// 			else if (S[i] == 1)
// 			{
// 				if (CurrState[i] == KEY_UNPRESSED)
// 				{
// 					Time[i] = KEY_TIME_DOUBLE;
// 					S[i] = 2;
// 				}
// 				else if (Time[i] == 0)
// 				{
// 					Time[i] = KEY_TIME_REPEAT;
// 					Key_Flag[i] |= KEY_LONG;
// 					S[i] = 4;
// 				}
// 			}
// 			else if (S[i] == 2)
// 			{
// 				if (CurrState[i] == KEY_PRESSED)
// 				{
// 					Key_Flag[i] |= KEY_DOUBLE;
// 					S[i] = 3;
// 				}
// 				else if (Time[i] == 0)
// 				{
// 					Key_Flag[i] |= KEY_SINGLE;
// 					S[i] = 0;
// 				}
// 			}
// 			else if (S[i] == 3)
// 			{
// 				if (CurrState[i] == KEY_UNPRESSED)
// 				{
// 					S[i] = 0;
// 				}
// 			}
// 			else if (S[i] == 4)
// 			{
// 				if (CurrState[i] == KEY_UNPRESSED)
// 				{
// 					S[i] = 0;
// 				}
// 				else if (Time[i] == 0)
// 				{
// 					Time[i] = KEY_TIME_REPEAT;
// 					Key_Flag[i] |= KEY_REPEAT;
// 					S[i] = 4;
// 				}
// 			}
// 		}
// 	}
// }





// 全局变量


// /**
//   * @brief  简化的非阻塞按键扫描
//   */
// void Key_Tick(void)
// {
//     static uint8_t last_state = 1;  // 上次状态，假设默认高电平
//     static uint32_t press_time = 0;
//     uint8_t current_state = Key_GetState();
//     static uint8_t Count = 0;

// 	Count ++;
//     // 状态变化检测
//     if (current_state != last_state) {
//         last_state = current_state;
//         Count = 0;
//         // 如果是按下（从高到低）
//         if (current_state == 0) {
//             press_time = Get_SystemTick();  // 记录按下时间
//         }
//         // 如果是释放（从低到高）
//         else {
//             // 检查按下时间是否超过消抖时间
//             if ((Get_SystemTick() - press_time) > KEY_TIME_DOUBLE ) {
//                 g_key_pressed = 1;  // 设置按键标志
//             }
//         }
//     }
// }

void Key_Tick(void)
{
    static uint8_t last_state = 1;
    static uint8_t count = 0;
    static uint8_t was_pressed = 0;  // 记录之前是否有效按下
    uint8_t current_state = Key_GetState();

    if (current_state != last_state) {
        last_state = current_state;
        count = 0;
    } else {
        count++;
    }

    // 按下消抖确认
    if (current_state == 0 && count == KEY_DEBOUNCE_TIME) {
        was_pressed = 1;  // 标记有效按下
    }
    
    // 释放检测
    if (current_state == 1 && was_pressed && count > KEY_DEBOUNCE_TIME) {
        g_key_pressed = 1;
        was_pressed = 0;  // 重置标记
    }
}

