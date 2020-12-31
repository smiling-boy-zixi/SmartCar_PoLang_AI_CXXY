#include "headfile.h"
#include "pl_headfile.h"
#include "SEEKFREE_FONT.h"


//����ADCͨ��������
#define POWER_ADC0_MOD  ADC_1       	//����ͨ��0 ADCģ���
#define POWER_ADC0_PIN  ADC1_CH3_B14	//����ͨ��0 ADC����
	
#define POWER_ADC1_MOD  ADC_1       	//����ͨ��1 ADCģ���
#define POWER_ADC1_PIN  ADC1_CH4_B15	//����ͨ��1 ADC����
														
#define POWER_ADC2_MOD  ADC_1       	//����ͨ��2 ADCģ���
#define POWER_ADC2_PIN  ADC1_CH5_B16	//����ͨ��2 ADC����

#define POWER_ADC3_MOD  ADC_1       	//����ͨ��3 ADCģ���
#define POWER_ADC3_PIN  ADC1_CH6_B17	//����ͨ��3 ADC����

#define POWER_ADC4_MOD  ADC_1       	//����ͨ��4 ADCģ���
#define POWER_ADC4_PIN  ADC1_CH15_B26	//����ͨ��4 ADC����

#define POWER_ADC5_MOD  ADC_1       	//����ͨ��5 ADCģ���
#define POWER_ADC5_PIN  ADC1_CH14_B25	//����ͨ��5 ADC����

#define POWER_ADC6_MOD  ADC_1       	//����ͨ��6 ADCģ���
#define POWER_ADC6_PIN  ADC1_CH13_B24	//����ͨ��6 ADC����

#define POWER_ADC7_MOD  ADC_1       	//����ͨ��7 ADCģ���
#define POWER_ADC7_PIN  ADC1_CH11_B22	//����ͨ��7 ADC����

#define POWER_ADC8_MOD  ADC_1       	//����ͨ��8 ADCģ���
#define POWER_ADC8_PIN  ADC1_CH7_B18	//����ͨ��8 ADC����

#define POWER_ADC9_MOD  ADC_1       	//����ͨ��9 ADCģ���
#define POWER_ADC9_PIN  ADC1_CH8_B19	//����ͨ��9 ADC����

#define POWER_ADC10_MOD  ADC_1       	//����ͨ��10 ADCģ���
#define POWER_ADC10_PIN  ADC1_CH9_B20	//����ͨ��10 ADC����

#define POWER_ADC11_MOD  ADC_1       	//����ͨ��11 ADCģ���
#define POWER_ADC11_PIN  ADC1_CH10_B21	//����ͨ��11 ADC����

#define POWER_ADC12_MOD  ADC_2       	//����ͨ��12 ADCģ���
#define POWER_ADC12_PIN  ADC2_CH3_B30	//����ͨ��12 ADC����

#define POWER_ADC13_MOD  ADC_2       	//����ͨ��13 ADCģ���
#define POWER_ADC13_PIN  ADC2_CH2_B29	//����ͨ��13 ADC����

#define POWER_ADC14_MOD  ADC_2       	//����ͨ��14 ADCģ���
#define POWER_ADC14_PIN  ADC2_CH1_B28	//����ͨ��14 ADC����

#define POWER_ADC15_MOD  ADC_2       	//����ͨ��15 ADCģ���
#define POWER_ADC15_PIN  ADC2_CH0_B27	//����ͨ��15 ADC����

#define POWER_ADC16_MOD  ADC_1       	//����ͨ��16 ADCģ���
#define POWER_ADC16_PIN  ADC1_CH12_B23	//����ͨ��16 ADC����  �Լ��ӵ�

extern uint16 ad_value[16];		//ad������ֵ
extern uint16 ad_value_max[16];//��ͬ�������µ����ֵ���������Ԫ����ʹ�ã�
extern uint16	ad_value_st[16]; //Standardization��׼����0-256/255��

