#include "ss_pwm.h"



/**
 *******************************************************************************
 *	函数:	void  epwmsInit(void)
 *	描述:	pwm初始化
 *	输入:	
 *	返回:	无
 *	作者:zwe
 *	版本:	
 *******************************************************************************
 */

void  epwmsInit(void)
{
# if 1

       /*config pwm pins*/
		InitEPwmGpio();

		// 配置定时器模块(TB)
    EPwm1Regs.TBPRD                 = SP ;   // 设置PWM周期为(TBPRD + 1)个CBCLK个时钟周期
    EPwm1Regs.TBPHS.all             = 0;                    // 清空相位寄存器
    EPwm1Regs.TBCTR                 = 0;                    // 清空时间基准计数器

    EPwm1Regs.TBCTL.bit.CTRMODE     = TB_COUNT_UPDOWN;  //配置为向上向下计数模式
    EPwm1Regs.TBCTL.bit.PHSEN       = TB_DISABLE;       //禁止相位控质，
    EPwm1Regs.TBCTL.bit.PRDLD       = TB_SHADOW;        //TBPED寄存器采用映射模式
    EPwm1Regs.TBCTL.bit.SYNCOSEL    = TB_CTR_ZERO;      //EPWM1为主模块，在CTR = ZERO时发出同步信号
    EPwm1Regs.TBCTL.bit.HSPCLKDIV   = TB_DIV1;          //设置分频器TBCLK = SYSCLKOUT / (HSPCLKDIV × CLKDIV)
    EPwm1Regs.TBCTL.bit.CLKDIV      = TB_DIV1;          //设置分频器TBCLK = SYSCLKOUT / (HSPCLKDIV × CLKDIV)

    EPwm1Regs.CMPCTL.bit.SHDWAMODE  = CC_SHADOW;        //设CMPA为映射模式
    EPwm1Regs.CMPCTL.bit.SHDWBMODE  = CC_SHADOW;        //设CMPB为映射模式
    EPwm1Regs.CMPCTL.bit.LOADAMODE  = CC_CTR_ZERO;      //在CTR = ZERO 时装载
    EPwm1Regs.CMPCTL.bit.LOADBMODE  = CC_CTR_ZERO;      //在CTR = ZERO 时装载
//配置比较方式预设模块(AQ)
    EPwm1Regs.AQCTLA.bit.CAU        = AQ_CLEAR;         // CTR = CAU时,PWM1A输出低
    EPwm1Regs.AQCTLA.bit.PRD        = AQ_CLEAR;         // CTR = PRD时,PWM1A输出低
    EPwm1Regs.AQCTLA.bit.CAD        = AQ_SET;           // CTR = CAD时,PWM1A输出高

    EPwm1Regs.AQCTLB.bit.CAU        = AQ_CLEAR;         // CTR = CAU时,PWM1B输出低
    EPwm1Regs.AQCTLB.bit.CAD        = AQ_SET;           // CTR = CAD时,PWM1B输出高
    EPwm1Regs.AQCTLB.bit.PRD        = AQ_SET;           // CTR = ZRO时,PWM1B输出高


//配置计数器比较模块(CC)
    EPwm1Regs.CMPA.bit.CMPA        = SP/3;                //设置占空比CMPx/TBPRD
    EPwm1Regs.CMPB.all                  = 0;                //设置占空比CMPx/TBPRD
//配置死区模块(DB)
    EPwm1Regs.DBCTL.bit.IN_MODE     = DBA_ALL;          //ePWMA作为上升沿信号，ePWMA作为下降沿信号
    EPwm1Regs.DBCTL.bit.OUT_MODE    = DB_FULL_ENABLE;   //使能死区模块;
    EPwm1Regs.DBCTL.bit.POLSEL      = DB_ACTV_HIC;      //对称互补
    EPwm1Regs.DBRED.bit.DBRED       = DBTIME_RED;       //上升沿死区时间
    EPwm1Regs.DBFED.bit.DBFED       = DBTIME_FED;       //下降沿死区时间
//配置故障捕获模块(TZ)
    EPwm1Regs.TZCTL.bit.TZA         = 0x02;             //强制ePWM1A为低电平
    EPwm1Regs.TZCTL.bit.TZB         = 0x02;             //强制ePWM1B为低电平

    DELAY_US(10);

//配置高分辨PWM(HrPWM)
    EALLOW;
    EPwm1Regs.ETSEL.bit.SOCAEN              = 1;            // 使能SOCA触发ADC转换
    EPwm1Regs.ETSEL.bit.SOCASEL             = 4;//ET_CTR_ZERO;  // CTR=ZERO触发
    EPwm1Regs.ETPS.bit.SOCAPRD              = 1;//ET_1ST;       // 每n次事件触发一次转换
	DELAY_US(10);

    EPwm1Regs.HRCNFG.all            = 0x00;             //复位HRCNFG
    EPwm1Regs.HRCNFG.bit.EDGMODE    = HR_FEP;           //MEP control on Rising edge
    EPwm1Regs.HRCNFG.bit.CTLMODE    = HR_CMP;           //
    EPwm1Regs.HRCNFG.bit.HRLOAD     = HR_CTR_ZERO;      //CTR = ZERO加载
    EDIS;
//            
	EPwm1Regs.CMPA.bit.CMPAHR = 0.2;	

	EPwm1Regs.TBCTL.all=0x0010+TBCTLVAL;			// Enable Timer

    DELAY_US(10);

		//配置中断模块(ET)
		//	  EPwm1Regs.ETSEL.bit.INTSEL  = ET_CTR_PRD; 	//配置中断触发信号
		//	  EPwm1Regs.ETSEL.bit.INTEN   = 0;				//使能中断位 1:使能，0:不使能
		//	  EPwm1Regs.ETPS.bit.INTPRD   = ET_2ND; 		//中断频率 = fPWM / 2
		//	  EPwm1Regs.ETCLR.bit.INT	  = 1;				//清除中断标志位
		//
		//	  IER |= M_INT3;		  // 使能PIE 的INT3
		//
		//	  PieCtrlRegs.PIECTRL.bit.ENPIE   = 1;	  //使能PIE block
		//	  PieCtrlRegs.PIEIER3.bit.INTx1   = 1;	  //使能INT3.1
#endif

 
}



