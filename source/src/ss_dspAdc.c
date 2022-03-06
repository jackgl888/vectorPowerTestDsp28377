#include "ss_dspadc.h"


interrupt void adca1_isr(void);






/*********************************************************************************
 *  name :void ConfigureADC(void)
 *  funtion : init adc0
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

void ConfigureADC(void)
{
   
    Uint16 i = 0;
	EALLOW;

	//write configurations
	AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

	//Set pulse positions to late
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	//power up the ADC
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	//delay for 1ms to allow ADC time to power up
	DELAY_US(1000);


	/*注册adc中断*/
    PieVectTable.ADCA1_INT = &adca1_isr;
	
    /*  set   epwm0    for trigger  source  of  adc */
    SetupADCEpwm();

	EDIS;

	
   
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
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert pin A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;  //SOC1 will convert pin A1
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5; //trigger on ePWM1 SOCA/C
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared

    EDIS;

}


