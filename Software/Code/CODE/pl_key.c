
//ע����main�����ﵱ��⵽key_flag�������������º�Ҫ������㣬��Ȼ��һֱִ��key_flag==1�����µ����

#include "SEEKFREE_FONT.h"
#include "headfile.h"
#include "pl_headfile.h"

//���尴������
#define KEY1    C25
#define KEY2    C26
#define KEY3    C3
#define KEY4    C4
//���岦�뿪������
#define SW1     C27
#define SW2     D27


//���뿪��״̬����
uint8 sw1_status;
uint8 sw2_status;

//����״̬����
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

//��һ�ο���״̬����
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

//���ر�־λ
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

uint8 test1,test2,test3,test4;

void key_init(void)
{
		//������ʼ��
    gpio_init(KEY1,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY2,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY3,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY4,GPI,0,GPIO_PIN_CONFIG);
    
    //���뿪�س�ʼ��
    gpio_init(SW1,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(SW2,GPI,0,GPIO_PIN_CONFIG);
}
void key_check(void)
{
    
    
		//��ȡ���뿪��״̬
		sw1_status = gpio_get(SW1);
		sw2_status = gpio_get(SW2);

		
		//ʹ�ô˷����ŵ����ڣ�����Ҫʹ��while(1) �ȴ������⴦������Դ�˷�
		//���水��״̬
		key1_last_status = key1_status;
		key2_last_status = key2_status;
		key3_last_status = key3_status;
		key4_last_status = key4_status;
		//��ȡ��ǰ����״̬
		key1_status = gpio_get(KEY1);
		key2_status = gpio_get(KEY2);
		key3_status = gpio_get(KEY3);
		key4_status = gpio_get(KEY4);
		
		//��⵽��������֮��  ���ſ���λ��־λ,ע����main�����ﵱ��⵽key_flag�������������º�Ҫ������㣬��Ȼ��һֱִ��
		if(key1_status && !key1_last_status)    key1_flag = 1;
		if(key2_status && !key2_last_status)    key2_flag = 1;
		if(key3_status && !key3_last_status)    key3_flag = 1;
		if(key4_status && !key4_last_status)    key4_flag = 1;
		
		
		systick_delay_ms(10);//��ʱ����������Ӧ�ñ�֤����ʱ�䲻С��10ms
}
