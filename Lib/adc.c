///**********************************************************************************
///*  UNIVERSIDAD NACIONAL DE COLOMBIA - FACULTAD DE INGENIERÍA - SEDE BOGOTÁ   	*
///*     Departamento de Ing. Mecánica y Mecatrónica  - Microcontroladores      	*
///**********************************************************************************
///*Nombre del archivo: adc.c                                                   	*
///*				                                                          		*
///*Fecha: 06/2019	                                                          		*
///*                	                                                        	*
///*Autor: Sebastián Cubides, David Fonseca   y Paola Medina                    	*
///*                	                                                        	*
///*Descripción: Source ADC, lectura de sensores con ADC y envio de datos por SCI	*
///*                	                                                          	*
///*Documentación:	-Hoja de datos	para MCU QE16 NXP	                      		*
///*********************************************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "sci.h"

//variables para convertir con ADC

unsigned int x_h;
volatile unsigned char canal = 1, calibracionIniciada,medidoCalibrado;
volatile unsigned int dato[8]=0, valorSensor[8]=0,maxCalibracion[8]=0,minCalibracion[8]=0,valorCal;
unsigned int maxSensorvalorCals[8]=0, minSensorvalorCals[8]=0;

unsigned int  ich=0,ireset,jset,iset,irecord,imed=0;

//variables para seguirLinea

volatile unsigned long promedio = 0,suma = 0;
volatile unsigned int refLinea = 0, denominador=0;
volatile unsigned int iLinea = 0;
volatile char enLinea;
volatile unsigned int leido=0;

//prototipos
void loopMainADC(void);
void calcularRefLinea(void);
void medirCalibrado(void);
void calibrar(void);
void retardo(unsigned short);


interrupt VectorNumber_Vadc void adc_isr() {		//interrupción que se activa con cada conversion completada
	switch (canal) {								//alternar entre canales segun el canal anterior leido
	case 1:
		dato[0] = ADCR;								//como se esta en configuracion de solo 8 bits
													//como se esta en configuracion de solo 8 bits
		canal = 2;									//basta con leer solo la parte baja del resultado ADCRL
		ADCSC1_ADCH = 0b00011;						//cambiar de canal
		break;										//importante break en switch para no evaluar mas casos.
	case 2:
		dato[1] = ADCR;
		canal = 3;
		ADCSC1_ADCH = 0b01000;
		break;
	case 3:
		dato[2] = ADCR;
		canal = 4;
		ADCSC1_ADCH = 0b01001;
		break;
	case 4:
		dato[3] = ADCR;
		canal = 5;
		ADCSC1_ADCH = 0b00100;
		break;
	case 5:
		dato[4] = ADCR;
		canal = 6;
		ADCSC1_ADCH = 0b00101;
		break;
	case 6:
		dato[5] = ADCR;
		canal = 7;
		ADCSC1_ADCH = 0b00110;
		break;
	case 7:
		dato[6] = ADCR;
		canal = 8;
		ADCSC1_ADCH = 0b00111;
		break;
	case 8:
		dato[7] = ADCR;
		canal = 1;
		ADCSC1_ADCH = 0b00010;
		break;
	default:					//caso default en caso de que se tengan otros valores de canal inesperados
		break;
	}
}

void mainADC(void) {
	refLinea= 0;
	canal=1;
	
	ADCSC1_AIEN = 1;			//Conversion complete interrupt enabled
	ADCSC1_ADCO = 0;			//One conversion following a write to the ADCSC1 when software triggered operation is selected, or one
						//conversion following assertion of ADHWT when hardware triggered operation is selected.
	//MUX PARA ADCSC1_ADCH DEBE SER VARIABLE EN EL TIEMPO: CÓMO BUSCAR LA MEJOR FORMA QUE NO SEA CONSECUTIVA
//	ADCSC2_ADTRG = 0;			//Software trigger selected
//	ADCSC2_ACFE = 0;			//Compare function disabled
	ADCCFG_ADLPC = 0;			//High speed configuration
	ADCCFG_ADIV = 0b00;			//Input clocK
	ADCCFG_ADLSMP = 0;			//Short sample time
	ADCCFG_MODE = 0b01;			//12-bit conversion (N=12)
	ADCCFG_ADICLK = 0b00;		//Bus clock
//	APCTL1=0x00;				// AD0 al AD7 activados}
//
//	APCTL2=0xFF;				// AD8 al AD15 desactivados
	///// *****************************

	ADCSC1_ADCO = 0;
	ADCSC1_AIEN = 1;				//habilitar interrupcion ADC
	ADCSC1_ADCH = 0b00010;
	
	/* include your code here */
}

