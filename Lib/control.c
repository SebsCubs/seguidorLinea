/*
 * control.c
 *
 *  Created on: Jul 5, 2019
 *      Author: david
 */


#include <hidef.h> 
#include "derivative.h"
#include "sci.h"
//
//volatile unsigned long promedio = 0;
//volatile unsigned long refLinea = 0;
//volatile unsigned int suma = 0,iLinea = 0;
//volatile char enLinea;
//volatile unsigned int leido=0;




//unsigned long leerLinea(volatile unsigned int *dato, volatile unsigned char canal ){
//	enLinea=0;
//	PTED_PTED0 = 0;
//	promedio = 0; // este es el ponderado total
//	suma = 1; // es para el denominador el cual debe ser menor a 64000
//	
//	if(canal==8){
////SEND SENSORES
//		for(iLinea = 0; iLinea < 8; iLinea++){
//				SCI_send(83);		//enviar "S"
//				SCI_send(iLinea+49);	//enviar numero del sensor
//				SCI_send(58);		//enviar ":"
//				leido = dato[iLinea];
//				SCI_sendAscii(leido);
//				
//				if (iLinea<7){
//					SCI_send(32);		//enviar " "
//				}		
//			}
//			
//		iLinea = 0;
//			SCI_send(13);		//enviar retorno de carrito
//			SCI_send(10);		//enviar line finish
//			
//		
//		for (iLinea = 0; iLinea < 8; iLinea++){
//			leido = dato[iLinea];
//			//	  leido = 4095 - leido;
//			//	if (invertReadings) { leido = 1000 - leido; }
//			
//			// keep track of whether we see the line at all
//			if (leido > 2000) { 
//				enLinea = 1; 
//			} //antes estaba como: leido > 200
//			
//			// only average in leidos that are above a noise threshold
//			if (leido > 400)						//antes estaba como: leido > 50
//			{
//				promedio += leido * (iLinea * 1000);
//				suma += leido;
//			}
//		}
//		
//		if (!enLinea){
//			PTED_PTED0 = 0;
//			// If it last read to the left of center, return 0.
//			if (refLinea < (7 * 1000 / 2)){
//				return 0;
//			}
//			// If it last read to the right of center, return the max.
//			else{
//				return 7 * 1000;
//			}
//		}else{
//			PTED_PTED0 = 1;
//		}
//
//		SCI_sendAscii(refLinea);
//	}
//	refLinea = promedio / suma;
//	return refLinea;
//
//}

