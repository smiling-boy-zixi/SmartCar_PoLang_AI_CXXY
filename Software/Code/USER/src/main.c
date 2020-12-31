/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.26
 * @Target core		NXP RT1021DAG5A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-02-18
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完




#include "headfile.h" 

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "pl_motor.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
volatile uint32_t g_tick;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
 
 #include "nncie.h"
 #define IMAGE_SIZE    (8 + 32*32*3)  // 3080

extern void* RunModel(const void *in_buf);
//q7_t *g_out;
int16 g_out[1];


typedef enum _enum_OutType
{
	outtype_cls = 0,
	outtype_lgsbin = 1,
	outtype_rgrcls = 2,
	outtype_angle = 3,
	outtype_recon = 4,
	outtype_raw = 100,
}outType_e;

typedef struct _ClsTest_t
{
	uint32_t clsCnt;
}ClsTest_t;

typedef struct _TestFileHdr_t
{
	char sMgc[4];	// NCTT
	uint32_t tvSize;
	uint32_t tvCnt;
	uint8_t outType;
	uint8_t isExtTV;	// is using extended test vector format
	uint16_t outDim;
	union {
		float f32Params[2];
		int32_t i32Params[2];
	};
	uint32_t rsvd[2];
}TestFileHdr_t;

typedef struct _TestVec_t
{
	char _0xA5;	// must be 0xA5, else check for TestVecExt_t
	uint8_t w, h, c;
	union {
		uint32_t label;
	};
}TestVec_t;

typedef struct _TestVecExt_t
{
	char magic[4];	//
	uint16_t w, h, c, z;
	union {
		uint32_t label;
	};
}TestVecExt_t;

typedef struct _TestCtx_t
{
	TestFileHdr_t hdr;
	const void* pvTVs;
	uint32_t tvNdx;
	uint8_t isXIP;
}TestCtx_t;

const TestVec_t* TestVecPopNext(TestCtx_t* pCtx) {
	if (pCtx->tvNdx >= pCtx->hdr.tvCnt)
		return 0;
	uint32_t adr = (uint32_t)pCtx->pvTVs + pCtx->tvNdx * pCtx->hdr.tvSize;
	pCtx->tvNdx++;
	return (const TestVec_t*)adr;
}

int TestVecCreateCtxXIP(const void *pvTestData, TestCtx_t* pCtx) {
	int ret = 0;
	memset(pCtx, 0, sizeof(*pCtx));
	memcpy(&pCtx->hdr, pvTestData, sizeof(pCtx->hdr));
	pCtx->pvTVs = (const void*)((uint32_t)pvTestData + sizeof(pCtx->hdr));
	pCtx->isXIP = 1;
	return ret;
}

#if 0
int TestVecCreateCtx(const char* pszTestFile, TestCtx_t* pCtx) {
	int ret = 0;
	FILE* fp = fopen(pszTestFile, "rb");
	uint32_t tvsSize;
	if (0 == fp) {
		ret = -1L;
		goto cleanup;
	}
	memset(pCtx, 0, sizeof(*pCtx));
	fread(&pCtx->hdr, sizeof(pCtx->hdr), 1, fp);
	tvsSize = pCtx->hdr.tvCnt * pCtx->hdr.tvSize;
	pCtx->pvTVs = malloc(tvsSize);
	fread((void*)pCtx->pvTVs, 1, tvsSize, fp);
	fclose(fp);
cleanup:	
	return ret;
}
#endif

int TestVecDestroyCtx(TestCtx_t* pCtx) {
	if (!pCtx->isXIP)
		free((void*)pCtx->pvTVs);
	return 0;
}

float GetAngle(signed char *pv1, signed char *pv2)
{
	int32_t iM1, iM2, iDot;
	float fM1, fM2, fDot;
	int i;
	for (i = 0, iM1 = 0, iM2 = 0, iDot = 0; i<128; i++) {
		iDot += (int32_t)pv1[i] * pv2[i];
		iM1 += (int32_t)pv1[i] * pv1[i];
		iM2 += (int32_t)pv2[i] * pv2[i];
	}
	fDot = (float)iDot;
	fM1 = sqrtf((float)iM1);
	fM2 = sqrtf((float)iM2);
	float cosVal = fDot / (fM1 * fM2);
	float angle = acosf(cosVal) * 180 / 3.141592654f;
	return angle;
}


