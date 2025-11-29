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

//改进的滤波算法 

// 1. 卡尔曼滤波结构体（用于高精度滤波）
typedef struct {
    float q;        // 过程噪声协方差
    float r;        // 测量噪声协方差
    float x;        // 系统状态估计值
    float p;        // 估计误差协方差
    float k;        // 卡尔曼增益
} KalmanFilter;

// 2. 指数加权移动平均结构体（用于快速响应）
typedef struct {
    float alpha;    // 平滑因子 (0-1)，越小滤波越强
    float value;    // 当前滤波值
} ExponentialFilter;

// 为每个传感器创建滤波器
KalmanFilter left1_kalman = {0.1, 0.1, 0.5, 1.0, 0};  // 初始状态为0.5
KalmanFilter left2_kalman = {0.1, 0.1, 0.5, 1.0, 0};
KalmanFilter right1_kalman = {0.1, 0.1, 0.5, 1.0, 0};
KalmanFilter right2_kalman = {0.1, 0.1, 0.5, 1.0, 0};

ExponentialFilter left1_exp = {0.5, 0.5};  // 提高alpha到0.7，响应更快
ExponentialFilter left2_exp = {0.5, 0.5};
ExponentialFilter right1_exp = {0.5, 0.5};
ExponentialFilter right2_exp = {0.5, 0.5};

//优化后的PD控制参数 

// 自适应PD控制参数
#define KP_FAST 2.0f    // 降低比例系数，减少过度修正
#define KP_SLOW 1.0f    // 降低稳定时的比例系数
#define KD_FAST 1.5f    // 降低微分系数，减少振荡
#define KD_SLOW 0.8f    // 降低稳定时的微分系数
#define BASE_SPEED 60   // 提高基础速度

// 传感器权重定义（从左到右：极左，左，右，极右）
#define WEIGHT_LEFT1  -1.5   // 降低极左权重，减少敏感度
#define WEIGHT_LEFT2  -0.8   // 降低左权重
#define WEIGHT_RIGHT1  0.8   // 降低右权重
#define WEIGHT_RIGHT2  1.5   // 降低极右权重

// PD控制相关变量
static int16_t last_error = 0;  // 上一次误差
static uint8_t rapid_change_count = 0; // 快速变化计数器

//  优化的滤波函数 

// 卡尔曼滤波函数
uint8_t kalman_filter(KalmanFilter* kf, uint8_t measurement) {
    float z = (float)measurement;
    
    // 预测步骤
    kf->p = kf->p + kf->q;
    
    // 更新步骤
    kf->k = kf->p / (kf->p + kf->r);
    kf->x = kf->x + kf->k * (z - kf->x);
    kf->p = (1 - kf->k) * kf->p;
    
    // 返回二值化结果
    return (kf->x > 0.5f) ? 1 : 0;
}//

// 指数加权移动平均滤波
uint8_t exponential_filter(ExponentialFilter* ef, uint8_t new_value) {
    ef->value = ef->alpha * (float)new_value + (1 - ef->alpha) * ef->value;
    return (ef->value > 0.5f) ? 1 : 0;
}

// 混合滤波：结合卡尔曼和指数滤波的优点
uint8_t hybrid_filter(uint8_t new_value, KalmanFilter* kf, ExponentialFilter* ef) {
    // 先用卡尔曼滤波
    uint8_t kalman_result = kalman_filter(kf, new_value);
    
    // 再用指数滤波平滑
    uint8_t final_result = exponential_filter(ef, kalman_result);
    
    return final_result;
}

// 计算传感器误差函数
int16_t calculate_error(void)
{
    // 使用已经滤波后的传感器数据
    int16_t error = (left1 * WEIGHT_LEFT1) + (left2 * WEIGHT_LEFT2) + 
                   (right1 * WEIGHT_RIGHT1) + (right2 * WEIGHT_RIGHT2);
    return error;
}

// 检测误差快速变化
uint8_t is_rapid_change(int16_t current_error, int16_t last_error) {
    return (abs(current_error - last_error) > 3); // 误差变化超过3视为快速变化
}

