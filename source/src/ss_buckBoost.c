#include "ss_minclude.h"

/************************************************************
 * @name锛歷oid buckBoostCtrlInit(void)
 * @function锛歜uckboost mode init
 * @input锛歯one
 * @output锛歯one
 * @note:
 ************************************************************/
void buckBoostCtrlInit(void)
{
#if 1
    /*	PID绠楁硶鍒濆鍖�	 */

    ssSystem.piFunc.reset(&ssSystem.volPiOut);
    ssSystem.volPiOut.T = (2.50f); //PID��������Ϊ2.5ms
    ssSystem.volPiOut.Kp = (0.04f);
    ssSystem.volPiOut.Ti = (4.50f);
    ssSystem.volPiOut.OutMin = (-0.05f * DP_CURRENT_OUT_MAX*30 ); //鏈�灏忕數娴侀檺鍒�?
    ssSystem.volPiOut.OutMax = (+1.00f * DP_CURRENT_OUT_MAX*30 ); //鏈�澶х數娴侀檺鍒�?
    ssSystem.piFunc.init(&ssSystem.volPiOut);

    ssSystem.piFunc.reset(&ssSystem.curPiOut);
    ssSystem.curPiOut.T = (2.50f); //PID鎺у埗鍛ㄦ湡锛屽崟浣�100us
    ssSystem.curPiOut.Kp = (2.15f);
    ssSystem.curPiOut.Ti = (0.85f);
    ssSystem.curPiOut.OutMin = (DUTYCYCLEMIN); //鏈�灏忓崰绌烘瘮闄愬�?鍏朵�?3涓篽rpwm缁嗗垎涓暟
    ssSystem.curPiOut.OutMax = (DUTYCYCLEMAX); //鏈�澶у崰绌烘瘮闄愬埗,鍏朵�?3涓篽rpwm缁嗗垎涓暟
    ssSystem.piFunc.init(&ssSystem.curPiOut);

    ssSystem.baseCmd.VoRefSet = DEFAULTVOLOUT;
    ssSystem.baseCmd.IoRefSet = DEFAULTCUROUT;

    /* turn off   charge switch */
    CHARGE_SWITCH_ON();

#endif

}

/************************************************************
 * @name锛歷oid dp_BuckCmcControl(void)
 * @function锛欳urrent and voltage double closed loop  ctrl  mode
 * @input锛歯one
 * @output锛歯one
 * @note:
 ************************************************************/
