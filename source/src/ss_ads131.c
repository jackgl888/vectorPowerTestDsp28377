#include "ss_minclude.h"



interrupt void xint_isr(void);

 Uint16 wrDate[][2]=
{ 
 	      {0x93,CONFIG1},                  // 24BIT  8KPS       SCLK 1.75M
 	      //{0xF3,CONFIG2},                //test signal creat internal
 	      {0x4C,CONFIG3},               // enable op amp  noninverting connect to (AVDD +AVSS )/2
 	      {0x10,CH1SET},               // 
		  {0x10,CH2SET},				 // 
		  {0x10,CH3SET},
		  {0x10,CH4SET},   
		  {0x10,CH5SET},
		  {0x10,CH6SET},
		  {0x10,CH7SET},
		  {0x10,CH8SET},//pga =1 
};




/*********************************************************************************
 *  name :void spiGpioInit(void)
 *  funtion : init spi gpio
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

void spiGpioInit(void)
{
      EALLOW;

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
//  GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;    // Enable pull-up on GPIO5 (SPISIMOA)
    GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
//  GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;    // Enable pull-up on GPIO3 (SPISOMIA)
    GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
    GpioCtrlRegs.GPBPUD.bit.GPIO59 = 0;   // Enable pull-up on GPIO19 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3; // Asynch input GPIO16 (SPISIMOA)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;  // Asynch input GPIO5 (SPISIMOA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3; // Asynch input GPIO17 (SPISOMIA)
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;  // Asynch input GPIO3 (SPISOMIA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3; // Asynch input GPIO18 (SPICLKA)
//    GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3; // Asynch input GPIO19 (SPISTEA)

/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBGMUX2.bit.GPIO60 = 1; // Configure GPIO16 as SPISIMOA
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 2;
//  GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;  // Configure GPIO5 as SPISIMOA
    GpioCtrlRegs.GPBGMUX2.bit.GPIO61 = 1; // Configure GPIO17 as SPISOMIA
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 2;
//  GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 2;  // Configure GPIO3 as SPISOMIA
    GpioCtrlRegs.GPBGMUX2.bit.GPIO58 = 1; // Configure GPIO18 as SPICLKA
    GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 2;
//    GpioCtrlRegs.GPBGMUX2.bit.GPIO59 = 1; // Configure GPIO19 as SPISTEA
    GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;
    GpioDataRegs.GPBSET.bit.GPIO59 = 1;

    EDIS;
}






///*********************************************************************************
// *  name :interrupt void xint_isr(void)
// *  funtion :    adc invert  ok
// *  inter para :
// *  out para :
// *  note:
// ********************************************************************************/
//
//interrupt void xint_isr(void)
//{
//   static Uint16 temp = 0;
//   temp+=1;
//}

void spiInitialization(void)
{    
    spiGpioInit();

	SpibRegs.SPICCR.all =0x0007;	    //Reset off, rising edge, 8-bit char bits
	SpibRegs.SPICTL.all =0x000E;        //Enable master mode, delayed phase,
                                        //enable talk, and SPI int disabled.
	SpibRegs.SPIBRR.all =SPI_BRR ;
    SpibRegs.SPICCR.all =0x0087;		//Relinquish SPI from Reset
    SpibRegs.SPIPRI.bit.FREE = 1; 		//Transmission not affected by emulator
}



