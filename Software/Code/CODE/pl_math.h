#ifndef __PL_MATH_H
#define __PL_MATH_H

#include "headfile.h"

//�������ã�����ֵ����
uint16	abslt(int num);

//�������ã��ж����������Ƿ��Լ��ȣ�num_a��num_bΪ������������֣�D_value_perΪ��������
//��д��Approximately equal toԼ����				tolerant error������%Ϊ��λ
bool is_app_equal (uint16 num_a,uint16 num_b,uint16 tole_error); 

#endif