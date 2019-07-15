///******************************************************************************
///*  UNIVERSIDAD NACIONAL DE COLOMBIA - FACULTAD DE INGENIER�A - SEDE BOGOT�   *
///*     Departamento de Ing. Mec�nica y Mecatr�nica  - Microcontroladores      *
///******************************************************************************
///*Fecha: 06/2019															  *
///*                														      *
///*Autor: Sebasti�n Cubides, David Fonseca y Paola Medina					  *
///*                															  *
///*Descripci�n: Header TPM, control de motores con PWM y lectura de encoders,  *
///*             comprobado por osciloscopio                              	  *
///*                															  *
///*Documentaci�n:	-Hoja de datos	para MCU QE16 NXP	                      *
///*****************************************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#ifndef TPM_H_
#define TPM_H_

extern volatile unsigned int frecuencia[2];


void TPM1_init();
void TPM2_init();
void TPM3_init();
void loopMainTPM(void);
void mainTPM(void);


interrupt VectorNumber_Vtpm1ch0 void TPM1C0_isr();

interrupt VectorNumber_Vtpm1ch1 void TPM1C1_isr();
interrupt VectorNumber_Vtpm2ch0 void TPM2C0_isr();

interrupt VectorNumber_Vtpm2ch1 void TPM2C1_isr();


#endif /* TPM_H_ */
