#include "headfile.h"
#include "pl_headfile.h"
#include "SEEKFREE_FONT.h"


//定义ADC通道与引脚
#define POWER_ADC0_MOD  ADC_1       	//定义通道0 ADC模块号
#define POWER_ADC0_PIN  ADC1_CH3_B14	//定义通道0 ADC引脚
	
#define POWER_ADC1_MOD  ADC_1       	//定义通道1 ADC模块号
#define POWER_ADC1_PIN  ADC1_CH4_B15	//定义通道1 ADC引脚
														
#define POWER_ADC2_MOD  ADC_1       	//定义通道2 ADC模块号
#define POWER_ADC2_PIN  ADC1_CH5_B16	//定义通道2 ADC引脚

#define POWER_ADC3_MOD  ADC_1       	//定义通道3 ADC模块号
#define POWER_ADC3_PIN  ADC1_CH6_B17	//定义通道3 ADC引脚

#define POWER_ADC4_MOD  ADC_1       	//定义通道4 ADC模块号
#define POWER_ADC4_PIN  ADC1_CH15_B26	//定义通道4 ADC引脚

#define POWER_ADC5_MOD  ADC_1       	//定义通道5 ADC模块号
#define POWER_ADC5_PIN  ADC1_CH14_B25	//定义通道5 ADC引脚

#define POWER_ADC6_MOD  ADC_1       	//定义通道6 ADC模块号
#define POWER_ADC6_PIN  ADC1_CH13_B24	//定义通道6 ADC引脚

#define POWER_ADC7_MOD  ADC_1       	//定义通道7 ADC模块号
#define POWER_ADC7_PIN  ADC1_CH11_B22	//定义通道7 ADC引脚

#define POWER_ADC8_MOD  ADC_1       	//定义通道8 ADC模块号
#define POWER_ADC8_PIN  ADC1_CH7_B18	//定义通道8 ADC引脚

#define POWER_ADC9_MOD  ADC_1       	//定义通道9 ADC模块号
#define POWER_ADC9_PIN  ADC1_CH8_B19	//定义通道9 ADC引脚

#define POWER_ADC10_MOD  ADC_1       	//定义通道10 ADC模块号
#define POWER_ADC10_PIN  ADC1_CH9_B20	//定义通道10 ADC引脚

#define POWER_ADC11_MOD  ADC_1       	//定义通道11 ADC模块号
#define POWER_ADC11_PIN  ADC1_CH10_B21	//定义通道11 ADC引脚

#define POWER_ADC12_MOD  ADC_2       	//定义通道12 ADC模块号
#define POWER_ADC12_PIN  ADC2_CH3_B30	//定义通道12 ADC引脚

#define POWER_ADC13_MOD  ADC_2       	//定义通道13 ADC模块号
#define POWER_ADC13_PIN  ADC2_CH2_B29	//定义通道13 ADC引脚

#define POWER_ADC14_MOD  ADC_2       	//定义通道14 ADC模块号
#define POWER_ADC14_PIN  ADC2_CH1_B28	//定义通道14 ADC引脚

#define POWER_ADC15_MOD  ADC_2       	//定义通道15 ADC模块号
#define POWER_ADC15_PIN  ADC2_CH0_B27	//定义通道15 ADC引脚

#define POWER_ADC16_MOD  ADC_1       	//定义通道16 ADC模块号
#define POWER_ADC16_PIN  ADC1_CH12_B23	//定义通道16 ADC引脚  自己加的

extern uint16 ad_value[16];		//ad读到的值
extern uint16 ad_value_max[16];//不同传感器下的最大值（方便更换元件后使用）
extern uint16	ad_value_st[16]; //Standardization标准化到0-256/255？

void ad_init_all (void)
{
	  adc_init(POWER_ADC0_MOD ,POWER_ADC0_PIN ,ADC_8BIT);  //同一个ADC模块分辨率应该设置为一样的，如果设置不一样，则最后一个初始化时设置的分辨率生效
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

void ad_get(void)									//读取AD的值
{
			ad_value[0]  = adc_mean_filter(POWER_ADC16_MOD ,POWER_ADC16_PIN,5);//B23 前瞻 1（从左到右）
      ad_value[1]  = adc_mean_filter(POWER_ADC15_MOD ,POWER_ADC15_PIN,5);//B27 前瞻 2
      ad_value[2]  = adc_mean_filter(POWER_ADC8_MOD  ,POWER_ADC8_PIN ,5);//B18	前瞻 3
      ad_value[3]  = adc_mean_filter(POWER_ADC3_MOD  ,POWER_ADC3_PIN ,5);//B17	前瞻 4
			ad_value[4]  = adc_mean_filter(POWER_ADC13_MOD ,POWER_ADC13_PIN,5);//B29 前瞻 5
	    ad_value[5]  = adc_mean_filter(POWER_ADC1_MOD  ,POWER_ADC1_PIN ,5);//B15	车头 1
      ad_value[6]  = adc_mean_filter(POWER_ADC10_MOD ,POWER_ADC10_PIN,5);//B20	车头 2
      ad_value[7]  = adc_mean_filter(POWER_ADC7_MOD  ,POWER_ADC7_PIN ,5);//B22	车头 3
      ad_value[8]  = adc_mean_filter(POWER_ADC11_MOD ,POWER_ADC11_PIN,5);//B21	车头 4
	    ad_value[9]  = adc_mean_filter(POWER_ADC0_MOD  ,POWER_ADC0_PIN ,5);//B14	车头 5
      ad_value[10] = adc_mean_filter(POWER_ADC14_MOD ,POWER_ADC14_PIN,5);//B28	车身 1
			ad_value[11] = adc_mean_filter(POWER_ADC12_MOD ,POWER_ADC12_PIN,5);//B30	车身 2
}

uint8 i_admax = 0;
void ad_get_max(void)			//得到最大值是多少的时候用的函数
{
	for(i_admax = 0;i_admax < 12;i_admax++)
	{
		ad_value_max[i_admax] = ad_value_max[i_admax] >= ad_value[i_admax]? ad_value_max[i_admax] : ad_value[i_admax];
	}
}

void ad_max_enum(void)		//将得到的值枚举
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
void ad_standard(void)		//将各AD的值都统一到128左右（所用的max是仅在一根电磁线上搞的，所以遇到圆环什么的可能会超128）因为不同电感运放等有不小的误差
{
	for(i_st = 0;i_st < 12;i_st++)
	{
		if(ad_value_max[i_st] >= 20)
			ad_value_st[i_st] = (ad_value[i_st]<<7)/ad_value_max[i_st];
	}
}