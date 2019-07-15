///*****************************************************************************/*
///*  UNIVERSIDAD NACIONAL DE COLOMBIA - FACULTAD DE INGENIERÍA - SEDE BOGOTÁ   /*
///*     Departamento de Ing. Mecánica y Mecatrónica  - Microcontroladores      /*
///*****************************************************************************/*
///*Fecha: 07/2019															  /*
///*                														      /*
///*Autor: Sebastián Cubides, David Fonseca y Paola Medina					  /*
///*                															  /*
///*Descripción: Codigo de control para un seguidor de linea de altas			  /*
///*             prestaciones.					                              /*
///*                															  /*
///*Documentación:	-Hoja de datos	para MCU QE16 NXP	                      /*
///*****************************************************************************/



#include <hidef.h> 
#include "derivative.h" 
#include "adc.h"
#include "tpm.h"
#include "sci.h"
//#include "control.h"

volatile unsigned int maxSum;
volatile signed int VEL_Base_Derecha,VEL_Base_Izquierda,VEL_Max_Derecha,VEL_Max_Izquierda;
volatile signed int errorVelDer=0, errorVelIzq=0, derVelMotor,izqVelMotor,velMotor=0,errorPos=0,ulterrorPos=0;
volatile signed int disVelDer=0,disVelIzq=0,sumIzq=0,sumDer=0;
	
void main(void) {

  	EnableInterrupts;
  SOPT1 = 0x02;	//Desactivar el watch dog 
  
  mainADC();
  mainTPM();
  SCI_init();
  
  TPM3C0V = 0;
  TPM3C1V = 0;
  VEL_Base_Derecha=650;
  VEL_Base_Izquierda=650;
  VEL_Max_Derecha=800;
  VEL_Max_Izquierda=800;
  
  maxSum=2000;
  
  
	PTEDD_PTEDD0 = 1;
	PTED_PTED0 = 0;
  for(;;) {

	  errorPos = 16-refLinea;
	  velMotor = Kp * errorPos + Kd * (errorPos - ulterrorPos);
	  ulterrorPos = errorPos;
	  
	  disVelDer=(VEL_Base_Derecha - velMotor);
	  disVelIzq=(VEL_Base_Izquierda + velMotor);
	  
	  errorVelDer = disVelDer - frecuencia[0];
	  errorVelIzq = disVelIzq - frecuencia[1];

	  sumDer = max(sumDer + errorVelDer*10^(-6),(-1)*maxSum);
	  sumDer = min(sumDer,maxSum);
	  	  	  
	  sumIzq = max(sumIzq + errorVelDer*10^(-10),(-1)*maxSum);
	  sumIzq = min(sumIzq,maxSum);
	  
	  derVelMotor = KpMDer*errorVelDer+KiMDer*sumDer;
	  izqVelMotor = KpMIzq*errorVelIzq+KiMIzq*sumIzq;
	  
	  if (derVelMotor > VEL_Max_Derecha ) {
		  derVelMotor = VEL_Max_Derecha; // prevent the motor from going beyond max speed
	  }
	  if (izqVelMotor > VEL_Max_Izquierda ) {
		  izqVelMotor = VEL_Max_Izquierda; // prevent the motor from going beyond max speed
	  }
	  if (derVelMotor < -VEL_Max_Derecha ) {
		  derVelMotor = -VEL_Max_Derecha; // prevent the motor from going beyond max speed
	  }
	  if (izqVelMotor < -VEL_Max_Izquierda ) {
		  izqVelMotor = -VEL_Max_Izquierda; // prevent the motor from going beyond max speed
	  }
//	  if (derVelMotor < 0) {
//		  derVelMotor = 0; // keep the motor speed positive
//	  }
//	  if (izqVelMotor < 0) {
//		  izqVelMotor = 0; // keep the motor speed positive
//	  }
	  if(flag_StarStop){
//		  SCI_sendAscii(refLinea);
//		  SCI_send(32);
//		  SCI_sendAscii(frecuencia[0]);
//		  SCI_send(32);
//		  SCI_sendAscii(frecuencia[1]);
//		  SCI_send(32);
//		  SCI_sendAscii(disVelDer);
//		  SCI_send(32);
//		  SCI_sendAscii(disVelIzq);
//		  SCI_send(32);
//		  SCI_sendAscii(sumDer);
//		  SCI_send(32);
//		  SCI_sendAscii(sumIzq);
//		  SCI_send(32);
//		  SCI_sendAscii(errorVelDer);
//		  SCI_send(32);
//		  SCI_sendAscii(errorVelIzq);
//		  SCI_send(32);
//		  SCI_sendAscii(derVelMotor);
//		  SCI_send(32);
//		  SCI_sendAscii(izqVelMotor);
//		  SCI_send(13);
//		  SCI_send(10);

		  if(izqVelMotor<0){
			  TPM3C3V=0;
			  TPM3C1V=(-1)*izqVelMotor;
		  }else{
			  TPM3C3V=izqVelMotor;
			  TPM3C1V=0;
		  }if(derVelMotor<0){
			  TPM3C0V=0;
			  TPM3C2V=(-1)*derVelMotor;
		  }else{
			  TPM3C0V=derVelMotor;
			  TPM3C2V=0;
		  }

//		  TPM3C3V=1000;
//		  TPM3C1V=0;
//		  TPM3C0V=1000;
//		  TPM3C2V=0;
		  
	  }else{
	  TPM3C0V=0;
	  TPM3C3V=0;
	  }
	 loopMainADC();
	 loopMainTPM();
//	 ref=leerLinea(dato,canal);
//	 SCI_sendAscii(ref);
//	 SCI_send(13);
    }
}

