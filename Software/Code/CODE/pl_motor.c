#include "headfile.h"
#include "pl_motor.h"


extern uint8 road_state;
extern int16 encoder_L,encoder_R;

bool strict_stop = 0;
int16 speed_set;


void motor_init() ///�����ʼ��
{
	pwm_init (PWM_L1, 10000, 0);      				//��Ƭ���˿�D1 ��ʼ������10K ռ�ձ�0
	pwm_init (PWM_L2, 10000, 0);     		
	pwm_init (PWM_R1, 10000, 0);     	
	pwm_init (PWM_R2, 10000, 0);     			
	
}

void motor_set(uint16 pwm_L,uint16 pwm_R,bool dir,bool strict_stop) //��������ת��ռ�ձ�  dir = 1 ��ת
{
	if(strict_stop)
	{
		pwm_duty(PWM_L1,0);
		pwm_duty(PWM_R1,0);
		pwm_duty(PWM_L2,0);
		pwm_duty(PWM_R2,0);		
	}
	else if(dir)
	{
		pwm_duty(PWM_L1,0);
		pwm_duty(PWM_R1,0);
		pwm_duty(PWM_L2,pwm_L);
		pwm_duty(PWM_R2,pwm_R);		
	}
	else
	{
		pwm_duty(PWM_L2,0);
		pwm_duty(PWM_R2,0);
		pwm_duty(PWM_L1,pwm_L);
		pwm_duty(PWM_R1,pwm_R);			
	}
}
uint8 stop_flag = 0;
void motor_ctrl ()  ///����ٶȵĿ���
{
	speed_set = 100;
//	if(ad_value_st[0]+ad_value_st[1]+ad_value_st[2]+ad_value_st[3]+ad_value_st[4] >= 50) //û�ܳ������Ļ�
//	{
//		if(!road_state)
//		{
//			speed_set = 120;
////			gpio_set(DIR_1,1);
////			pwm_duty(PWM_1,7000);		
////			gpio_set(DIR_2,1);
////			pwm_duty(PWM_2,7000); 
//		}
//		else 
//		{
//			speed_set = 100;
////			gpio_set(DIR_1,1);
////			pwm_duty(PWM_1,7000);		
////			gpio_set(DIR_2,1);
////			pwm_duty(PWM_2,7000); 	
//		}
//			
//	}
//	else   ///�ܳ�������Ҫͣ��
//	{
//		if(encoder_L < -10 )  //���ͣ���ɹ�,(���ǵ��򻬵������������������)
//			strict_stop = 1;
//		else
//			speed_set = -150;
////		if(!stop_flag)
////		{

////			gpio_set(DIR_1,0);
////			pwm_duty(PWM_1,5000);		
////			gpio_set(DIR_2,0);
////			pwm_duty(PWM_2,5000); 
////			systick_delay_ms(1000);
////			stop_flag = 1;
////		}
////		else
////		{
////			pwm_duty(PWM_1,0);		
////			pwm_duty(PWM_2,0);				
////		}
//	}
}


extern int16 encoder_L;
extern int16 encoder_R;
void encoder_init() //������pit��ʼ��
{
  //��ʼ�� QTIMER_1 A��ʹ��QTIMER1_TIMER0_C0 B��ʹ��QTIMER1_TIMER1_C1
  qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
    
  //��ʼ�� QTIMER_1 A��ʹ��QTIMER1_TIMER2_C2 B��ʹ��QTIMER1_TIMER3_C24
  qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
		
	
	pit_interrupt_ms(PIT_CH2,5);//��ʼ�����ڶ�ʱ��ͨ��2  ����ʱ��Ϊ5���� ��������

}

uint16 pwm_duty_L,pwm_duty_R;
int16 error_speed_L,error_speed_R;
bool direction;
bool car_wrong; //�����ٺܵ͵�ռ�ձȺܸ�ʱ��˵�������˴��󣬱��翨ס�ˣ���ʱcar_wrong = 1
void speed_ctrl(int16 speed_set,float P_speed,float I_speed)  ///PI�����ٶ� ��I��ʱ��û�ã�
{
	direction = speed_set>=0? 1:0;
	error_speed_L = speed_set - encoder_L;
	error_speed_R = speed_set - encoder_R;
	
	if((-5 <= encoder_L && encoder_L <= 5 && pwm_duty_L >= 10000)||(-5 <= encoder_R && encoder_R <= 5 && pwm_duty_R >= 10000))//���ת�ٺ�����ռ�ձȲ�С�Ļ���˵������ס��֮��ģ�ֱ�Ӳ�����
	{
		pwm_duty_L = 0;
		pwm_duty_R = 0;
		car_wrong  = 1;
	}
  if(!car_wrong)
	{
		pwm_duty_L += P_speed * error_speed_L * (2*direction-1);  //2d-1 d=1ʱ��=1��d=0 ʱ����-1 
		pwm_duty_R += P_speed * error_speed_R * (2*direction-1);		
	}
//	else if(car_wrong) //10s������
//	{
//		systick_delay_ms(5000);
//		car_wrong = 0;
//	}
	motor_set(pwm_duty_L,pwm_duty_R,direction,strict_stop);
}