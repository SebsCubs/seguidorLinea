///******************************************************************************
///*  UNIVERSIDAD NACIONAL DE COLOMBIA - FACULTAD DE INGENIERÍA - SEDE BOGOTÁ   *
///*     Departamento de Ing. Mecánica y Mecatrónica  - Microcontroladores      *
///******************************************************************************
///*Fecha: 06/2019															  *
///*                														      *
///*Autor: Sebastián Cubides, David Fonseca y Paola Medina					  *
///*                															  *
///*Descripción: Header TPM, control de motores con PWM y lectura de encoders,  *
///*             comprobado por osciloscopio                              	  *
///*                															  *
///*Documentación:	-Hoja de datos	para MCU QE16 NXP	                      *
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
