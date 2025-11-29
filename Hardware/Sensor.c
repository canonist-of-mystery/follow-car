

// //1是指示灯灭，对应白色区域
// //0是指示灯亮，对应黑色区域

#include "all header.h"

uint8_t left1;
uint8_t left2;
uint8_t right1;
uint8_t right2;
extern uint8_t mode;
extern uint8_t straight;

uint8_t last_left1=0;
uint8_t last_left2=0;
uint8_t last_right1=0;
uint8_t last_right2=0;

#define straight 1
#define turnleft 2
#define turnright 3
#define leftvertical 4
#define rightvertical 5
#define corleft 6
#define corright 7
#define stop 8
#define straight_fast 9  // 直行加速模式


            // // 调整PD控制参数，增加转弯响应
            // #define KP 1.8f   // 增加比例系数
            // #define KD 1.5f   // 调整微分系数
            // #define BASE_SPEED 50  // 基础速度

            // // 传感器权重定义（从左到右：极左，左，右，极右）
            // #define WEIGHT_LEFT1  -2
            // #define WEIGHT_LEFT2  -1
            // #define WEIGHT_RIGHT1  1
            // #define WEIGHT_RIGHT2  2

            // // 添加PD控制相关变量
            // static int16_t last_error = 0;  // 上一次误差

    // 移动平均滤波配置
    #define FILTER_SIZE 2// 滤波窗口大小，可以调整（3-10之间）

    // 移动平均滤波数据结构
    typedef struct {
        uint8_t buffer[FILTER_SIZE];  // 数据缓冲区
        uint8_t index;                // 当前索引
        uint8_t count;                // 有效数据计数
        uint16_t sum;                 // 数据总和
    } MovingAverageFilter;

    // 为每个传感器创建滤波器
    MovingAverageFilter left1_filter = {0};
    MovingAverageFilter left2_filter = {0};
    MovingAverageFilter right1_filter = {0};
    MovingAverageFilter right2_filter = {0};

    // 移动平均滤波函数
    uint8_t moving_average_filter(MovingAverageFilter* filter, uint8_t new_value)
    {
        // 如果缓冲区已满，减去最旧的值
        if (filter->count == FILTER_SIZE) {
            filter->sum -= filter->buffer[filter->index];
        } else {
            filter->count++;
        }
        
        // 添加新值
        filter->buffer[filter->index] = new_value;
        filter->sum += new_value;
        
        // 更新索引
        filter->index = (filter->index + 1) % FILTER_SIZE;
        
        // 返回平均值（四舍五入）
        return (filter->sum + filter->count / 2) / filter->count;
    }


                // 修改calculate_error函数，返回误差值但不在函数内更新last_error
                // int16_t calculate_error(void)
                // {
                //     // 读取原始传感器数据并应用滤波
                //     uint8_t raw_left1 = (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1) ? 0 : 1;
                //     uint8_t raw_left2 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1) ? 0 : 1;
                //     uint8_t raw_right1 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 1) ? 0 : 1;
                //     uint8_t raw_right2 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 1) ? 0 : 1;
                    
                //     // 应用移动平均滤波
                //     left1 = moving_average_filter(&left1_filter, raw_left1);
                //     left2 = moving_average_filter(&left2_filter, raw_left2);
                //     right1 = moving_average_filter(&right1_filter, raw_right1);
                //     right2 = moving_average_filter(&right2_filter, raw_right2);
                    
                //     // 计算误差
                //     int16_t error = (left1 * WEIGHT_LEFT1) + (left2 * WEIGHT_LEFT2) + 
                //                 (right1 * WEIGHT_RIGHT1) + (right2 * WEIGHT_RIGHT2);
                    
                //     return error;
                // }


            // // 修改calculate_pd_control函数，增加输出范围
            // int16_t calculate_pd_control(void)
            // {
            //     int16_t current_error = calculate_error();
            //     int16_t error_diff = current_error - last_error;
            //     int16_t turn = (int16_t)(KP * current_error + KD * error_diff);
                
            //     // 限制转弯幅度在合理范围内
            //     if(turn > 80) turn = 80;
            //     if(turn < -80) turn = -80;
                
            //     last_error = current_error;
                
            //     return turn;
            // }

            // // 优化PD控制函数
            // int16_t calculate_pd_control(void)
            // {
            //     int16_t current_error = calculate_error();
            //     int16_t error_diff = current_error - last_error;
                
            //     // 根据误差大小动态调整系数
            //     float dynamic_kp = KP;
            //     float dynamic_kd = KD;
                
            //     // 大误差时增加响应性
            //     if(abs(current_error) > 3) {
            //         dynamic_kp = KP * 1.5f;
            //         dynamic_kd = KD * 1.2f;
            //     }
                
            //     int16_t turn = (int16_t)(dynamic_kp * current_error + dynamic_kd * error_diff);
                
            //     // 限制转弯幅度
            //     if(turn > 60) turn = 60;
            //     if(turn < -60) turn = -60;
                
            //     last_error = current_error;
                
            //     return turn;
            // }
