#ifndef __PL_MOTOR_H
#define __PL_MOTOR_H

#include "headfile.h"

//�궨��
#define PWM_L1 PWM1_MODULE3_CHB_D1
#define PWM_L2 PWM1_MODULE3_CHA_D0
#define PWM_R1 PWM2_MODULE3_CHA_D2
#define PWM_R2 PWM2_MODULE3_CHB_D3

//����
void motor_init(void);
void motor_set(uint16 pwm_L,uint16 pwm_R,bool dir,bool strict_stop); //strict stopΪ1ʱ���ò���pwm�ķ�ʽֹͣ��������pi����
void motor_ctrl(void);
void encoder_init(void);
void speed_ctrl(int16 speed_set,float P_speed,float I_speed);
#endif