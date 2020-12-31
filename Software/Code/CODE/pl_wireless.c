#include "headfile.h"
#include "pl_headfile.h"

extern uint16 ad_value[16];
extern uint16 duty;
int8 ad[7];
void wireless_init() //�������ߴ��䷽��ĺ�PIT�����
{
	seekfree_wireless_init();
  pit_interrupt_ms(PIT_CH1,20);//��ʼ�����ڶ�ʱ��ͨ��1  ����ʱ��Ϊ50���� ���ߴ����õ�

}
	
void send_ad_and_duty()
{
	int i_send = 0;
	for (i_send=5; i_send<12; i_send++)
	{
		ad[i_send-5] = ad_value[i_send] - 128;
	}
	int8 send_duty = (int32)(duty-duty_c)*128/450; //��ֵ���900����4��225���ӽ�256(֮ǰ�ı�ע��)
	uint8 send_package[11] = {0,0,ad[5],ad[6],ad[7],ad[8],ad[9],ad[10],ad[11],send_duty,0x5a};
	seekfree_wireless_send_buff(send_package,11);
}