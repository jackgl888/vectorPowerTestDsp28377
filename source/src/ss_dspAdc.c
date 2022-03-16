#include "ss_include.h"


interrupt void adca1_isr(void);






/*********************************************************************************
 *  name :void ConfigureADC(void)
 *  funtion : init adc0
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

void adcInit(void)
{
   
    Uint16 i ,j;
	EALLOW;
	
	
	   (*Device_cal)();

	//write configurations
	AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

	//Set pulse positions to late
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	//power up the ADC
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	//delay for 1ms to allow ADC time to power up
	DELAY_US(1000);


	/*娉ㄥ唽adc涓柇*/
  //  PieVectTable.ADCA1_INT = &adca1_isr;
	
    /*  set   epwm0    for trigger  source  of  adc */
    SetupADCEpwm();
    

	EDIS;

	
	for(j=0;j<ADCNUM;j++)
		{
			for( i = 0; i < ADCVAL_BUFFER_SIZE;i++)
			ssSystem.dspAdc.adcValue[j][i] = 0;  
		}
	  
		ssSystem.dspAdc.resultsIndex = 0;
		ssSystem.dspAdc. bufferFull = 0;

   
}

/*********************************************************************************
 *  name :void SetupADCEpwm(Uint16 channel)
 *  funtion : set pwm0 for trigger source of adc0
 *  inter para :channel   
 *  out para :
 *  note:
 ********************************************************************************/

void SetupADCEpwm(void)
{
    Uint16 acqps;

    //determine minimum acquisition window (in SYSCLKS) based on resolution
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION){
        acqps = 14; //75ns
    }
    else { //resolution is 16-bit
        acqps = 63; //320ns
    }


    //Select the channels to convert and end of conversion flag
//    EALLOW;
//    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert pin A0
//    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
//    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C
//    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
//    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
//    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
//
//    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;  //SOC1 will convert pin A1
//    AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
//    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C
//    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
//    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
//    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
//
//	 AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;  //SOC1 will convert pin A2
//    AdcaRegs.ADCSOC2CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
//    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C
//    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
//    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
//    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
//
//    EDIS;


	   EALLOW;

	AdcaRegs.ADCINTSEL1N2.bit.INT1CONT		= 0;// enable ADCINT1 Continuous mode

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS     = 1;// ADCINT1 trips after AdcResults latch
    AdcaRegs.ADCINTSEL1N2.bit.INT1E         = 1;// Enabled ADCINT1
    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT      = 0;// Disable ADCINT1 Continuous mode
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL       = 2;// 閫氶亾2瀹屾垚杞崲瑙﹀彂涓柇ADCINT1

    AdcaRegs.ADCSOC0CTL.bit.CHSEL        = 0;//璁剧疆SOC0鐨勮浆鎹㈤�氶亾涓篈DCINA1(杈撳叆鐢靛帇)
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL      = 5;//璁剧疆SOC0鐨勮Е鍙戞簮涓篍PWM1A
    AdcaRegs.ADCSOC0CTL.bit.ACQPS        = 11;//璁剧疆SOC0鐨勯噰鏍风獥涓�(11+1)涓懆鏈�

    AdcaRegs.ADCSOC1CTL.bit.CHSEL        = 1;//璁剧疆SOC1鐨勮浆鎹㈤�氶亾涓篈DCINA2(杈撳嚭鐢靛帇)
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL      = 5;//璁剧疆SOC1鐨勮Е鍙戞簮涓篍PWM1A
    AdcaRegs.ADCSOC1CTL.bit.ACQPS        = 11;//璁剧疆SOC1鐨勯噰鏍风獥涓�(11+1)涓懆鏈�

    AdcaRegs.ADCSOC2CTL.bit.CHSEL        = 2;//璁剧疆SOC1鐨勮浆鎹㈤�氶亾涓篈DCINA3(杈撳嚭鐢垫祦)
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL      = 5;//璁剧疆SOC1鐨勮Е鍙戞簮涓篍PWM1A
    AdcaRegs.ADCSOC2CTL.bit.ACQPS        = 11;//璁剧疆SOC1鐨勯噰鏍风獥涓�(11+1)涓懆鏈�

	AdcaRegs.ADCINTSOCSEL1.bit.SOC0 = 2;
	   AdcaRegs.ADCINTSOCSEL1.bit.SOC1 = 2;
	   AdcaRegs.ADCINTSOCSEL1.bit.SOC2 = 2;

    EDIS;

    EPwm1Regs.ETSEL.bit.SOCAEN   = 1;            // 浣胯兘SOCA瑙﹀彂ADC杞崲
    EPwm1Regs.ETSEL.bit.SOCASEL  = ET_CTR_ZERO;  // 鍦–TR = PRD鏃惰Е鍙�
    EPwm1Regs.ETPS.bit.SOCAPRD   = 3;            // 姣忓彂鐢�2娆¤Е鍙戜簨浠讹紝浜х敓涓�娆DC鍚姩淇″彿


}



/*********************************************************************************
 *  name :interrupt void adca1_isr(void)
 *  funtion : adc interrupt handle
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

interrupt void adca1_isr(void)
{

    static Uint16 index =0;
   
    ssSystem.dspAdc.adcValue[OUT_CUR][index] =AdcaResultRegs.ADCRESULT0 ;    
    ssSystem.dspAdc.adcValue[BAT_VOL][index] =AdcaResultRegs.ADCRESULT1;
	ssSystem.dspAdc.adcValue[PORT_VOL][index] =AdcaResultRegs.ADCRESULT2;
    	 

    if(ADCVAL_BUFFER_SIZE <= index)
    {
       index =0;
       ssSystem.dspAdc.bufferFull = 1;
    }
    index++;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
	// AdcaRegs.ADCTRL2.bit.SOC_SEQ1 = 1
	

}




/*********************************************************************************
 *  name :    void  GetConvertedValueOfAdc(void)
 *  funtion : Gets the converted value of the ADC
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

void  GetConvertedValueOfAdc(void)
{
	 static Uint16 index =0;

     if(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 1)
     {
        static Uint16 index =0;
   
	    ssSystem.dspAdc.adcValue[OUT_CUR][index] =AdcaResultRegs.ADCRESULT0 ;    
	    ssSystem.dspAdc.adcValue[BAT_VOL][index] =AdcaResultRegs.ADCRESULT1;
		ssSystem.dspAdc.adcValue[PORT_VOL][index] =AdcaResultRegs.ADCRESULT2;
	    	 

	    if( ARRAY_FILTER_SIZE  <= index)
	    {
	       index =0;
	       ssSystem.dspAdc.bufferFull = 1;    
	    }
	    index++;
	    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag

	    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
     }
}


