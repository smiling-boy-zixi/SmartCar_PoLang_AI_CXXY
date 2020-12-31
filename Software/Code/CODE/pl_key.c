
//注意在main函数里当检测到key_flag并做完想做的事后，要将其归零，不然会一直执行key_flag==1条件下的语句

#include "SEEKFREE_FONT.h"
#include "headfile.h"
#include "pl_headfile.h"

//定义按键引脚
#define KEY1    C25
#define KEY2    C26
#define KEY3    C3
#define KEY4    C4
//定义拨码开关引脚
#define SW1     C27
#define SW2     D27


//拨码开关状态变量
uint8 sw1_status;
uint8 sw2_status;

//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

//开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

uint8 test1,test2,test3,test4;

void key_init(void)
{
		//按键初始化
    gpio_init(KEY1,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY2,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY3,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY4,GPI,0,GPIO_PIN_CONFIG);
    
    //拨码开关初始化
    gpio_init(SW1,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(SW2,GPI,0,GPIO_PIN_CONFIG);
}
void key_check(void)
{
    
    
		//获取拨码开关状态
		sw1_status = gpio_get(SW1);
		sw2_status = gpio_get(SW2);

		
		//使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
		//保存按键状态
		key1_last_status = key1_status;
		key2_last_status = key2_status;
		key3_last_status = key3_status;
		key4_last_status = key4_status;
		//读取当前按键状态
		key1_status = gpio_get(KEY1);
		key2_status = gpio_get(KEY2);
		key3_status = gpio_get(KEY3);
		key4_status = gpio_get(KEY4);
		
		//检测到按键按下之后  并放开置位标志位,注意在main函数里当检测到key_flag并做完想做的事后，要将其归零，不然会一直执行
		if(key1_status && !key1_last_status)    key1_flag = 1;
		if(key2_status && !key2_last_status)    key2_flag = 1;
		if(key3_status && !key3_last_status)    key3_flag = 1;
		if(key4_status && !key4_last_status)    key4_flag = 1;
		
		
		systick_delay_ms(10);//延时，按键程序应该保证调用时间不小于10ms
}
