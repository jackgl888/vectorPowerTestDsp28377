#include "ss_buckboost.h"



/************************************************************
 * @name：void buckBoostCtrlInit(void)
 * @function：buckboost mode init 
 * @input：none
 * @output：none
 * @note:
 ************************************************************/
void buckBoostCtrlInit(void)
{
   #if 0
	/*	PID算法初始化	 */
	
		ssSystem.piFunc.reset(&gPID_VoltOutLoop);
		gPID_VoltOutLoop.T		 = _IQ12(0.50f);//PID控制周期，单位1ms
		gPID_VoltOutLoop.Kp 	 = _IQ12(0.04f);
		gPID_VoltOutLoop.Ti 	 = _IQ12(4.50f);
		gPID_VoltOutLoop.OutMin  = (-0.05f * DP_CURRENT_OUT_MAX); //最小电流限制
		gPID_VoltOutLoop.OutMax  = (+1.00f * DP_CURRENT_OUT_MAX); //最大电流限制
		pi_func.init(&gPID_VoltOutLoop);
	
		pi_func.reset(&gPID_CurrOutLoop);
		gPID_CurrOutLoop.T		 = _IQ12(0.50f);//PID控制周期，单位100us
		gPID_CurrOutLoop.Kp 	 = _IQ12(2.15f);
		gPID_CurrOutLoop.Ti 	 = _IQ12(0.85f);
		gPID_CurrOutLoop.OutMin  = _IQ12(0.01f * DP_PWM_PER * 93);//最小占空比限制,其中93为hrpwm细分个数
		gPID_CurrOutLoop.OutMax  = _IQ12(0.85f * DP_PWM_PER * 93);//最大占空比限制,其中93为hrpwm细分个数
		pi_func.init(&gPID_CurrOutLoop);
	
	
		gMainCmd.VoRefSet = gDefSetVoltageOut;
		gMainCmd.IoRefSet = gDefSetCurrentOut;

		 #endif

}



/************************************************************
 * @name：void dp_BuckCmcControl(void)
 * @function：Current and voltage double closed loop  ctrl  mode 
 * @input：none
 * @output：none
 * @note:
 ************************************************************/
void buckBoostCmcCtrl(void)
{
  #if 0
    static int32   m_PwmDuty;
    static Uint16  j;
    static Uint16  m_LoopCnt      = 0;//环路控制切换时间
    static Uint16  oocp_timecount = 0;
    static Uint16  iocp_timecount = 0;
    static Uint16  ovp_timecount  = 0;
    static Uint16  uvp_timecount  = 0;


    /*  将ADC值转换为相应的电气参数  */
    gLowFilter_Vin.Input  = (int64)gStrVin.Coeff  * gAdcViSampleValue;
    gLowFilter_Vout.Input = (int64)gStrVout.Coeff * gAdcVoSampleValue;
    gLowFilter_Iout.Input = (int64)gStrIout.Coeff * (gStrIout.Offset - gAdcIoSampleValue);

    /*  对输入信号滤波,以降低抖动  */
    low_filter_calc(&gLowFilter_Vin);
    low_filter_calc(&gLowFilter_Vout);
    low_filter_calc(&gLowFilter_Iout);

    gStrVout.Value  = gLowFilter_Vout.Output;
    gStrIout.Value  = gLowFilter_Iout.Output;
    gStrVin.Value   = gLowFilter_Vin.Output;

    oocp_timecount = fabs(gStrIout.Value) > DP_CURRENT_OUT_MAX ? ++oocp_timecount : 0;//过流计时
    uvp_timecount  = gStrVin.Value        < DP_VOLTAGV_IN_MIN  ? ++uvp_timecount  : 0;//欠压计时
    ovp_timecount  = gStrVin.Value        > DP_VOLTAGV_IN_MAX  ? ++ovp_timecount  : 0;//过压计时

    if( oocp_timecount > 20 )gPSM_ALM.bit.oocp  = 1;    //过流保护
    if( iocp_timecount > 20 )gPSM_ALM.bit.iocp  = 1;    //过流保护
    if( uvp_timecount  > 20 )gPSM_ALM.bit.iuvp  = 1;    //欠压保护
    if( ovp_timecount  > 20 )gPSM_ALM.bit.iovp  = 1;    //过压保护


    if( gPSM_ALM.all != 0 )
    {
        gPSM_STA.bit.FAUT = 1;
        j   += 1; //恢复运行时间计时
        if(j >= DP_RESET_FAULT_TIME)
        {
            j = 0;
            gPSM_ALM.all = 0;
        }
    }

    gPSM_STA.bit.RUN  = !gPSM_STA.bit.FAUT && gPSM_STA.bit.RDY && gPSM_STA.bit.EN;//故障、就绪、使能


    if( gPSM_STA.bit.RUN == 0)   //运行与否？
    {
·
      m_LoopCnt = 0;

      dr_EpwmsSrcTZ(&EPwm1Regs);//软件强制PWM1输出低电平

      dp_ExcursionCheck();//输出电流零漂检测

      m_PwmDuty = 0;
      pi_func.clc(&gPID_VoltOutLoop); //清除gPID_VoltOutLoop
      pi_func.clc(&gPID_CurrOutLoop); //清除gPID_CurrOutLoop

      return;
    }


    switch( m_LoopCnt )//在中断中4次运行电流环  1次运行电压环
    {
        case 0:

          gPID_VoltOutLoop.Ref    = gMainCmd.VoRefSet;
          gPID_VoltOutLoop.Fdb    = gStrVout.Value;
          pi_func.calc( &gPID_VoltOutLoop); //输出电压环PID计算

        break;

        default:

          gPID_CurrOutLoop.Ref = gPID_VoltOutLoop.Output > gMainCmd.IoRefSet ? gMainCmd.IoRefSet : gPID_VoltOutLoop.Output;
          gPID_CurrOutLoop.Fdb = gStrIout.Value;
          pi_func.calc( &gPID_CurrOutLoop);  //电流环PID计算
          m_PwmDuty = ((int64)gPID_CurrOutLoop.Output*70 + (int64)m_PwmDuty*30)/100;//一阶滤波，70 + 30 = 100;等效0.7 + 0.3 = 1

        break;
    }//switch(..)
    m_LoopCnt  = ++m_LoopCnt >= 5 ? 0 : m_LoopCnt;  //环路控制计数

    dr_Epwm1Upate( m_PwmDuty >> 12);
    dr_EpwmsClrTZ( &EPwm1Regs);//清除PWM强制,恢复PWM正常输出


#endif
}


