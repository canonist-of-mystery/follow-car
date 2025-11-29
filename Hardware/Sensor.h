#ifndef __LED_H
#define __LED_H

void sensor_init(void);
uint8_t sensor_check(void);
void sensor_control(uint8_t mode);
uint8_t moving_average_filter(MovingAverageFilter* filter, uint8_t new_value);
int16_t calculate_error(void);
int16_t calculate_pd_control(void);
void car_pd_drive(void);
uint8_t kalman_filter(KalmanFilter* kf, uint8_t measurement);
uint8_t exponential_filter(ExponentialFilter* ef, uint8_t new_value);
uint8_t hybrid_filter(uint8_t new_value, KalmanFilter* kf, ExponentialFilter* ef);
void car_pd_drive_fast(void);
int16_t calculate_adaptive_pd_control(void);
uint8_t is_rapid_change(int16_t current_error, int16_t last_error);

#endif
