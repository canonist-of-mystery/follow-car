#ifndef __LED_H
#define __LED_H

void sensor_init(void);
uint8_t sensor_check(void);
void sensor_control(uint8_t mode);
uint8_t moving_average_filter(MovingAverageFilter* filter, uint8_t new_value);
int16_t calculate_error(void);
int16_t calculate_pd_control(void);

#endif
