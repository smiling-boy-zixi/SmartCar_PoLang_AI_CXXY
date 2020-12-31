#include "headfile.h"
#include "pl_headfile.h"



//用extern的 ad value    duty是exern还是咋还没考虑好
extern uint16 ad_value_st[16];
extern int16 delta_advalue[16];
extern uint16 duty;
extern uint8 road_state;

int error_L1L2_R1R2 = 0; //前瞻
int error_d_L1L2_R1R2 = 0;
int error_head;//车头
int error_body;//车身


void	pid_turn(uint16 para_p,uint16 para_d)
{
	error_L1L2_R1R2 = ((ad_value_st[0] + ad_value_st[1] - ad_value_st[3] - ad_value_st[4])<<7)/(ad_value_st[0] + ad_value_st[1] + ad_value_st[3] + ad_value_st[4]); //差比和
	error_d_L1L2_R1R2 = delta_advalue[0] + delta_advalue[1] - delta_advalue[3] - delta_advalue[4];
	
	if(duty <= duty_max && duty >= duty_min)
	{
		if((ad_value_st[0] > 90 &&	ad_value_st[1] >= 70 && ad_value_st[3] <= 50 && ad_value_st[4] >= 70)||(ad_value_st[2] >= 130)) //如果是环岛
		{
			duty = duty_c + ((para_p * error_L1L2_R1R2)/50) + ((para_d * error_d_L1L2_R1R2)/50);			
			if(duty > duty_max)
				duty = duty_max;
			else if(duty < duty_min)
				duty = duty_min;
		}
		else
		{	
			if(error_L1L2_R1R2 <= 40 && error_L1L2_R1R2 >= -40) //直道
			{
				duty = duty_c + ((para_p * error_L1L2_R1R2)/20) + ((para_d * error_d_L1L2_R1R2)/20); //右移四位是为了适当的缩小
				road_state = 0;
			}
			else if((error_L1L2_R1R2 > 40 && error_L1L2_R1R2 <= 100)||(error_L1L2_R1R2 >= -100 && error_L1L2_R1R2 <= -40))//缓弯
			{
				duty = duty_c + ((para_p * error_L1L2_R1R2)/13) + ((para_d * error_d_L1L2_R1R2)/13); //右移四位是为了适当的缩小
				road_state = 1;
			}
			else if(error_L1L2_R1R2 >= 100 || error_L1L2_R1R2 <= -100)//急弯
			{
				duty = duty_c + ((para_p * error_L1L2_R1R2)/11) + ((para_d * error_d_L1L2_R1R2)/11); //右移四位是为了适当的缩小
				road_state = 2;
			}
			if(duty > duty_max)
				duty = duty_max;
			else if(duty < duty_min)
				duty = duty_min;
		}
	}
	
}