#define ANGLE_BORDER	59.8f // 64.569822f
int CIETest(const void *pvModel, TestCtx_t *pCtx) {
	int i, j, okCnt = 0;
	const char* pImg;
	float avgAngs[2] = { 0.0f, 0.0f };
	int cnts[2] = { 0, 0 };
	int isSame = 0;
	int avg_mse = 0;
	ModelInfo_t inf;
	CI_GetModelInfoXIP(pvModel, &inf);
	if (inf.pszString)
		PRINTF("%s\r\n", inf.pszString); 
	#ifdef SRC_DEBUG
	uint8_t *pOut;
	int max, maxAt;
	#else
	
	 // CI_RunModelXIP(pvModel, pImg + 8, g_out);
	#endif 
	uint32_t outType = pCtx->hdr.outType;
	uint32_t t0, t1;
	t0 = g_tick;
	for (i = 0; i < 100000; i++) 
	{
		pImg = (const char*) TestVecPopNext(pCtx);
		// pImg = GetTestSample(i);
		if (0 == pImg)
			break;
		#ifdef SRC_DEBUG
		uint8_t *pOut;
		int max, maxAt;
		pOut = (uint8_t*)RunModel(pImg + 8);
		memcpy(g_out, pOut, 128);
		int16_t *pOut16 = (int16_t*)g_out;
		pOut = (int8_t *)g_out;
		for (j = 0, max = -128; j < pCtx->hdr.outDim; j++) 
		{
			if (pOut[j] > max) 
			{
				max = pOut[j];
				maxAt = j;
			}
		}
		g_out[0] = maxAt;
		outType = outtype_cls;
		#else
		const CI_OutMetricInfo* pOM = CI_GetOutMetricInfo(0);
		//CI_RunModelXIP_NoCopyOutput(pvModel, pImg + 8, (void**)&g_out);
		CI_RunModelXIP(pvModel, pImg + 8, g_out);
	  
		#endif
		int16_t g_servoValue;
		if(inf.quantBits>7)
		{
			int16_t *g_out16 = (int16_t *)g_out;
			g_servoValue = g_out16[0] >> (inf.quantBits - pOM->fracBits - 1);
			g_servoValue = g_out[0]>>3;
		}
		else
		{
			g_servoValue = g_out[0];
		}

		//PRINTF("%04d %d got %d -- %d\r\n", i + 1, label,(g_out16[0]), avg_mse);
		avg_mse += (pImg[4] - g_servoValue) * (pImg[4] - g_servoValue);
	}
	 
	 
	t1 = g_tick - t0;
	j = (10000 * okCnt) / i;
	int printOvhd = 0;
	avg_mse=sqrt(avg_mse/i);
	j = avg_mse;
	//t1 = t1 / i;
	PRINTF("%ddms, average %d.%02ddms/inference %d\r\n", t1, t1 / i - printOvhd, (t1 * 100 / i) % 100, i);
	
	PRINTF("%05d\r\n", j);
	return j;
}


#define SYSTICK_PRESCALE 10

#define SMOTOR_CENTER   3710//舵机中值
#define SMOTOR_RANGE    400	//舵机极限值

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
int16 smotor_angle;			//舵机控制变量

uint8 send_buff[20];		//无线转串口发送数组

uint8 collection_succes;	//1：ad数据采集完成   0：ad数据未采集完成
int8 ad1,ad2,ad3,ad4,ad5,ad6,ad7,ad8;//ad数据
int16 encoder_L,encoder_R;//编码器

void smotor_control(void)
{
	smotor_angle = (int16)limit(smotor_angle,SMOTOR_RANGE);//限幅保护避免舵机出现卡死 导致舵机烧毁
    //输出控制量
    pwm_duty(PWM4_MODULE3_CHA_C31,SMOTOR_CENTER+smotor_angle);
}