void buckBoostCmcCtrl(void)
{

    static int32 m_PwmDuty = 0;
    static Uint16 m_LoopCnt = 0; //Statistics the number of pid cycles

    //    static Uint16  oocp_timecount = 0;
    //    static Uint16  iocp_timecount = 0;
    //    static Uint16  ovp_timecount  = 0;
    //    static Uint16  uvp_timecount  = 0;

    /*data filtering*/
    filterAdcChannels();


    /*Convert to voltage and current  value*/
    // if(  ssSystem.fuction.bit.RDY==4)
    //  {
    ssSystem.inCurInfo.sampleVal=  (ssSystem.adcFilter.channelValAvg[OUT_CUR]/(float32)4096 )*3;
    /* calc vol &cur  */
    ssSystem.inCurInfo.Value = -(ssSystem.inCurInfo.sampleVal-1-AMP_REF)*1000/AMP_FACTOR/RES_VAL+0.3;

    if(ssSystem.inCurInfo.Value > 7)
        EPwm1Regs.TZFRC.bit.OST =1;   //ǿ���ж�PWM

    //    absVal1=abs((0.436- curValue));
    //    absVal2=absVal1*ssSystem.inCurInfo.Coeff;
    //    if(curValue > 0.436)
    //    ssSystem.inCurInfo.Value=abs( (absVal2 +1.576));
    //    else
    //    ssSystem.inCurInfo.Value=abs( (absVal2 -1.576));

    //  }
    //    else
    //        ssSystem.outCurInfo.Value = ssSystem.outCurInfo.Coeff
    //        * ssSystem.adcFilter.channelValAvg[OUT_CUR];

    ssSystem.batVolInfo.Value = ssSystem.batVolInfo.Coeff
            * ssSystem.adcFilter.channelValAvg[BAT_VOL];
    ssSystem.portVolInfo.Value = (ssSystem.portVolInfo.Coeff
            * ssSystem.adcFilter.channelValAvg[PORT_VOL]) / 0.16;

    /*Statistics the number of overflows*/
    //     ssSystem.outCurInfo.overCurTimes =  ssSystem.outCurInfo.overCurTimes > DP_CURRENT_OUT_MAX ? ++ssSystem.outCurInfo.overCurTimes : 0;
    //
    //	 /*overflows   protection*/
    // 	 if( ssSystem.outCurInfo.overCurTimes > 20)
    //	 {
    //    dr_Epwm1Upate(0);
    //	 }
    // m_PwmDuty = ssSystem.pwmPara.calDuty = 0;
    if (ssSystem.fuction.bit.RUN == 1)
    {
        /*for ost  TZ*/
        //	  EPwm1Regs.TZFRC.bit.OST =1;
        ssSystem.fuction.bit.RUN = 0;
        if (ssSystem.fuction.bit.CV == 1)
            pi_init(&ssSystem.volPiOut);
        else if (ssSystem.fuction.bit.CC == 1)
            pi_init(&ssSystem.curPiOut);
    }

    /*change */
    if (ssSystem.fuction.bit.EN == 1)
    {
        /*clear ost flag */
        // 	  EPwm1Regs.TZCLR.bit.OST = 1;
        if (m_PwmDuty % 2 == 0)
            CHARGE_SWITCH_ON();
        else
            CHARGE_SWITCH_OFF();
        //  if(ssSystem.portVolInfo.Value >=ssSystem.batVolInfo.Value)
        //  	{
        /*output fixed  vol */
        EPwm1Regs.CMPA.bit.CMPA = ssSystem.pwmPara.cmpValueHi;  //duty cycle
        ssSystem.fuction.bit.EN = 0;
        //	}
        m_PwmDuty++;
    }

#if 1

    switch (1)//m_LoopCnt
    //
    {
    case 0:
        if (ssSystem.fuction.bit.CV == 1)
        {
            ssSystem.volPiOut.Ref = ssSystem.baseCmd.VoRefSet;
            ssSystem.volPiOut.Fdb = ssSystem.batVolInfo.Value;
            ssSystem.piFunc.calc(&ssSystem.volPiOut); //
        }
        break;

    default:
        if (ssSystem.fuction.bit.CC == 1)
        {
//            ssSystem.curPiOut.Ref =   ssSystem.volPiOut.Output > ssSystem.baseCmd.IoRefSet ? ssSystem.baseCmd.IoRefSet : ssSystem.volPiOut.Output;
//            ssSystem.curPiOut.Fdb =   ssSystem.outCurInfo.Value;
//            ssSystem.piFunc.calc( &ssSystem.curPiOut);  //鐢垫祦鐜疨ID璁＄�?



//#if  CC_ONLY
            ssSystem.curPiOut.Ref = ssSystem.baseCmd.IoRefSet;
            //   ssSystem.curPiOut.Fdb = ssSystem.outCurInfo.Value;
            ssSystem.curPiOut.Fdb = ssSystem.inCurInfo.Value;
            ssSystem.piFunc.calc(&ssSystem.curPiOut);              //鐢垫祦鐜疨ID璁＄�?
//#endif
        }
        //ssSystem.pwmPara.calDuty  = ((int64)ssSystem.curPiOut.Output*70 + (int64)ssSystem.pwmPara.calDuty *30)/100;//涓�闃舵护娉紝70 + 30 = 100;绛夋�?.7 + 0.3 = 1
        break;
    }               //switch(..)
//    m_LoopCnt  = ++m_LoopCnt >= 4 ? 0 : m_LoopCnt;  //鐜矾鎺у埗璁℃暟
//    m_PwmDuty = ssSystem.pwmPara.calDuty;

    if (ssSystem.fuction.bit.CC == 1)
    {
        m_PwmDuty = ssSystem.curPiOut.Output;

        dr_Epwm1Upate(m_PwmDuty);
    }

    else if (ssSystem.fuction.bit.CV == 1)
    {

        m_PwmDuty = ssSystem.volPiOut.Output;

        dr_Epwm1Upate(m_PwmDuty);
    }

#endif

    //  m_PwmDuty=ssSystem.volPiOut.Output;

    //  dr_EpwmsClrTZ( &EPwm1Regs);//娓呴櫎PWM寮哄�?鎭㈠PWM姝ｅ父杈撳嚭

}

