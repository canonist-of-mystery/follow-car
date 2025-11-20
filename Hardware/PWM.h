#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);

void PWM_SetCompare1_left(uint8_t Compare);
void PWM_SetCompare2_left(uint8_t Compare);

void PWM_SetCompare1_right(uint8_t Compare);
void PWM_SetCompare2_right(uint8_t Compare);

#endif
