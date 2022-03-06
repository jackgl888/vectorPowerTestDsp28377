#ifndef __SS_RS485_H__
#define __SS_RS485_H__

#include "F28x_Project.h"

#include "ss_include.h"

#define   BAUDRATE     9600

#define RS485_RX_EN()       GpioDataRegs.GPCCLEAR.bit.GPIO68 = 1;
#define RS485_TX_EN()      GpioDataRegs.GPCSET.bit.GPIO68 = 1;

void scicRs485Init(Uint32        baudRate);
Uint16 rs485SendData(Uint16 *txBuf,Uint16 length);





#endif


