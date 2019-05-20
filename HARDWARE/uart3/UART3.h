#ifndef __UART3_H_
#define __UART3_H_

void Initial_UART3(unsigned long baudrate);
void UART3_Put_Char(unsigned char DataToSend);
void UART3_Put_String(unsigned char *Str);
void CopeSerial3Data(unsigned char ucData);
#endif

//------------------End of File----------------------------