void loopMainADC(void){
	enLinea=0;
		PTED_PTED0 = 0;
		promedio = 0; // este es el ponderado total
		suma = 0; // es para el denominador el cual debe ser menor a 64000
		
//		if(canal==8){
	//CALIBRAR
			
	//SEND SENSORES
//			for(iLinea = 0; iLinea < 8; iLinea++){
//				SCI_send(83);		//enviar "S"
//				SCI_send(iLinea+49);	//enviar numero del sensor
//				SCI_send(58);		//enviar ":"
//				leido = dato[iLinea];
//				SCI_sendAscii(leido);
//				
//					SCI_send(32);		//enviar " "
//		
//			}
				

			calcularRefLinea();
//			SCI_send(39);
//			SCI_sendAscii(refLinea);
//			SCI_send(13);		//enviar retorno de carrito
//			SCI_send(10);		//enviar line finish
//		}


		
	//antes
	
	
}

void calcularRefLinea(){

	iLinea = 0;
	promedio = 0;	
//	if(medidoCalibrado){
	for (iLinea = 0; iLinea < 8; iLinea++){
		leido = dato[iLinea];
		//	  leido = 4095 - leido;
		//	if (invertReadings) { leido = 1000 - leido; }
//		x
		// keep track of whether we see the line at all
		if (leido > 2000) { 
			enLinea = 1; 
		} //antes estaba como: leido > 200
		
		// only average in leidos that are above a noise threshold
		if (leido > 600)						//antes estaba como: leido > 50
		{
			leido=leido/3.3;
			promedio = promedio + leido * (iLinea * 5);
			suma = suma + leido;
		}
	}

	if (!enLinea){
		PTED_PTED0 = 0;
		// If it last read to the left of center, return 0.
		if (refLinea < (7 * 5 / 2)){
			refLinea= 0;
		}
		// If it last read to the right of center, return the max.
		else{
			refLinea = 40;
		}
	}else{
		PTED_PTED0 = 1;
	}
	
	if(refLinea == 40 && promedio == 0 && suma == 0){
		//caso especial donde llegó al max de la izq y se salio de la linea
	}else{
		refLinea = promedio / suma;
	}
//	}
	
}

void medirCalibrado(){

	medidoCalibrado=0;
  for (imed = 0; imed < 8; imed++)
  {
    denominador = maxCalibracion[imed] - minCalibracion[imed];
    valorCal = 0;

    	  SCI_sendAscii(denominador);
    	  SCI_send(32);
    if (denominador != 0)
    {
      valorCal = (dato[imed] - minCalibracion[imed]) * 1000 / denominador;
    }

    if (valorCal < 0) { valorCal = 0; }
    else if (valorCal > 1000) { valorCal = 1000; }

    valorSensor[imed] = valorCal;
    
	  SCI_sendAscii(valorCal);
	  SCI_send(10);
	  SCI_send(13);
  } 
  medidoCalibrado=1;
    
}

void calibrar()
{

	calibracionIniciada=0;

  // (Re)allocate and initialize the arrays if necessary.
  if (!calibracionIniciada){
    // Initialize the max and min calibrated valorCals to valorCals that
    // will cause the first reading to update them.
    for (ireset = 0; ireset < 8; ireset++){
      maxCalibracion[ireset] = 0;
      minCalibracion[ireset] = 4095;
    }

    calibracionIniciada = 1;
  }

  for (jset = 0; jset < 10; jset++)
  {

    for (iset = 0; iset < 8; iset++)
    {
      // set the max we found THIS time
      if ((jset == 0) || (dato[iset] > maxSensorvalorCals[iset]))
      {
        maxSensorvalorCals[iset] = dato[iset];
      }

      // set the min we found THIS time
      if ((jset == 0) || (dato[iset] < minSensorvalorCals[iset]))
      {
        minSensorvalorCals[iset] = dato[iset];
      }
    
   	}
//    if(jset==9){
//
//        for (iset = 0; iset < 8; iset++)
//        {
//      	  SCI_sendAscii(iset);
//    	  SCI_send(32);
//	  SCI_sendAscii(minSensorvalorCals[iset]);
//	  SCI_send(32);
//	  SCI_sendAscii(minSensorvalorCals[iset]);
//	  SCI_send(32);
//	  SCI_send(13);
//	  SCI_send(10);
//        }
//    }
    retardo(600);
  }

  // record the min and max calibration valorCals
  for (irecord = 0; irecord < 8; irecord++)
  {
    // Update maximum only if the min of 10 readings was still higher than it
    // (we got 10 readings in a row higher than the existing maximum).
    if (minSensorvalorCals[irecord] > maxCalibracion[irecord])
    {
      maxCalibracion[irecord] = minSensorvalorCals[irecord];
    }

    // Update minimum only if the max of 10 readings was still lower than it
    // (we got 10 readings in a row lower than the existing minimum).
    if (maxSensorvalorCals[irecord] < minCalibracion[irecord])
    {
      minCalibracion[irecord] = maxSensorvalorCals[irecord];
    }

//	  SCI_send(32);
//	  SCI_send(13);
//	  SCI_send(10);
	  

  }
  
  

}



