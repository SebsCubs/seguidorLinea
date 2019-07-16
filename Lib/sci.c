///******************************************************************************
///*  UNIVERSIDAD NACIONAL DE COLOMBIA - FACULTAD DE INGENIERÍA - SEDE BOGOTÁ   *
///*     Departamento de Ing. Mecánica y Mecatrónica  - Microcontroladores      *
///******************************************************************************
///*Fecha: 06/2019															  	*
///*                														    *
///*Autor: Sebastián Cubides, David Fonseca y Paola Medina					 	*
///*                															*
///*Descripción: Source TPM, control de motores con PWM y lectura de encoders,  *
///*             comprobado por osciloscopio                              	  	*
///*                															*
///*Documentación:	-Hoja de datos	para MCU QE16 NXP	                      	*
///*****************************************************************************/


#include <hidef.h> 
#include "derivative.h"

/*Declaración de variables */ 
float Kp,Kd,KpMIzq,KpMDer, KiMIzq, KiMDer;
volatile unsigned char iSCI;
volatile unsigned int KpMotor=1,byteEnviar, change;
unsigned char flag_rx;		//variable dummy para mecanismo que limpia interrupcion 
unsigned short varRetard;
unsigned char flag_StarStop;

//variables del send ascii
unsigned long factor = 1;	
unsigned char arreglo[8]=0, iAscii = 0,icalibrate=0;
unsigned int num=0, temp=0;

void getAscii(unsigned int);
void SCI_send(unsigned char);
void SCI_sendAscii(unsigned int);

/*Iniciación de módulos*/
void SCI_init() {
	  Kp=10.0;
	  Kd=30.0;
	  KpMIzq =4;	//Caracterización de 2.8003
	  KpMDer =4;	//Caracterización de 6.1581
	  KiMIzq=0.05;
	  KiMDer=0.05;
	SCI2BDH = 0;
	SCI2BDL = 27;
	SCI2C1 = 0x00;
	SCI2C2_TE = 1;
	SCI2C2_RE = 1;
	SCI2C2_RIE = 1;
}


//funcion que atendera la interrupcion por SCI al recibir datos
interrupt VectorNumber_Vsci2rx void sci_rx(){
	flag_rx=SCI2S1_RDRF;
	

	if(SCI2D == 0b00000010){

		flag_StarStop=1;
////		TPM3C0V = 600;
////		TPM3C3V = 600;
	}
	if(KpMotor == 1){
		//kp m1 byte High
		Kp=SCI2D-48;
		SCI_send(80);
		SCI_sendAscii(Kp);
		SCI_send(32);
		KpMotor = 2;
	}else if(KpMotor == 2){
		//kp m1 byte Low
		Kd=SCI2D-48;
		SCI_send(68);
		SCI_sendAscii(Kd);
		SCI_send(32);
		KpMotor = 3;
	}else if(KpMotor == 3){
		//kp m2 byte High
		KpMIzq=SCI2D-48;
		SCI_sendAscii(KpMIzq);
		SCI_send(32);
		KpMotor = 4;
	}else if(KpMotor == 4){
		//kp m2 byte High
		KpMDer=SCI2D-48;
		SCI_sendAscii(KpMDer);
		SCI_send(32);
		KpMotor = 5;					
	}else if(KpMotor == 5){
		//kp m2 byte High
		KiMIzq=SCI2D-48;
		SCI_sendAscii(KiMDer);
		SCI_send(32);
		KpMotor = 6;					
	}else if(KpMotor == 6){
		//kp m2 byte High
		KiMDer=SCI2D-48;
		SCI_sendAscii(KiMIzq);
		SCI_send(13);
		SCI_send(10);
		KpMotor = 1;					
	}
	
//	if(SCI2D == 0b00000010){
//
////		  for (icalibrate = 0; icalibrate < 100; icalibrate++)
////		  {
////			  calibrar();
////		  }
////		  
////		  medirCalibrado();
////		  SCI_send(67);
////		  SCI_send(65);
//		  
//		flag_StarStop=1;
////		TPM3C0V = 600;
////		TPM3C3V = 600;
//	}else if(SCI2D == 0b00000011){
//		flag_StarStop=0;
//		TPM3C0V = 0;
//		TPM3C1V = 0;
//		TPM3C2V = 0;
//		TPM3C3V = 0;
//	
//	}else{
//		if(KpMotor == 1){
//			//kp m1 byte High
//			Kp=SCI2D;
//			SCI_send(80);
//			KpMotor = 2;
//		}else if(KpMotor == 2){
//			//kp m1 byte Low
//			Kd=SCI2D;
//			SCI_send(68);
//			KpMotor = 1;
//		}else if(KpMotor == 3){
//			//kp m2 byte High
//			KpMotor = 4;
//		}else if(KpMotor == 4){
//			//kp m2 byte High
//			KpMotor = 1;					
//		}
//	}
}

void SCI_sendAscii(unsigned int numero){

	factor = 1;	
	arreglo[0]=0;
	arreglo[1]=0;
	arreglo[2]=0;
	arreglo[3]=0;
	arreglo[4]=0;
	arreglo[5]=0;
	arreglo[6]=0;
	arreglo[7]=0;
	iAscii = 0;
	num=0;
	temp=0;
	getAscii(numero);				//conversion a ASCII
	if(numero == 0){
		SCI_send(48);
	}
	while (numero != 0) {			//enviar digitos al puerto
		numero = numero / 10;			//cuando ya no quedan digitos sale del while
					//retardo por polling para dealy entre envio de datos
			SCI_send(arreglo[iAscii]);	
			iAscii++;			
		
	}
}

void getAscii(unsigned int num) {
	temp = num;
	while (temp) {					//contar el numero de digitos del dato
		temp = temp / 10;
		factor = factor*10;
	}
	while (factor > 1) {
		factor = factor/10;		
		arreglo[iAscii] = (char)(num / factor);	//descomponer el número en digitos
		arreglo[iAscii] = arreglo[iAscii] + 48;		//sumarle 48 a cada digito y guardarlo en un arreglo
		num = (num % factor);			//actualizar numero con los siguientes digitos que faltan
		iAscii++;
	}
	iAscii = 0;
	
}

void SCI_send(unsigned char mensaje) {
	while (SCI2S1_TDRE == 0);	//enviar datos por polling en el pu erto serial
	//if (SCI2S1_TDRE == 1){ SCI2D = mensaje;} 
	SCI2D = mensaje;	//esperando que el registro se desocupe
}

void SCI_send_ADC_TPM(unsigned char mensaje) {
	//enviar datos por polling en el puerto serial
	// envía 8 valores de sensores y dos velocidades con sus dos sentidos
}

void SCI_sendNumber(volatile unsigned int numero){
	byteEnviar=numero;
	while(numero){
		SCI_send(byteEnviar);
		byteEnviar=((1<<8)-1) & (numero >> (8));
		numero=numero>>8;
	}
}

void retardo(unsigned short max) {
	for (varRetard = 0; varRetard < max; ++varRetard) {
	}
}