void sensor_init(void)//红外模块用到的四个引脚的初始化
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t sensor_check(void)//设置红外模块的状态（带滤波）
{
    uint8_t mode;
    static uint8_t last_valid_mode = straight; // 保存上一次有效模式
    
    // 读取原始传感器数据并立即进行滤波
    uint8_t raw_left1 = (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1) ? 0 : 1;
    uint8_t raw_left2 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1) ? 0 : 1;
    uint8_t raw_right1 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 1) ? 0 : 1;
    uint8_t raw_right2 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 1) ? 0 : 1;
    
    // // 应用移动平均滤波
    // last_left1 = left1;
    // last_left2 = left2;
    // last_right1 = right1;
    // last_right2 = right2;

    left1 = moving_average_filter(&left1_filter, raw_left1);
    left2 = moving_average_filter(&left2_filter, raw_left2);
    right1 = moving_average_filter(&right1_filter, raw_right1);
    right2 = moving_average_filter(&right2_filter, raw_right2);
    
                // uint8_t left1 = (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1) ? 0 : 1;
                // uint8_t left2 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1) ? 0 : 1;
                // uint8_t right1 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 1) ? 0 : 1;
                // uint8_t right2 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 1) ? 0 : 1;

    // 首先检查是否全黑(0000)
    if(left1 == 0 && left2 == 0 && right1 == 0 && right2 == 0)
    {
        // 全黑时保持上一次的有效模式
        mode = last_valid_mode;
        return mode;
    }

    // 使用滤波后的数据进行模式判断（原有逻辑不变）
    if((left1==0 && left2==1 && right1==1 && right2==0)||
    (left1==1 && left2==1 && right1==1 && right2==1))
    {
        mode=straight;//直行
        last_valid_mode = mode; // 更新有效模式
    }
    
    // else if((left1==1 && left2==1 && right1==0 && right2==0))
    // {
    //     //mode=turnleft;//左转
    //     mode=leftvertical;//左直角
    //     last_valid_mode = mode; // 更新有效模式
    // }else if((left1==0 && left2==0 && right1==1 && right2==1))
    // {
    //     //mode=turnright;//右转
    //     mode=rightvertical;//右直角
    //     last_valid_mode = mode; // 更新有效模式
    // }
    else if((left1==1 && left2==0 && right1==0 && right2==0)||
    (left1==0 && left2==1 && right1==0 && right2==0)||
    (left1==1 && left2==1 && right1==0 && right2==0))
    {
        mode=leftvertical;//左直角
        last_valid_mode = mode; // 更新有效模式
    }else if((left1==0 && left2==0 && right1==0 && right2==1)||
    (left1==0 && left2==0 && right1==1 && right2==0)||
    (left1==0 && left2==0 && right1==1 && right2==1))
    {
        mode=rightvertical;//右直角
        last_valid_mode = mode; // 更新有效模式
    }
    // 在现有的判断条件后添加这些修正条件
    else if((left1==1 && left2==0 && right1==1 && right2==1)||
    (left1==0 && left2==1 && right1==0 && right2==1)||
    (left1==0 && left2==1 && right1==1 && right2==1))
    {
        mode=corright;//向右修正
        last_valid_mode = mode;
    }
    else if((left1==1 && left2==1 && right1==0 && right2==1)||
    (left1==1 && left2==0 && right1==1 && right2==0)||
    (left1==1 && left2==1 && right1==1 && right2==0))
    {
        mode=corleft;//向左修正
        last_valid_mode = mode;
    }
    else
    {
        // 其他未明确情况，保持上一次有效模式
        mode = last_valid_mode;
    }
    return mode;
}

void sensor_control(uint8_t mode)//根据红外模块的状态控制小车运动
{
    
    switch(mode)
    {
        case straight:
            car_go_straight();
            break;
        case straight_fast:      // 新增：直行加速模式
            car_speed();         // 调用加速函数
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
        case corleft:           // 添加向左修正
            car_corleft();
            break;
        case corright:          // 添加向右修正
            car_corright();
            break;
        default:
            car_go_straight();
            break;
    }
}
