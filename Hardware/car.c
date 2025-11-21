#include "stm32f10x.h"

void car_go_straight(uint8_t speed)//小车直行
{
    // 左前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 设置 PB3 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);     // 将 PB4 清零（反向引脚置低）
    PWM_SetCompare1(speed);           // 将速度幅值作为 PWM 占空比（通道 3）

    // 左后轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_5);       // 设置 PB5 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);     // 将 PB6 清零（反向引脚置低）
    PWM_SetCompare2(speed);           // 将速度幅值作为 PWM 占空比（通道 4）

    // 右前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_10);      // 设置 PB10 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);    // 将 PB11 清零（反向引脚置低）    
    PWM_SetCompare3(speed);           // 将速度幅值作为 PWM 占空比（通道 3）

    // 右后轮正转
    GPIO_SetBits(GPIOA, GPIO_Pin_9);       // 设置 PA9 为高电平（正向）
    GPIO_ResetBits(GPIOA, GPIO_Pin_10);    // 将 PA10 清零（反向引脚置低）
    PWM_SetCompare4(speed);           // 将速度幅值作为 PWM 占空比（通道 4）
}

void car_turn_left(uint8_t speed)//小车左转
{
    // 左前轮停止
    PWM_SetCompare1(0);           // 将 PWM 占空比设为 0（停止）

    // 左后轮停止
    PWM_SetCompare2(0);           // 将 PWM 占空比设为 0（停止）

    // 右前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_10);      // 设置 PB10 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);    // 将 PB11 清零（反向引脚置低）    
    PWM_SetCompare3(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 3）

    // 右后轮正转
    GPIO_SetBits(GPIOA, GPIO_Pin_9);       // 设置 PA9 为高电平（正向）
    GPIO_ResetBits(GPIOA, GPIO_Pin_10);    // 将 PA10 清零（反向引脚置低）
    PWM_SetCompare4(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 4）
}

void car_turn_right(uint8_t speed)//小车右转
{
    // 右前轮停止
    PWM_SetCompare3(0);           // 将 PWM 占空比设为 0（停止）

    // 右后轮停止
    PWM_SetCompare4(0);           // 将 PWM 占空比设为 0（停止）

    // 左前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 设置 PB3 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);     // 将 PB4 清零（反向引脚置低）
    PWM_SetCompare1(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 1）

    // 左后轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_5);       // 设置 PB5 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);     // 将 PB6 清零（反向引脚置低）
    PWM_SetCompare2(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 2）
}

void car_left_vertical(uint8_t speed)//小车左直角
{
    // 左前轮反转
    GPIO_SetBits(GPIOB, GPIO_Pin_4);       // 设置 PB4 为高电平（反向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_3);     // 将 PB3 清零（正向引脚置低）
    PWM_SetCompare1(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 1）

    // 左后轮反转
    GPIO_SetBits(GPIOB, GPIO_Pin_6);       // 设置 PB6 为高电平（反向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);     // 将 PB5 清零（正向引脚置低）
    PWM_SetCompare2(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 2）

    // 右前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_10);      // 设置 PB10 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);    // 将 PB11 清零（反向引脚置低）    
    PWM_SetCompare3(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 3）

    // 右后轮正转
    GPIO_SetBits(GPIOA, GPIO_Pin_9);       // 设置 PA9 为高电平（正向）
    GPIO_ResetBits(GPIOA, GPIO_Pin_10);    // 将 PA10 清零（反向引脚置低）
    PWM_SetCompare4(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 4）
}

void car_right_vertical(uint8_t speed)//小车右直角
{
    // 右前轮反转
    GPIO_SetBits(GPIOB, GPIO_Pin_11);      // 设置 PB11 为高电平（反向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);    // 将 PB10 清零（正向引脚置低）    
    PWM_SetCompare3(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 3）

    // 右后轮反转
    GPIO_SetBits(GPIOA, GPIO_Pin_10);      // 设置 PA10 为高电平（反向）
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);     // 将 PA9 清零（正向引脚置低）
    PWM_SetCompare4(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 4）

    // 左前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 设置 PB3 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);     // 将 PB4 清零（反向引脚置低）
    PWM_SetCompare1(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 1）

    // 左后轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_5);       // 设置 PB5 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);     // 将 PB6 清零（反向引脚置低）
    PWM_SetCompare2(80);          // 设置适当的速度幅值作为 PWM 占空比（通道 2）
}

void car_stop(void)//小车停止
{
    PWM_SetCompare1(0);           // 将 PWM 占空比设为 0（停止）
    PWM_SetCompare2(0);           // 将 PWM 占空比设为 0（停止）
    PWM_SetCompare3(0);           // 将 PWM 占空比设为 0（停止）
    PWM_SetCompare4(0);           // 将 PWM 占空比设为 0（停止）
}