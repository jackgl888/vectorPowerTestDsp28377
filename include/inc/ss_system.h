#ifndef __SS_SYSTEM_H__
#define __SS_SYSTEM_H__







typedef enum
{
  FALSE =0,
  TRUE = 1
}Bool;


typedef struct
{

  Uint32   time;                  //sys-time 
  DSP_ADC    dspAdc;  // 28337 adc数据结构
  ADCFILTER  adcFilter;  //filter
  PI_FUNC	 piFunc;
  PI_STRUCT  volPiOut;
  PI_STRUCT  curPiOut;
  BASE_CMD_STRUCT   baseCmd;  //下发的电流电压参数
  PWMPARA pwmPara;      
  SOFTWARETIMER   swTimer[SWT_COUNT];  //
  ELEC_INFO_STRUCT inVolInfo;  //输入电压参数
  ELEC_INFO_STRUCT batVolInfo;  //电池电压参数
  ELEC_INFO_STRUCT portVolInfo;  // port voltage
  ELEC_INFO_STRUCT inCurInfo;  //输入电流参数
  ELEC_INFO_STRUCT outCurInfo;  //输出电流参数
  
  SYSTEM_STA_STRUCT  fuction;  //功能结构
  MCLC_COMMUNICATION   communication;  //通信
  
}SS_SYSTEM;







void  paraInit(void);

void lcBoxInit(void);

void dspSystemInit(void);


extern   SS_SYSTEM  ssSystem;
#endif





