#include "all header.h"

#define KEY_PRESSED 0
#define KEY_UNPRESSED 1

// 非阻塞按键检测相关变量
static uint8_t key_debounce_counter = 0;
static uint8_t key_last_state = 1;     // 上次按键状态（默认未按下）
static uint8_t key_current_state = 1;  // 当前按键状态
static uint8_t key_pressed_event = 0;  // 按键按下事件标志

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//正常情况下为1电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 非阻塞按键扫描函数，需要在主循环或定时器中断中定期调用
void Key_ScanNonBlocking(void)
{
    // 读取当前按键状态
    key_current_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
    
    // 消抖处理
    if (key_current_state != key_last_state)
    {
        key_debounce_counter++;
        if (key_debounce_counter >= 4)  // 连续4次检测到状态变化（约20ms消抖）
        {
            key_last_state = key_current_state;
            
            // 检测下降沿（按键按下）
            if (key_current_state == KEY_PRESSED)
            {
                key_pressed_event = 1;  // 设置按键按下事件标志
            }
            
            key_debounce_counter = 0;
        }
    }
    else
    {
        key_debounce_counter = 0;  // 状态稳定，清零计数器
    }
}

// 获取按键事件（非阻塞）
uint8_t Key_GetNum(void)
{
    uint8_t event = key_pressed_event;
    key_pressed_event = 0;  // 读取后清除事件标志
    return event;
}

// 获取当前按键状态（非阻塞）
uint8_t Key_GetState(void)
{
    return key_current_state;
}