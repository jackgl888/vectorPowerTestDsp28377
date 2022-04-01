#ifndef __BUCKBOOST_H__
#define __BUCKBOOST_H__
#include "F28x_Project.h"


#define   CC_ONLY      1


#define DP_RESET_FAULT_TIME     40000        //鏁呴殰鎭㈠鏃堕棿,鍛ㄦ湡涓暟锛屽ぇ绾︽槸2s
#define DP_PWM_FREQUENCY        120e3        //开关频率HZ
#define DP_VOLTAGE_FREQUENCY    4e3          //鐢靛帇鐜甫瀹�,鍗曚綅HZ
#define DP_CURRENT_FREQUENCY    20e3         //鐢垫祦鐜甫瀹�,鍗曚綅HZ

#define DP_VOLTAGV_IN_MAX       (12.0f)        //最大输入电压
#define DP_VOLTAGV_IN_MIN       (1.0f)        //最大输入电压V
#define DP_VOLTAGV_OUT_MAX      (5.0f)        //最大输出电压V
#define DP_CURRENT_IN_MAX       (10.0f)        //最大输入电流A
#define DP_CURRENT_OUT_MAX      (10.0f)        //最大输出 电流A
#define DP_CURRENT_IN_OFFSET    (2048L)            //鐢垫祦閲囨牱鐨�1.65V鍋忕Щ鐢靛帇瀵瑰簲鐨凙DC閲囨牱鍊�
#define DP_CURRENT_OUT_OFFSET   (2048L)            //鐢垫祦閲囨牱鐨�1.65V鍋忕Щ鐢靛帇瀵瑰簲鐨凙DC閲囨牱鍊�

#define DP_VOLTAGE_IN_RATIO     (99.00f/4095)  //杈撳叆鐢靛帇姣斾緥 3.3*33
#define DP_VOLTAGE_OUT_RATIO    (99.00f/4095)  //杈撳嚭鐢靛帇姣斾緥 3.3*33
#define DP_CURRENT_OUT_RATIO    (15.00f/4095)  //杈撳嚭鐢垫祦姣斾緥 3.3/22/0.01,鏁版嵁鍋忓樊澶ф椂锛屾牎鍑嗚鍊�

#define DEFAULTVOLOUT    (3.8f)    //上电时输出电压
#define DEFAULTCUROUT    (2.0f)    //默认输出电流

#define    CHARGE_SWITCH_ON()      GpioDataRegs.GPACLEAR.bit.GPIO2 = 1         //充电放电控制开
#define    CHARGE_SWITCH_OFF()      GpioDataRegs.GPASET.bit.GPIO2 = 1          //充电放电控制关

#define   AMP_REF         2.064               //电流检测运算 放参考 电压
#define   AMP_FACTOR      50.4                //运算放大倍数
#define   RES_VAL         3.0                   //采样电阻阻值



typedef struct {

  float32 Value;   //实际电压电流数据
  float32 sampleVal;   //采样电压值
  float32 Coeff;   //转换为实际电压电流时的系数
  int32 Offset;  //
  Uint16 overCurTimes;  //过流次数

}ELEC_INFO_STRUCT;//鐢垫皵鍙傛暟缁撴瀯浣�



typedef struct {

  float32  VoRefSet;   // 鍔熻兘涓撻�掔殑杈撳嚭鐢靛帇璁惧畾
  float32   VoRefApp;   // 瀹為檯浣跨敤鐨勮緭鍑虹數鍘嬭瀹�
  float32   IoRefSet;   // 鍔熻兘涓撻�掔殑杈撳嚭鐢垫祦璁惧畾
  float32   IoRefApp;   // 瀹為檯浣跨敤鐨勮緭鍑虹數娴佽瀹�
  float32  ViRefSet;   // 鍔熻兘涓撻�掔殑杈撳嚭鐢靛帇璁惧畾
  float32   ViRefApp;   // 瀹為檯浣跨敤鐨勮緭鍑虹數鍘嬭瀹�
  float32   IiRefSet;   // 鍔熻兘涓撻�掔殑杈撳嚭鐢垫祦璁惧畾
  float32   IiRefApp;   // 瀹為檯浣跨敤鐨勮緭鍑虹數娴佽瀹�

}BASE_CMD_STRUCT;//瀹炴椂淇敼鐨勫懡浠ょ粨鏋勪綋

typedef union {
   Uint16 all;
   struct {
     Uint16 iuvp:  1;
     Uint16 iovp:  1;
     Uint16 ilrv:  1;
     Uint16 iocp:  1;
     Uint16 oovp:  1;
     Uint16 oocp:  1;
     Uint16 oopp:  1;
     Uint16 motp:  1;
     Uint16 cotp:  1;
     Uint16 excu:  1;//闆剁偣婕傜Щ
     Uint16 rsvd:  6;
   }bit;
}SYSTEM_ALM_STRUCT;

typedef union {
   Uint16 all;
   struct {
     Uint16 EN:     1; //使能
     Uint16 RUN:    1; //运行
     Uint16 RDY:    3; //就绪
     Uint16 FAUT:   1; //故障
     Uint16 ALM:    1; //警告
     Uint16 DIR:    1; //0：正向  1：反向
     Uint16 COM:    1; //通信连接上
     Uint16 CC:     1; //恒流动模式
     Uint16 CV:     1; //恒压模式
     Uint16 CW:     1; //恒功率模式
     Uint16 MPPT:   1; //MPPT模式
     Uint16 CHARGE: 1; //电池充电
     Uint16 DISCHA: 1; //电池放电
     Uint16 SWITCH: 1; //负载开发
     Uint16 rsvd:   1; //保留
   }bit;
}SYSTEM_STA_STRUCT;




void buckBoostCmcCtrl(void);
void buckBoostCtrlInit(void);




#endif

