#ifndef    __SS_ADS131_H__
#define   __SS_ADS131_H__
#include "F28x_Project.h"

#define ADC_CONVERT_CNT     20      //ad convert  times
#define MAX_ADC_CHN          8

typedef struct 
{
    Uint16 readBuf[30];   
	Uint32 adcBuf[10];
	Uint32 chnData[8][ADC_CONVERT_CNT ];
	Uint32 singleChnData[ADC_CONVERT_CNT ];
	
}ADS131_PARA;


//register addr 
#define  CONFIG1     0x41
#define  CONFIG2     0X42
#define  CONFIG3     0X43
#define  CH1SET      0X45
#define  CH2SET      0X46
#define  CH3SET      0X47
#define  CH4SET      0X48
#define  CH5SET      0X49
#define  CH6SET      0X4a
#define  CH7SET      0X4b
#define  CH8SET      0X4c






#define SPI_BRR         0X43      //


#define ADS131_START_H()         (GpioDataRegs.GPBSET.bit.GPIO62 = 1)
#define ADS131_START_L()         (GpioDataRegs.GPBCLEAR.bit.GPIO62 = 1)
#define ADS131_RESET_H()         (GpioDataRegs.GPBSET.bit.GPIO63 = 1)
#define ADS131_RESET_L()         (GpioDataRegs.GPBCLEAR.bit.GPIO63=1)
#define ADS131_CS_H()            (GpioDataRegs.GPBSET.bit.GPIO59 = 1)
#define ADS131_CS_L()            (GpioDataRegs.GPBCLEAR.bit.GPIO59=1)

#define ADS131_DATA_READY()      (GpioDataRegs.GPADAT.bit.GPIO20)

#define  ADS131E08_SDATAC      0x11
#define  ADS131E08_RESET       0X06
#define  ADS131E08_START       0x08
#define  ADS131E08_RDATAC      0x10
#define  ADS131E08_OFFSETCAL   0x1a














void spiGpioInit(void);
void spiInit(void);
Uint16  spiXmitByte(Uint16 byte);


void  ads131e08GpioInit(void);

void ads131e08Init(void);


void ads131e08WriteReg(Uint16 cmd, Uint16 val);

void ads131e08ReadReg(Uint16 addr,Uint16 num,Uint16 *valBuf);

void  getAdcInvertedData(void);



#endif