void ad_collection(void)
{
	    ad1  = adc_mean_filter(POWER_ADC1_MOD  ,POWER_ADC1_PIN ,5);//B15	车头 1
      ad2  = adc_mean_filter(POWER_ADC10_MOD ,POWER_ADC10_PIN,5);//B20	车头 2
      ad3  = adc_mean_filter(POWER_ADC7_MOD  ,POWER_ADC7_PIN ,5);//B22	车头 3
      ad4  = adc_mean_filter(POWER_ADC11_MOD ,POWER_ADC11_PIN,5);//B21	车头 4
	    ad5  = adc_mean_filter(POWER_ADC0_MOD  ,POWER_ADC0_PIN ,5);//B14	车头 5
      ad6  = adc_mean_filter(POWER_ADC14_MOD ,POWER_ADC14_PIN,5);//B28	车身 1
			ad7  = adc_mean_filter(POWER_ADC12_MOD ,POWER_ADC12_PIN,5);//B30	车身 2
	
	collection_succes = 1;
}

int8 cie_data[7];
void cie_data_get(void)
{
	cie_data[0] = ad1 - 128;
	cie_data[1] = ad2 - 128;
	cie_data[2] = ad3 - 128;
	cie_data[3] = ad4 - 128;
	cie_data[4] = ad5 - 128;
	cie_data[5] = ad6 - 128;
	cie_data[6] = ad7 - 128;
}

//*************************************************main函数***********************************************************************
int main(void)
{
		int16 temp;
	
		DisableGlobalIRQ();
    board_init();//务必保留，本函数用于初始化MPU 时钟 调试串口
    EnableGlobalIRQ(0);
	
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
		
		lcd_init();
		
    pwm_init(PWM4_MODULE3_CHA_C31,50,3710);
		pit_init(); 

		motor_init();
		encoder_init();
	SysTick_Config(CLOCK_GetFreq(kCLOCK_CoreSysClk) / (SYSTICK_PRESCALE * 10000U));
	
	const CI_OutMetricInfo* pOM = CI_GetOutMetricInfo(0);
	ModelInfo_t inf;
	CI_GetModelInfoXIP(model1, &inf);
    while (1)
    {
		//AD采集部分最好是放入周期中断  保证调用的周期是固定的
			ad_collection();
		
		//如果采集完成调用CI_RunModelXIP函数进行计算
			if(collection_succes)
				{
					collection_succes = 0;
					cie_data_get();//获取RunModel函数所需的参数
					CI_RunModelXIP(model1, cie_data, &temp);//调用模型计算转角
			
			//根据获取到的模型参数对计算结果进行右移位
					int16_t g_servoValue;
					if(inf.quantBits>7)
					{
						int16_t *g_out16 = (int16_t *)g_out;
						g_servoValue = temp >> (inf.quantBits - pOM->fracBits - 1);
						g_servoValue = temp>>3;
					}
					else
					{
						g_servoValue = temp;
					}
		
			smotor_angle = g_servoValue * (int32)450 / 128;//放大数据
			smotor_control();
			motor_ctrl();
			lcd_showint16(0,0,cie_data[0]);
			lcd_showint16(0,1,cie_data[1]);
			lcd_showint16(0,2,cie_data[2]);
			lcd_showint16(0,3,cie_data[3]);
			lcd_showint16(0,4,cie_data[4]);
			lcd_showint16(0,5,cie_data[5]);
			lcd_showint16(0,6,cie_data[6]);

        }
    }
}


#define PROFILING	1
#ifdef 	PROFILING

#define PROF_CNT		20
#define PROF_ERR		16
#define PROF_MASK 		(~(PROF_ERR - 1))
#define PROF_HITCNT_INC	100
// whether hitCnt should decay, faster decay makes most time consuming functions
// seems to have even more hit count
#define PROF_DECAY	
uint32_t s_ignrList[] = {
	0,
	// (uint32_t)sensor_snapshot + 0x40 ,
};

typedef struct {
	uint32_t baseAddr;	// (aligned) base address range of PC sample
	uint32_t hitCnt;    // hit count (a decay mecahnism automatically drops it)
	uint32_t hitRatio;	// 10-bit resolution hit ratio, 
	uint32_t rsvd;		// reserved for better view in memory window
} ProfUnit_t;

typedef struct {
	uint8_t decayNdx;  // which item to decay its hitCnt 
	uint32_t profCnt;  // totoal hit count of profiling
	ProfUnit_t items[PROF_CNT];
}Prof_t;
Prof_t s_prof;

