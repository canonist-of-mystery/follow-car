#include "stm32f10x.h"                  

int main(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); // 开启 GPIOC 时钟

	
	GPIO_InitTypeDef GPIO_Initstructure; // 声明一个 GPIO 初始化结构体变量

	
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP; // 设置为推挽输出
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_13; // 选择引脚：GPIOC pin 13
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz; // 输出速率选择
	GPIO_Init(GPIOC,&GPIO_Initstructure); // 初始化 GPIOC 的 pin13
	GPIO_SetBits(GPIOC,GPIO_Pin_13); // 将 PC13 拉高
	
	while(1)
	{
		// 在这里放用户代码，例如：
		// GPIO_ResetBits(GPIOC,GPIO_Pin_13); // 点亮 LED（如果 LED 是低电平点亮）
		// delay_ms(500);
		// GPIO_SetBits(GPIOC,GPIO_Pin_13); // 熄灭 LED
		// delay_ms(500);
		
		// 当前示例保持空循环，避免跑到主函数外
	}

}
