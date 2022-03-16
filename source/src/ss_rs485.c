#include "ss_include.h"


interrupt void scicRxData_handle(void);
Uint16 ReceivedChar1[10]={0};


/*********************************************************************************
 *  name :void sciRs485Init(void)
 *  funtion : init sci communication
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/
void scicRs485Init(Uint32      baudRate)
{
     // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function

	EALLOW;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO56 = 1;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO57 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO56  = 2;
    GpioCtrlRegs.GPBMUX2.bit.GPIO57  = 2;

    EDIS;
    GPIO_SetupPinMux(68, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(68, GPIO_OUTPUT, GPIO_PUSHPULL);
// Step 3. Clear all __interrupts and initialize PIE vector table:
// Disable CPU __interrupts
    DINT;
	


    ScicRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
    ScicRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
    ScicRegs.SCICTL2.all =0x0003;
    ScicRegs.SCICTL2.bit.TXINTENA =0;
    ScicRegs.SCICTL2.bit.RXBKINTENA =1;

    //
    // SCIA at 9600 baud
    // @LSPCLK = 50 MHz (200 MHz SYSCLK) HBAUD = 0x02 and LBAUD = 0x8B.
    // @LSPCLK = 30 MHz (120 MHz SYSCLK) HBAUD = 0x01 and LBAUD = 0x86.

	 // Uint16 BRR = 50e6/(baudRate*8) - 1;//配置波特率
    //
  //  ScicRegs.SCIHBAUD.all    =BRR/256;
   // ScicRegs.SCILBAUD.all    =BRR%256;
     ScicRegs.SCIHBAUD.all    =0x0002;
    ScicRegs.SCILBAUD.all    =0x008B;

    ScicRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
    /*disable fifo*/
	ScicRegs.SCIFFTX.all=0xC022;
	ScicRegs.SCIFFRX.all=0x0022;
    ScicRegs.SCIFFCT.all=0x0;

	ScicRegs.SCICCR.bit.ADDRIDLE_MODE	 = 0;//空闲线模式
	ScicRegs.SCICCR.bit.LOOPBKENA		 = 0;//非测试模式

	/*DATALEN */
	 ScicRegs.SCICCR.bit.SCICHAR          = 7; //设置数据位长度 = k+1
    /*noparity*/
	ScicRegs.SCICCR.bit.PARITYENA        =  0;
    ScicRegs.SCICCR.bit.STOPBITS         = 0;//停止位固定为1bit

	 ScicRegs.SCIFFTX.bit.TXFIFORESET=1;
    ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
	

   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.SCIC_RX_INT = &scicRxData_handle;
   EDIS;
   
  RS485_RX_EN() ;

 
}



/*********************************************************************************
 *  name :interrupt void scicRxData_handle(void)
 *  funtion : scic  rxdata handle 
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

interrupt void scicRxData_handle(void)
{
	static Uint16 rxIndex =0;

   //  if(ScicRegs.SCIRXST.bit.RXRDY   ==  1) //接受数据成功
//  {
   // mb_port1.FrameReq.Buf[mb_port1.FrameReq.Length++] = mb_port1_get();
          ReceivedChar1[rxIndex++]=    ScicRegs.SCIRXBUF.all;
         if(rxIndex ==9)
		 	rxIndex =0;
   
 // }
	ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    ScicRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all  |= M_INT8;     
}


/*********************************************************************************
 *  name :void rs485SendData(Uint16 *txBuf)
 *  funtion : scic  txdata
 *  inter para :
 *  out para :
 *  note:
 ********************************************************************************/

Uint16 rs485SendData(Uint16 *txBuf,Uint16 length)
{

  RS485_TX_EN();
  DELAY_US(50);
  while(length--)
  {
    while (SciaRegs.SCICTL2.bit.TXRDY == 0){} //可接受新数据
    SciaRegs.SCITXBUF.all  = *txBuf++;    //发送一个字节
  }

  RS485_RX_EN() ;
  return TRUE;
}