/********************************************************************************
 *  函数: void dr_EpwmsSrcTZ(volatile struct EPWM_REGS *EPwmRegs)
 *  描述:   soft     forces  a single event  to be triggred  , and pwm output low;
 *  输入: EPwmRegs:  EPwm1Regs、EPwm2Regs ...
 *  返回: 无
 *  作者: 
 *  版本: 
 ********************************************************************************/
void dr_EpwmsSrcTZ(volatile struct EPWM_REGS *EPwmRegs)
{
    EALLOW;
    EPwmRegs->TZFRC.all         = 0x04;   //软件触发一次单次触发事件
    EDIS;
}

/********************************************************************************
 *  函数: void dr_EpwmsClcerTZ(volatile struct EPWM_REGS *EPwmRegs)
 *  描述:clear erorr flags
 *  输入: EPwmRegs:   EPwm1Regs、EPwm2Regs ...
 *  返回: 无
 *  作者: 
 *  版本:
 ******************************************************************************* */
void dr_EpwmsClrTZ(volatile struct EPWM_REGS *EPwmRegs)
{
    EALLOW;
    EPwmRegs->TZCLR.all =  EPwmRegs->TZFLG.all;
    EDIS;
}

/* *******************************************************************************
 *	函数: void ePWM1_update(float32 DutyA,float32 DutyB)
 *	描述:  update  PWM1  dutycycle
 *	输入: PWM_DUTY_CMP:   dutycycle value 
 *	返回: 
 *	作者: 
 *	版本:
 ******************************************************************************* */
void dr_Epwm1Upate(int32 PWM_DUTY_CMP)
{

    static int32 m_cmp = 0;

    if(PWM_DUTY_CMP > DUTYCYCLEMAX )//限幅
    {
        PWM_DUTY_CMP = DUTYCYCLEMAX ;
    }

    if( abs(PWM_DUTY_CMP - m_cmp) > 2 )m_cmp = PWM_DUTY_CMP;//去抖

	EPwm1Regs.CMPA.bit.CMPA	= (Uint16)(m_cmp / 93);
	EPwm1Regs.CMPA.bit.CMPAHR  = (Uint16)(m_cmp % 93)<<8;
}