// 自适应PD控制计算函数
int16_t calculate_adaptive_pd_control(void)
{
    int16_t current_error = calculate_error();
    int16_t error_diff = current_error - last_error;
    
    // 自适应参数调整
    float kp, kd;
    if (is_rapid_change(current_error, last_error)) {
        kp = KP_FAST;  // 快速变化时使用大参数
        kd = KD_FAST;
        rapid_change_count = (rapid_change_count < 10) ? rapid_change_count + 1 : 10;
    } else {
        // 根据之前的快速变化情况平滑过渡到慢参数
        if (rapid_change_count > 0) {
            rapid_change_count--;
            float ratio = (float)rapid_change_count / 10.0f;
            kp = KP_FAST * ratio + KP_SLOW * (1 - ratio);
            kd = KD_FAST * ratio + KD_SLOW * (1 - ratio);
        } else {
            kp = KP_SLOW;  // 稳定时使用小参数
            kd = KD_SLOW;
        }
    }
    
    // 带死区的PD控制（减少微小振荡）
    int16_t turn = 0;
    if (abs(current_error) > 2) {  // 误差大于1时才响应
        turn = (int16_t)(kp * current_error + kd * error_diff);
    }
    
    // 非线性限制：小误差时限制松，大误差时限制紧
    int16_t max_turn;
    if (abs(current_error) <= 2) {
        max_turn = 30;  // 小误差时限制宽松
    } else if (abs(current_error) <= 4) {
        max_turn = 45;  // 中等误差
    } else {
        max_turn = 60;  // 大误差时允许更大的转向
    }
    
    if(turn > max_turn) turn = max_turn;
    if(turn < -max_turn) turn = -max_turn;
    
    last_error = current_error;
    
    return turn;
}

// 优化的PD控制驾驶函数
void car_pd_drive_fast(void)
{
    int16_t turn = calculate_adaptive_pd_control();
    
    // 计算左右轮速度 - 动态范围更大
    int16_t left_speed = BASE_SPEED + turn;
    int16_t right_speed = BASE_SPEED - turn;
    
    // 动态速度限制：根据转向幅度调整最小速度
    int16_t min_speed = (abs(turn) > 20) ? 20 : 30;  // 大转向时允许更低速度
    
    // 限制速度在合理范围内
    left_speed = (left_speed < min_speed) ? min_speed : (left_speed > 90) ? 90 : left_speed;
    right_speed = (right_speed < min_speed) ? min_speed : (right_speed > 90) ? 90 : right_speed;
    
    // 设置左轮速度
    GPIO_SetBits(GPIOB, GPIO_Pin_3);       // 左前轮正转
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    PWM_SetCompare1(left_speed);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_5);       // 左后轮正转
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
    PWM_SetCompare2(left_speed);
    
    // 设置右轮速度
    GPIO_SetBits(GPIOB, GPIO_Pin_11);      // 右前轮正转
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);
    PWM_SetCompare3(right_speed);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_10);      // 右后轮正转
    GPIO_ResetBits(GPIOA, GPIO_Pin_9);
    PWM_SetCompare4(right_speed);
}

// 原有的PD控制函数
int16_t calculate_pd_control(void)
{
    return calculate_adaptive_pd_control(); // 默认使用自适应版本
}

void car_pd_drive(void)
{
    car_pd_drive_fast(); // 默认使用快速版本
}

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

uint8_t sensor_check(void)//设置红外模块的状态（带改进的滤波）
{
    uint8_t mode;
    static uint8_t last_valid_mode = straight; // 保存上一次有效模式
    
    // 读取原始传感器数据并使用改进的混合滤波
    uint8_t raw_left1 = (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1) ? 0 : 1;
    uint8_t raw_left2 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1) ? 0 : 1;
    uint8_t raw_right1 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 1) ? 0 : 1;
    uint8_t raw_right2 = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 1) ? 0 : 1;

    // 使用改进的混合滤波算法
    left1 = hybrid_filter(raw_left1, &left1_kalman, &left1_exp);
    left2 = hybrid_filter(raw_left2, &left2_kalman, &left2_exp);
    right1 = hybrid_filter(raw_right1, &right1_kalman, &right1_exp);
    right2 = hybrid_filter(raw_right2, &right2_kalman, &right2_exp);
    
    // 首先检查是否全黑(0000)
    if(left1 == 0 && left2 == 0 && right1 == 0 && right2 == 0)
    {
        // 全黑时保持上一次的有效模式
        mode = last_valid_mode;
        //return mode;
    }

    // 使用滤波后的数据进行模式判断（原有逻辑不变）
    if((left1==0 && left2==1 && right1==1 && right2==0)||
    (left1==1 && left2==1 && right1==1 && right2==1))
    {
        mode=straight;//直行
        last_valid_mode = mode; // 更新有效模式
    }
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
        case corleft:           // 向左修正 - 使用PD控制
        case corright:          // 向右修正 - 使用PD控制
            car_pd_drive_fast(); // 使用优化的快速PD控制
            break;
        default:
            car_go_straight();
            break;
    }
}