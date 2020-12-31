#include "headfile.h"
#include "pl_headfile.h"



//��extern�� ad value    duty��exern����զ��û���Ǻ�
extern uint16 ad_value_st[16];
extern int16 delta_advalue[16];
extern uint16 duty;
extern uint8 road_state;

int error_L1L2_R1R2 = 0; //ǰհ
int error_d_L1L2_R1R2 = 0;
int error_head;//��ͷ
int error_body;//����


void	pid_turn(uint16 para_p,uint16 para_d)
{
	error_L1L2_R1R2 = ((ad_value_st[0] + ad_value_st[1] - ad_value_st[3] - ad_value_st[4])<<7)/(ad_value_st[0] + ad_value_st[1] + ad_value_st[3] + ad_value_st[4]); //��Ⱥ�
	error_d_L1L2_R1R2 = delta_advalue[0] + delta_advalue[1] - delta_advalue[3] - delta_advalue[4];
	
	if(duty <= duty_max && duty >= duty_min)
	{
		if((ad_value_st[0] > 90 &&	ad_value_st[1] >= 70 && ad_value_st[3] <= 50 && ad_value_st[4] >= 70)||(ad_value_st[2] >= 130)) //����ǻ���
		{
			duty = duty_c + ((para_p * error_L1L2_R1R2)/50) + ((para_d * error_d_L1L2_R1R2)/50);			
			if(duty > duty_max)
				duty = duty_max;
			else if(duty < duty_min)
				duty = duty_min;
		}
		else
		{	
			if(error_L1L2_R1R2 <= 40 && error_L1L2_R1R2 >= -40) //ֱ��
			{
				duty = duty_c + ((para_p * error_L1L2_R1R2)/20) + ((para_d * error_d_L1L2_R1R2)/20); //������λ��Ϊ���ʵ�����С
				road_state = 0;
			}
			else if((error_L1L2_R1R2 > 40 && error_L1L2_R1R2 <= 100)||(error_L1L2_R1R2 >= -100 && error_L1L2_R1R2 <= -40))//����
			{
				duty = duty_c + ((para_p * error_L1L2_R1R2)/13) + ((para_d * error_d_L1L2_R1R2)/13); //������λ��Ϊ���ʵ�����С
				road_state = 1;
			}
			else if(error_L1L2_R1R2 >= 100 || error_L1L2_R1R2 <= -100)//����
			{
				duty = duty_c + ((para_p * error_L1L2_R1R2)/11) + ((para_d * error_d_L1L2_R1R2)/11); //������λ��Ϊ���ʵ�����С
				road_state = 2;
			}
			if(duty > duty_max)
				duty = duty_max;
			else if(duty < duty_min)
				duty = duty_min;
		}
	}
	
}