/*********************************************************************************
 *  name :void spiInit(void)
 *  funtion : init spi 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
void spiInit(void)
{

      /*init spi gpio*/
      spiGpioInit();
    
    // Initialize SPI-A

    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 16-bit character
    // Enable loop-back
    SpibRegs.SPICCR.bit.SPISWRESET = 0;
    SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpibRegs.SPICCR.bit.SPICHAR = (8-1);
    SpibRegs.SPICCR.bit.SPILBK = 0;

    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpibRegs.SPICTL.bit.TALK = 1;
    SpibRegs.SPICTL.bit.CLK_PHASE = 1;
    SpibRegs.SPICTL.bit.SPIINTENA = 0;

    // Set the baud rate
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = SPI_BRR;

    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    SpibRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset`
    SpibRegs.SPICCR.bit.SPISWRESET = 1;


	    // Initialize SPI FIFO registers
    SpibRegs.SPIFFTX.all = 0xE040;
    SpibRegs.SPIFFRX.all = 0x2044;
    SpibRegs.SPIFFCT.all = 0x0;

}



/*********************************************************************************
 *  name :void spiXmitByte(void)
 *  funtion : spi transmit one byte 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
Uint16  spiXmitByte(Uint16 byte)
{
  	SpibRegs.SPITXBUF = byte;         			//Transmit Byte
 	while(SpibRegs.SPIFFRX.bit.RXFFST !=1) { };
  	//	while(SpibRegs.SPISTS.bit.INT_FLAG != 1); 	//Wait until the RXBUF has received last bit
  	return (SpibRegs.SPIRXBUF);			//Read Byte from RXBUF and return
}






/*********************************************************************************
 *  name :void  ads131e08GpioInit(void)
 *  funtion : ads131e08 gpio init 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
void  ads131e08GpioInit(void)
{
      

	   	EALLOW;
		PieVectTable.XINT1_INT =&xint_isr ;
		EDIS;
	
		
		EALLOW;
		GpioCtrlRegs.GPAMUX2.bit.GPIO20= 0;
	/*DRDY	signal check  gpio*/
	   /*enable  pull up */
//	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;
//	   /*set input*/
		GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;

	   /*async*/
		GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 2;

	  EDIS;

	    GPIO_SetupXINT1Gpio(20);
		XintRegs.XINT1CR.bit.POLARITY = 1;
		XintRegs.XINT1CR.bit.ENABLE =1  ;
		
	  /*RESET pin init */
	   /*enable  pull up */
		GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;
	   /*set output*/
		GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;
		GpioCtrlRegs.GPBMUX2.bit.GPIO63= 0;
	
		/*START pin init */
		  /*enable	pull up */
		GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;
		   /*set output*/
		GpioCtrlRegs.GPBDIR.bit.GPIO62 = 1;
		GpioCtrlRegs.GPBMUX2.bit.GPIO62= 0;

		
}




/*********************************************************************************
 *  name :void ads131e08WriteReg(Uint16 addr, Uint16 val)
 *  funtion :write ads131e08 regster 
 *  inter para :cmd :   cmd for write reg  , val : val  write in addr   
 *  out para :
 *  note:
 ********************************************************************************/

void ads131e08WriteReg(Uint16 cmd, Uint16 val)
{
         /*write cmd */
         spiXmitByte(cmd);

		 /* write number of reg*/
         spiXmitByte(0);

		 /*write in data*/
		 spiXmitByte(val);
		 
	 	
}




/*********************************************************************************
 *  name :Uint16 ads131e08ReadReg(Uint16 addr)
 *  funtion : read ads131e08 regster 
 *  inter para :cmd :   cmd for write reg  , val : val  write in addr   
 *  out para :
 *  note:
 ********************************************************************************/

void ads131e08ReadReg(Uint16 addr,Uint16 num,Uint16 *valBuf)
{
      Uint16 i=0;
	   /*cmd &addr */
       spiXmitByte(addr);
       /*nnumber*/
	   spiXmitByte(num-1);

	   for(i=0;i<num;i++)
	   	{
	   	      valBuf[i]=spiXmitByte(0);
			  DELAY_US(10);
	   	}
	   	
}


/*********************************************************************************
 *  name :void  getAdcInvertedData(void);
 *  funtion :get inverted adc channels data
 *  inter para :cmd :   cmd for write reg  , val : val  write in addr   
 *  out para :
 *  note:
 ********************************************************************************/
 void  getAdcInvertedData(void)
{
      Uint16 i=0,j=0,k=0;
	  Uint32 valSum=0,valAvg=0;
	  
     /*read ouut adc value*/
	 for(k=0;k<ADC_CONVERT_CNT;)
	 {
	      ssSystem.time =3;
		  while(ssSystem.time)
		  {
		        if(ADS131_DATA_READY()==TRUE)    
		        {
	              /*read adc data*/
			       for(i = 0;i<27;i++)//read 8 channels data
			       	{
			       	  ssSystem.ads131Para.readBuf[i] = spiXmitByte(0);
			       	}
				   for(j = 0;j<9;j++)
				   	{
				   	     ssSystem.ads131Para.adcBuf[j]=(Uint32)(ssSystem.ads131Para.readBuf[0+3*j]<<16| ssSystem.ads131Para.readBuf[1+3*j]<<8|ssSystem.ads131Para.readBuf[2+3*j]);


					     if(j>0)
						 ssSystem.ads131Para.chnData[j-1][k]= ssSystem.ads131Para.adcBuf[j];
					   	
				   	}
		          break;
		        }
				else
				{
				   //read out old value
				}
		  }
         k++;	     
	 }

	 /*filter adc value*/
	for(j=0;j<MAX_ADC_CHN;j++)
	{
	     for(k=0;k<ADC_CONVERT_CNT;k++)
		 ssSystem.ads131Para.singleChnData[k]=ssSystem.ads131Para.chnData[j][k]    ;

		 arrayBubbleSorting((Uint16*)ssSystem.ads131Para.singleChnData,ADC_CONVERT_CNT);

		 for(i=ARRAY_STARTING_POS ;i<ARRAY_ENDING_POS;i++)
		 {
		     valSum+=ssSystem.ads131Para.singleChnData[i];
		 }
		 valAvg=valSum/ARRAY_AVG_LEN;
		 
		 ssSystem.adcFilter.channelValAvg[j] =valAvg;
		 valAvg=0;
		 valSum=0;
	}
    
}


/*********************************************************************************
 *  name :void ads131e08Init(void)
 *  funtion : ads131e08 init 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
void ads131e08Init(void)
{
     Uint16 i=0;
	 Uint16  regBuf[15]={0};
     /*spi init */
   // spiInit();
    spiInitialization();
    /*ads 131 gpio init */
    ads131e08GpioInit();

	ADS131_CS_L() ;  
    /*start signal */
	 ADS131_START_L();
		 DELAY_US(200);
    /*reset*/
//     ADS131_RESET_L();
//	 DELAY_US(50000);
//     ADS131_RESET_H();
   spiXmitByte( ADS131E08_RESET );
   DELAY_US(50000);
     ADS131_START_H();
	 DELAY_US(500); 

	/*stop   continus mode */
     spiXmitByte(ADS131E08_SDATAC);
	
    writeAgain:
    /*write regster*/
   for(i = 0;i<sizeof(wrDate)/2;i++)
   	{
   	    ads131e08WriteReg(wrDate[i][1],wrDate[i][0]);
		DELAY_US(10);
   	}
 /*read id*/
   ads131e08ReadReg(0x20,12,regBuf);
   if(regBuf[1]!=wrDate[0][0])
   goto writeAgain;
	   
	/* calibration of ads131e08*/
    spiXmitByte(ADS131E08_OFFSETCAL);
    DELAY_US(10);
   /*start conversion*/
   spiXmitByte(ADS131E08_START);
   DELAY_US(10);
   /*start continus mode */
   spiXmitByte(ADS131E08_RDATAC);
   DELAY_US(10);
	 /*set  cs low*/
   ADS131_CS_L() ;  
 
	 
}







