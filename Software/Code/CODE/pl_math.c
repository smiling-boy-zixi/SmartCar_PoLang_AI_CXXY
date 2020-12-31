#include "headfile.h"
#include "pl_headfile.h"

//函数作用：绝对值函数
uint16 abslt(int num)
{
	if(num >= 0)
		return num;
	else 
		return -num;
}

//函数作用：判断两个数字是否大约相等，num_a、num_b为输入的两个数字，D_value_per为容许的误差 %为单位
//缩写：Approximately equal to约等于				tolerant error容忍误差，
bool is_app_equal(uint16 num_a,uint16 num_b,uint16 tole_error)
{
	int d_num = num_a - num_b; //差值
	if(abslt((d_num)*100/(num_a >= num_b? num_a:num_b)) <= tole_error)
		return 1;
	else
		return 0;
}