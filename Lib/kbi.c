/*
 * kbi.c
 *
 *  Created on: Jul 15, 2019
 *      Author: david
 */



#include <hidef.h> 
#include "derivative.h"

/*Declaraci�n de variables */ 

volatile unsigned char stopKBI; 

/*Iniciaci�n de m�dulos*/
void KBI_init(void) {

	PTDDD  = 0b01111111;					//puerto D bit7 como  salida
	PTDPE  = 0b10000000;					//habilitar pull-up en pin0 & pin1 de PTA

	KBI2ES = 0b10000000;            		//interrupci�n por flanco de entrada en los pines KBIP0 & KBPI
	KBI2PE = 0b10000000;            		//habilitacion solo de pines 0,1 como fuentes de KBI

	//KBI Status and Control
	KBI2SC_KBACK=1;          				//1 en KBACK para evitar falsar interrupciones antes de iniciar el modulo
	KBI2SC_KBIMOD=0;           				//0 en KBMOD para generar interrupcion solo por flancos
	KBI2SC_KBIE=1;           				//1 en KBIE para activar las interrupciones por KBI
}

//interrupci�n que se activa con cada conversion completada
interrupt VectorNumber_Vkeyboard void kbi_isr() {		


	KBI2SC_KBACK=2;           			//limpiar bandera al poner en 1 el bit, acknowledge interrupcion
								//para permitir alguna interrupci�n futura
								//preguntar que pin gener� la interrupcion KBI y ejecutar acci�n
	if(PTDD == 0b10000000){
		stopKBI=1;
	}

}
