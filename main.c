/******************************************************************************
/*  UNIVERSIDAD NACIONAL DE COLOMBIA - FACULTAD DE INGENIERÍA - SEDE BOGOTÁ   *
/*     Departamento de Ing. Mecánica y Mecatrónica  - Microcontroladores      *
/******************************************************************************
/*Fecha: 07/2019															  *
/*                														      *
/*Autor: Sebastián Cubides, David Fonseca y Paola Medina					  *
/*                															  *
/*Descripción: Codigo de control para un seguidor de linea de altas			  *
/*             prestaciones.					                              	  *
/*                															  *
/*Documentación:	-Hoja de datos	para MCU QE16 NXP	                      *
/*****************************************************************************/

#include <hidef.h> 
#include "derivative.h" 

/*Declaración de variables */ 
volatile unsigned int encoder1 = 0;
volatile unsigned int encoder2 = 0;
volatile unsigned int frecuencia[2] = 0;
volatile unsigned int velocidad[2] = 0;
volatile unsigned long factor = 1;	
volatile unsigned char arr[6], i,i2,varTPM1,varTPM2,sentido1,sentido2 = 0;
volatile unsigned int var,temp = 0;
volatile unsigned char contadorPulsos,contadorPulsos2 = 0;
volatile unsigned int contadorTiempoRTC = 0;
volatile unsigned int t1 = 2*250;
volatile unsigned int t2 = 2*250;
volatile unsigned int t3 = 2*250;
volatile unsigned int t4 = 2*250;  


/*Prototipo de funciones*/
void SCI_send(unsigned char);
void getAscii(unsigned int);
void trapezoide(void);

/*Iniciación de módulos*/
void SCI_init() {
	
SCI2BDH = 0;
SCI2BDL = 27;
SCI2C1 = 0x00;
SCI2C2_TE = 1;
SCI2C2_RE = 1;
SCI2C2_RIE = 1;
}

void TPM1_init(){
/*********Encoder M1*******/
TPM1C0SC = 0b01001100; //Channel 0 activado por flanco de subida, interrupciones activadas
TPM1C1SC = 0b01001100; //Channel 1 activado por flanco de subida, interrupciones activadas
TPM1SC = 0b00001111;	//Activar contador, prescale = 128
}
void TPM2_init(){
/*******Encoder M2 *********/
TPM2C0SC = 0b01001100; //Channel 0 activado por flanco de subida, interrupciones activadas
TPM2C1SC = 0b01001100; //Channel 1 activado por flanco de subida, interrupciones activadas
TPM2SC = 0b00001111;	//Activar contador, prescale = 128
}
void TPM3_init(){
/***********PWM para motores******/

TPM3SC_CLKSx = 0b00;
TPM3MOD = 2000; //overflow en 2000 para una freq de 2kHz

TPM3C0SC_ELS0x = 0b10;	//Canal 0 config Motor 1.1 ELS3x Inicialmente en 00
TPM3C1SC_ELS1x = 0b10;	//Canal 1 config Motor 2.1 ELS3x Inicialmente en 00
TPM3C2SC_ELS2x = 0b10;	//Canal 0 config Motor 1.1 ELS3x Inicialmente en 00
TPM3C3SC_ELS3x = 0b10;	//Canal 1 config Motor 2.1 ELS3x Inicialmente en 00


TPM3SC = 0b00101000;   //Status y control register prescaler = 1 CPWMS = 1



}



void main(void) {
  EnableInterrupts;
  SOPT1 = 0x02;	//Desactivar el watch dog  
  /*Puertos GPIO del TPM 3 en 0 (Motores apagados y PWM disabled)*/
   PTCDD = 0b00001111;
   PTCD = 0b00000000;  
  /*Iniciación de módulos*/
  SCI_init();
  TPM1_init();
  TPM2_init();
  TPM3_init(); 
  /*
  TPM3C0V = 1000;
  TPM3C3V = 1000;
  */
  for(;;) { 
	  /*Enviar los datos de velocidad y de los sensores						*/
	  
	  /*Cuando se reciba el comando, activar o desactivar el PWM			*/
	  /*El control cambia la velocidad de las ruedas traseras				*/
	  /*Parte del requerimiento de velocidad lineal y velocidad				*/
	  /*angular del robot													*/
	  /*Se usa el modelo donde se tiene la matriz D = [r/2 r/2;-r/2l -r/2l]	*/
	  /*y la velocidad angular de cada rueda dependerá de:					*/
	  /*[phi1 phi2] = D^-1[V;w] donde phi1 y phi2 son la velocidad			*/
	  
	  /* Leer datos de sensores, generar error
	   * Implementar PID actualizando el valor de los sensores
	   * Enviar datos con SCI.
	   */
	  
    }
}

interrupt VectorNumber_Vtpm1ch0 void TPM1C0_isr(){
	TPM1C0SC_CH0F = 0;

	encoder1 = TPM1C0V;
	frecuencia[0] = 31250/(encoder1*2);
	TPM1CNT = 0;

	if(varTPM1 == 1){
		sentido1 = 0;
		varTPM1 = 0;
	}else if(varTPM1 == 0){
		varTPM1 = 1;
	}
}

interrupt VectorNumber_Vtpm1ch1 void TPM1C1_isr(){

	TPM1C1SC_CH1F = 0;

	if(varTPM1 == 1){
		sentido1 = 1;
		varTPM1 = 0;
	}else if(varTPM1 == 0){
		varTPM1 = 1;
	}
}

interrupt VectorNumber_Vtpm2ch0 void TPM2C0_isr(){
	TPM2C0SC_CH0F = 0;

		encoder2 = TPM2C0V;
		frecuencia[1] = 31250/(encoder2*2);
		TPM2CNT = 0;
		
	if(varTPM2 == 1){
		sentido2 = 1;
		varTPM2 = 0;
	}else if(varTPM2 == 0){
		varTPM2 = 1;
	}

	
}

interrupt VectorNumber_Vtpm2ch1 void TPM2C1_isr(){
	TPM2C1SC_CH1F = 0;

	if(varTPM2 == 1){
		sentido2 = 0;
		varTPM2 = 0;
	}else if(varTPM2 == 0){
		varTPM2 = 1;
	}
}


void SCI_send(unsigned char mensaje) {
while (SCI2S1_TDRE == 0);	//enviar datos por polling en el puerto serial
SCI2D = mensaje;	//esperando que el registro se desocupe
}