void ad_init_all (void)
{
	  adc_init(POWER_ADC0_MOD ,POWER_ADC0_PIN ,ADC_8BIT);  //ͬһ��ADCģ��ֱ���Ӧ������Ϊһ���ģ�������ò�һ���������һ����ʼ��ʱ���õķֱ�����Ч
    adc_init(POWER_ADC1_MOD ,POWER_ADC1_PIN ,ADC_8BIT);
    adc_init(POWER_ADC2_MOD ,POWER_ADC2_PIN ,ADC_8BIT);
    adc_init(POWER_ADC3_MOD ,POWER_ADC3_PIN ,ADC_8BIT);
    adc_init(POWER_ADC4_MOD ,POWER_ADC4_PIN ,ADC_8BIT);  
    adc_init(POWER_ADC5_MOD ,POWER_ADC5_PIN ,ADC_8BIT);
    adc_init(POWER_ADC6_MOD ,POWER_ADC6_PIN ,ADC_8BIT);
    adc_init(POWER_ADC7_MOD ,POWER_ADC7_PIN ,ADC_8BIT);
		adc_init(POWER_ADC8_MOD ,POWER_ADC8_PIN ,ADC_8BIT);  
    adc_init(POWER_ADC9_MOD ,POWER_ADC9_PIN ,ADC_8BIT);
    adc_init(POWER_ADC10_MOD,POWER_ADC10_PIN,ADC_8BIT);
    adc_init(POWER_ADC11_MOD,POWER_ADC11_PIN,ADC_8BIT);
		adc_init(POWER_ADC12_MOD,POWER_ADC12_PIN,ADC_8BIT);  
    adc_init(POWER_ADC13_MOD,POWER_ADC13_PIN,ADC_8BIT);
    adc_init(POWER_ADC14_MOD,POWER_ADC14_PIN,ADC_8BIT);
    adc_init(POWER_ADC15_MOD,POWER_ADC15_PIN,ADC_8BIT);
    adc_init(POWER_ADC16_MOD,POWER_ADC16_PIN,ADC_8BIT);
}

void ad_get(void)									//��ȡAD��ֵ
{
			ad_value[0]  = adc_mean_filter(POWER_ADC16_MOD ,POWER_ADC16_PIN,5);//B23 ǰհ 1�������ң�
      ad_value[1]  = adc_mean_filter(POWER_ADC15_MOD ,POWER_ADC15_PIN,5);//B27 ǰհ 2
      ad_value[2]  = adc_mean_filter(POWER_ADC8_MOD  ,POWER_ADC8_PIN ,5);//B18	ǰհ 3
      ad_value[3]  = adc_mean_filter(POWER_ADC3_MOD  ,POWER_ADC3_PIN ,5);//B17	ǰհ 4
			ad_value[4]  = adc_mean_filter(POWER_ADC13_MOD ,POWER_ADC13_PIN,5);//B29 ǰհ 5
	    ad_value[5]  = adc_mean_filter(POWER_ADC1_MOD  ,POWER_ADC1_PIN ,5);//B15	��ͷ 1
      ad_value[6]  = adc_mean_filter(POWER_ADC10_MOD ,POWER_ADC10_PIN,5);//B20	��ͷ 2
      ad_value[7]  = adc_mean_filter(POWER_ADC7_MOD  ,POWER_ADC7_PIN ,5);//B22	��ͷ 3
      ad_value[8]  = adc_mean_filter(POWER_ADC11_MOD ,POWER_ADC11_PIN,5);//B21	��ͷ 4
	    ad_value[9]  = adc_mean_filter(POWER_ADC0_MOD  ,POWER_ADC0_PIN ,5);//B14	��ͷ 5
      ad_value[10] = adc_mean_filter(POWER_ADC14_MOD ,POWER_ADC14_PIN,5);//B28	���� 1
			ad_value[11] = adc_mean_filter(POWER_ADC12_MOD ,POWER_ADC12_PIN,5);//B30	���� 2
}

uint8 i_admax = 0;
void ad_get_max(void)			//�õ����ֵ�Ƕ��ٵ�ʱ���õĺ���
{
	for(i_admax = 0;i_admax < 12;i_admax++)
	{
		ad_value_max[i_admax] = ad_value_max[i_admax] >= ad_value[i_admax]? ad_value_max[i_admax] : ad_value[i_admax];
	}
}

void ad_max_enum(void)		//���õ���ֵö��
{
	ad_value_max[0] = 82; //163
	ad_value_max[1] = 96; //233
	ad_value_max[2] = 84;
	ad_value_max[3] = 91; //239
	ad_value_max[4] = 73;//117
	ad_value_max[5] = 249;//239
	ad_value_max[6] = 239;
}

uint8 i_st = 0;
void ad_standard(void)		//����AD��ֵ��ͳһ��128���ң����õ�max�ǽ���һ��������ϸ�ģ���������Բ��ʲô�Ŀ��ܻᳬ128����Ϊ��ͬ����˷ŵ��в�С�����
{
	for(i_st = 0;i_st < 12;i_st++)
	{
		if(ad_value_max[i_st] >= 20)
			ad_value_st[i_st] = (ad_value[i_st]<<7)/ad_value_max[i_st];
	}
}