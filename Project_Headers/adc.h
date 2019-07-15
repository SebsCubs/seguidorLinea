/*
 * adc.h
 *
 *  Created on: Jun 29, 2019
 *      Author: david
 */

#ifndef ADC_H_
#define ADC_H_

///******************************************************************************
///*  UNIVERSIDAD NACIONAL DE COLOMBIA - FACULTAD DE INGENIERÍA - SEDE BOGOTÁ   *
///*     Departamento de Ing. Mecánica y Mecatrónica  - Microcontroladores      *
///******************************************************************************
///*Fecha: 06/2019	                                                          *
///*                	                                                          *
///*Autor: Sebastián Cubides, David Fonseca   y Paola Medina                    *
///*                	                                                          *
///*Descripción: Source ADC, lectura de sensores con ADC y envîo de datos por SCI  	  *
///*                	                                                          *
///*Documentación:	-Hoja de datos	para MCU QE16 NXP	                      *
///*****************************************************************************/

#include <hidef.h> 							/* for EnableInterrupts macro */
#include "derivative.h" 					/* include peripheral declarations */
#include "sci.h"							/* Usada para enviar datos como ASCII o como bytes */

//VARIABLES usadas por el ADC que son necesarias en el Control Global
extern volatile unsigned int refLinea;		// Variable 
extern volatile unsigned int dato[8];
extern volatile unsigned char canal;

//INTERRUPCIONES

interrupt VectorNumber_Vadc void adc_isr();

//FUNCIONES
//
void calcularRefLinea(void);
void medirCalibrado(void);
void calibrar(void);
void mainADC(void);
void mainLoopADC(void);


#define min(a, b) (((a) < (b)) ? (a) : (b));
#define max(a, b) (((a) > (b)) ? (a) : (b)); 


#endif /* ADC_H_ */
