/*
 * sci.h
 *
 *  Created on: Jun 29, 2019
 *      Author: david
 */

#ifndef SCI_H_
#define SCI_H_



#include <hidef.h> 
#include "derivative.h"


extern float Kp,Kd,KpMIzq,KpMDer, KiMIzq, KiMDer;
extern unsigned char flag_StarStop;

void SCI_init(void);
//funcion que atendera la interrupcion por SCI al recibir datos
interrupt VectorNumber_Vsci2rx void sci_rx();

void SCI_send(unsigned char);

void SCI_send_ADC_TPM(unsigned char);

void SCI_sendNumber(volatile unsigned int);

void SCI_sendAscii(unsigned int);

void getAscii(unsigned int);

#endif /* SCI_H_ */