void _ProfOnHit(ProfUnit_t *pItem, uint32_t pc) 
{
	pItem->baseAddr = pc & PROF_MASK;
	s_prof.profCnt+= PROF_HITCNT_INC;
	pItem->hitCnt += PROF_HITCNT_INC;
	pItem->hitRatio = 
		(uint32_t)(((uint64_t)(pItem->hitCnt) << 10) / s_prof.profCnt);
	// sort items descending
	ProfUnit_t tmpItem;	
	for (;pItem != s_prof.items && pItem[0].hitCnt > pItem[-1].hitCnt; pItem--) 
	{
		tmpItem = pItem[0]; pItem[0] = pItem[-1] ; pItem[-1] = tmpItem;
	}
}
void ProfUpdateRate(float res)
{
  uint32_t i; 
  float profCnt = s_prof.profCnt;
  ProfUnit_t *pItem = s_prof.items;
  for (i=0; i<PROF_CNT; i++, pItem++) {
	pItem->hitRatio = 
	  (uint32_t)( (float)(pItem->hitCnt) * res / profCnt);
  }
}
void ProfReset(void)
{
  memset(&s_prof, 0, sizeof(s_prof));
}
void Profiling(uint32_t pc) 
{
	uint32_t i;
	ProfUnit_t *pItem = &s_prof.items[0];
	// filter ignore list
	for (i=0; i<ARRAY_SIZE(s_ignrList); i++) {
		if (pc - s_ignrList[i] < PROF_ERR)
			return;
	}
	#ifdef PROF_DECAY
		// apply decaying, we do not decay to zero, this means PC samples do not
		// get removed automatically, only can be pushed by later new more frequent
		// hit PC samples 
		if (s_prof.items[s_prof.decayNdx].hitCnt > 1) {
			s_prof.items[s_prof.decayNdx].hitCnt--;
			s_prof.profCnt--;
		}
		if (++s_prof.decayNdx == PROF_CNT)
			s_prof.decayNdx = 0;
	#endif	
	uint32_t freeNdx = PROF_CNT;
	// >>> traverse to search existing same PC sample
	for (i=0, pItem = s_prof.items; i<PROF_CNT; i++, pItem++) {
		if (pItem->baseAddr == (pc & PROF_MASK)) {
			_ProfOnHit(pItem, pc);
			break;
		} else if (freeNdx == PROF_CNT && pItem->hitCnt == 0){
			// record empty item, in case need to use for new PC sample
			freeNdx = i;
		}
	}
	if (i == PROF_CNT) {
		if (freeNdx < PROF_CNT) {
			// does not find, allocate for new
			_ProfOnHit(s_prof.items + freeNdx, pc);
		} else {
		  // replace the last one. We must give new samples chance to compete to
		  // get into the list.
		  freeNdx = PROF_CNT - 1;
		  s_prof.profCnt -= s_prof.items[freeNdx].hitCnt;
		  s_prof.items[freeNdx].hitCnt = 0;
		  _ProfOnHit(s_prof.items + freeNdx, pc);
		}
	}
}

#endif
typedef struct {
    uint32_t    r0, r1, r2, r3, r12, lr, pc, xpsr;
} ExceptionRegisters_t;


volatile uint32_t s_prescale;
void SysTick_C_Handler(ExceptionRegisters_t *regs) {
    extern uint32_t uwTick;
	
	#if SYSTICK_PRESCALE > 1
	if (++s_prescale < SYSTICK_PRESCALE)
		return;
	s_prescale = 0;
	#endif
	
	#ifdef PROFILING
	Profiling(regs->pc);
	#endif	
	g_tick++;
}



#if (__CC_ARM)
__asm void SysTick_Handler(void) 
{
	IMPORT	SysTick_C_Handler

	PRESERVE8
	tst lr, #4 
    ite eq 
	mrseq r0, msp
	mrsne r0, psp	
	push   {lr}
	bl SysTick_C_Handler  // Off to C land
	pop    {lr}
	bx	   lr
}

#else

void SysTick_Handler(void)
{
    __asm (
		".globl SysTick_C_Handler\n"
		".align 3				\n"
        "tst lr, #4             \n"
        "ite eq              	\n"
        "mrseq r0, msp       	\n"
		"mrsne r0, psp	        \n"
		"push   {lr}	        \n"
		"bl SysTick_C_Handler	\n"
		"pop    {lr}	       	\n"
		"bx	   lr				\n"
    );
}

#endif
