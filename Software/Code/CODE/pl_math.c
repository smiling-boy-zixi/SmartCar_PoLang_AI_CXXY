#include "headfile.h"
#include "pl_headfile.h"

//�������ã�����ֵ����
uint16 abslt(int num)
{
	if(num >= 0)
		return num;
	else 
		return -num;
}

//�������ã��ж����������Ƿ��Լ��ȣ�num_a��num_bΪ������������֣�D_value_perΪ�������� %Ϊ��λ
//��д��Approximately equal toԼ����				tolerant error������
bool is_app_equal(uint16 num_a,uint16 num_b,uint16 tole_error)
{
	int d_num = num_a - num_b; //��ֵ
	if(abslt((d_num)*100/(num_a >= num_b? num_a:num_b)) <= tole_error)
		return 1;
	else
		return 0;
}