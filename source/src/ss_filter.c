#include "ss_include.h"






/*********************************************************************************
 *  name :void filterInit(void)
 *  funtion :   
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

void filterInit(void)
{
   Uint16 i;
   for(i = 0;i<ADCVAL_BUFFER_SIZE;i++)
   	 ssSystem.adcFilter.singleChannelVal[i]=0;

   for(i =0 ;i< ADCNUM ;i++  )
   	ssSystem.adcFilter.channelValAvg[i]=0;
   	

}



/*********************************************************************************
 *  name :void  Array_Bubble_Sorting(Uint16 *array_addr,Uint16 len)
 *  funtion :Bubble sort algorithm
 *  inter para :array_addr, len
 *  out para :
 *  note:
 ********************************************************************************/

void  arrayBubbleSorting(Uint16 *array_addr,Uint16 len)
{
	Uint16 i,j;
	Uint32 temp;
																																																																																																	
	for(i=0;i<(len-1);i++)
	{
		for(j=0;j<(len-(i+1));j++)
		{
			if(array_addr[j]>array_addr[j+1])
			{
				temp=array_addr[j];
				array_addr[j]=array_addr[j+1];
				array_addr[j+1]=temp;
			}
		}
	}
}


/*********************************************************************************
 *  name :void filterAdcChannels(void)
 *  funtion :
 *  inter para :  参数  数字
 *  out para :
 *  note:
 ********************************************************************************/

void filterAdcChannels(void)
{

    Uint16 count, i;
	Uint32 singleChannelValSum=0;
	Uint16 singleChannelValBuf[100]={0};

	/* filter for each adc_channel */

            /*冒泡与均值*/
			 for(i=0;i<ADCNUM;i++)
		    {
			        for ( count=0;count<  ARRAY_FILTER_SIZE;count++)
					{
					 
                      ssSystem.adcFilter.singleChannelVal[count] = ssSystem.dspAdc.adcValue[i][count];

			          // singleChannelValBuf[count]=  ssSystem.dspAdc.adcValue[i][count];
					}
					/*排序*/
					arrayBubbleSorting(  singleChannelValBuf, ARRAY_FILTER_SIZE );
                    /*均值*/
					for(count=ARRAY_STARTING_POS ; count< ARRAY_ENDING_POS ;count++)  
					{
						singleChannelValSum +=  ssSystem.adcFilter.singleChannelVal[count];
					}
					/*得到最终的滤波数据*/
					ssSystem.adcFilter.channelValAvg[i] = (Uint32)(singleChannelValSum/ ARRAY_AVG_LEN);   

					singleChannelValSum=0;
					                       
		    }  /////////
		 
}



