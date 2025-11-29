#include "all header.h"

void car_go_straight(void)//小车直行
{
    // 左前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 设置 PB4 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);     // 将 PB3 清零（反向引脚置低）
    PWM_SetCompare1(83);           // 将速度幅值作为 PWM 占空比（通道 1）

    // 左后轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_5);       // 设置 PB5 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);     // 将 PB13 清零（反向引脚置低）
    PWM_SetCompare2(83);           // 将速度幅值作为 PWM 占空比（通道 2）

    // 右前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_11);      // 设置 PB11 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);    // 将 PB10 清零（反向引脚置低)    
    PWM_SetCompare3(83);            // 将速度幅值作为 PWM 占空比（通道 3）

    // 右后轮正转
    GPIO_SetBits(GPIOA, GPIO_Pin_10);       // 设置 PA10 为高电平（正向）
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);    // 将 PA9 清零（反向引脚置低）
    PWM_SetCompare4(83);           // 将速度幅值作为 PWM 占空比（通道 4）
}

void car_turn_left(void)//小车左转
{
    // 左前轮停止
    PWM_SetCompare1(5);           // 将 PWM 占空比设为 0（停止）

    // 左后轮停止
    PWM_SetCompare2(5);           // 将 PWM 占空比设为 0（停止）

    // 右前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_11);      // 设置 PB11 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);    // 将 PB10 清零（反向引脚置低）    
    PWM_SetCompare3(70);          // 设置适当的速度幅值作为 PWM 占空比（通道 3）

    // 右后轮正转
    GPIO_SetBits(GPIOA, GPIO_Pin_10);       // 设置 PA10 为高电平（正向）
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);    // 将 PA9 清零（反向引脚置低）
    PWM_SetCompare4(70);          // 设置适当的速度幅值作为 PWM 占空比（通道 4）
}

void car_turn_right(void)//小车右转
{
    // 右前轮停止
    PWM_SetCompare3(5);           // 将 PWM 占空比设为 0（停止）

    // 右后轮停止
    PWM_SetCompare4(5);           // 将 PWM 占空比设为 0（停止）

    // 左前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 设置 PB3 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);     // 将 PB4 清零（反向引脚置低）
    PWM_SetCompare1(70);          // 设置适当的速度幅值作为 PWM 占空比（通道 1）

    // 左后轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_5);       // 设置 PB5 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);     // 将 PB13 清零（反向引脚置低）
    PWM_SetCompare2(70);          // 设置适当的速度幅值作为 PWM 占空比（通道 2）
}

void car_left_vertical(void)//小车左直角
{
    // 左前轮正转 - 增加制动力
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 正转方向
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    PWM_SetCompare1(5);          // 增加正转力度

    // 左后轮正转 - 增加制动力
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    PWM_SetCompare2(5);          // 增加正转力度

    // 右前轮正转 - 提高速度
    GPIO_SetBits(GPIOB, GPIO_Pin_11);
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);    
    PWM_SetCompare3(100);          // 提高速度

    // 右后轮正转 - 提高速度
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);
    PWM_SetCompare4(100);          // 提高速度
}

void car_right_vertical(void)//小车右直角
{
    // 右前轮反转 - 增加制动力
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);
    GPIO_SetBits(GPIOB, GPIO_Pin_11);
    PWM_SetCompare3(5);          // 增加正转力度

    // 右后轮反转 - 增加制动力
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
    PWM_SetCompare4(5);          // 增加正转力度

    // 左前轮正转 - 提高速度
    GPIO_SetBits(GPIOB, GPIO_Pin_3);
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    PWM_SetCompare1(100);          // 提高速度

    // 左后轮正转 - 提高速度
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    PWM_SetCompare2(100);          // 提高速度
}
void car_corleft(void)
{
    // 左前轮减速
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 设置 PB4 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);     // 将 PB3 清零（反向引脚置低）
    PWM_SetCompare1(5);           // 将速度幅值作为 PWM 占空比（通道 1）

    // 左后轮减速
    GPIO_SetBits(GPIOB, GPIO_Pin_5);       // 设置 PB5 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);     // 将 PB13 清零（反向引脚置低）
    PWM_SetCompare2(5);           // 将速度幅值作为 PWM 占空比（通道 2）

    // 右前轮正常速度
    GPIO_SetBits(GPIOB, GPIO_Pin_11);      // 设置 PB11 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);    // 将 PB10 清零（反向引脚置低)    
    PWM_SetCompare3(30);            // 将速度幅值作为 PWM 占空比（通道 3）

    // 右后轮正常速度
    GPIO_SetBits(GPIOA, GPIO_Pin_10);       // 设置 PA10 为高电平（正向）
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);    // 将 PA9 清零（反向引脚置低）
    PWM_SetCompare4(30);           // 将速度幅值作为 PWM 占空比（通道 4）
}
void car_corright(void)
{
    // 右前轮减速
    GPIO_SetBits(GPIOB, GPIO_Pin_11);      // 设置 PB11 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);    // 将 PB10 清零（反向引脚置低)    
    PWM_SetCompare3(5);            // 将速度幅值作为 PWM 占空比（通道 3）

    // 右后轮减速
    GPIO_SetBits(GPIOA, GPIO_Pin_10);       // 设置 PA10 为高电平（正向）
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);    // 将 PA9 清零（反向引脚置低）
    PWM_SetCompare4(5);           // 将速度幅值作为 PWM 占空比（通道 4）

    // 左前轮正常速度
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 设置 PB3 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);     // 将 PB4 清零（反向引脚置低）
    PWM_SetCompare1(30);           // 将速度幅值作为 PWM 占空比（通道 1）

    // 左后轮正常速度
    GPIO_SetBits(GPIOB, GPIO_Pin_5);       // 设置 PB5 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);     // 将 PB13 清零（反向引脚置低）
    PWM_SetCompare2(30);           // 将速度幅值作为 PWM 占空比（通道 2）
}
void car_stop(void)//小车停止
{
    PWM_SetCompare1(0);           // 将 PWM 占空比设为 0（停止）
    PWM_SetCompare2(0);           // 将 PWM 占空比设为 0（停止）
    PWM_SetCompare3(0);           // 将 PWM 占空比设为 0（停止）
    PWM_SetCompare4(0);           // 将 PWM 占空比设为 0（停止）
}
void car_speed(void)
{
     // 左前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 设置 PB4 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);     // 将 PB3 清零（反向引脚置低）
    PWM_SetCompare1(96);           // 将速度幅值作为 PWM 占空比（通道 1）

    // 左后轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_5);       // 设置 PB5 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);     // 将 PB13 清零（反向引脚置低）
    PWM_SetCompare2(96);           // 将速度幅值作为 PWM 占空比（通道 2）

    // 右前轮正转
    GPIO_SetBits(GPIOB, GPIO_Pin_11);      // 设置 PB11 为高电平（正向）
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);    // 将 PB10 清零（反向引脚置低)    
    PWM_SetCompare3(96);            // 将速度幅值作为 PWM 占空比（通道 3）
    // 右后轮正转
    GPIO_SetBits(GPIOA, GPIO_Pin_10);       // 设置 PA10 为高电平（正向）
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);    // 将 PA9 清零（反向引脚置低）
    PWM_SetCompare4(96);           // 将速度幅值作为 PWM 占空比（通道 4）
}