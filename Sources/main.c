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

//Importar  headers 
#include <hidef.h> 
#include "derivative.h" 
#include "adc.h"
#include "tpm.h"
#include "sci.h"

//Declaracion de variables
volatile unsigned int maxSum;
volatile signed int VEL_Base_Derecha, VEL_Base_Izquierda, VEL_Max_Derecha,
		VEL_Max_Izquierda;
volatile signed int errorVelDer = 0, errorVelIzq = 0, derVelMotor, izqVelMotor,
		velMotor = 0, errorPos = 0, ulterrorPos = 0;
volatile signed int disVelDer = 0, disVelIzq = 0, sumIzq = 0, sumDer = 0;

//Programa principal
void main(void) {

	EnableInterrupts
	;
	SOPT1 = 0x02;	//Desactivar el watch dog 

	//Llamado a los demás procedimientos
	mainADC();
	mainTPM();
	SCI_init();
	IIC_main();

	//Inicializar variables
	TPM3C0V = 0;
	TPM3C1V = 0;
	VEL_Base_Derecha = 650;
	VEL_Base_Izquierda = 650;
	VEL_Max_Derecha = 800;
	VEL_Max_Izquierda = 800;

	maxSum = 2000;

	PTEDD_PTEDD0 = 1;
	PTED_PTED0 = 0;
	for (;;) {

		errorPos = 16 - refLinea;// diferencia entre el valor deseado y el valormedido. El objetivo es que el error sea cero
		velMotor = Kp * errorPos + Kd * (errorPos - ulterrorPos);//La constante proporcional se multiplica por el término proporcional, es decir, el error.
		//La constante derivativa se multiplica por el término derivado. ( el término derivado, como el
		//nombre sugiere, mide el cambio en el término proporcional)
		ulterrorPos = errorPos;                         //guarda el ultimo error
		disVelDer = (VEL_Base_Derecha - velMotor);
		disVelIzq = (VEL_Base_Izquierda + velMotor);

		errorVelDer = disVelDer - frecuencia[0];
		errorVelIzq = disVelIzq - frecuencia[1];

		sumDer = max(sumDer + errorVelDer*10^(-6),(-1)*maxSum)
		;
		sumDer = min(sumDer,maxSum)
		;

		sumIzq = max(sumIzq + errorVelDer*10^(-10),(-1)*maxSum)
		;
		sumIzq = min(sumIzq,maxSum)
		;

		derVelMotor = KpMDer * errorVelDer + KiMDer * sumDer;
		izqVelMotor = KpMIzq * errorVelIzq + KiMIzq * sumIzq;

		if (derVelMotor > VEL_Max_Derecha) {
			derVelMotor = VEL_Max_Derecha; // impide que el motor supere la velocidad máxima
		}
		if (izqVelMotor > VEL_Max_Izquierda) {
			izqVelMotor = VEL_Max_Izquierda; // impide que el motor supere la velocidad máxima
		}
		if (derVelMotor < -VEL_Max_Derecha) {
			derVelMotor = -VEL_Max_Derecha; // impide que el motor supere la velocidad máxima
		}
		if (izqVelMotor < -VEL_Max_Izquierda) {
			izqVelMotor = -VEL_Max_Izquierda; // impide que el motor supere la velocidad máxima
		}
		//	  if (derVelMotor < 0) {
		//		  derVelMotor = 0; // mantener la velocidad del motor positiva
		//	  }
		//	  if (izqVelMotor < 0) {
		//		  izqVelMotor = 0; // mantener la velocidad del motor positivamantener la velocidad del motor positiva
		//	  }
		if (flag_StarStop) {
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

			if (izqVelMotor < 0) {
				TPM3C3V = 0;
				TPM3C1V = (-1) * izqVelMotor;
			} else {
				TPM3C3V = izqVelMotor;
				TPM3C1V = 0;
			}
			if (derVelMotor < 0) {
				TPM3C0V = 0;
				TPM3C2V = (-1) * derVelMotor;
			} else {
				TPM3C0V = derVelMotor;
				TPM3C2V = 0;
			}

			//		  TPM3C3V=1000;
			//		  TPM3C1V=0;
			//		  TPM3C0V=1000;
			//		  TPM3C2V=0;

		} else {
			TPM3C0V = 0;
			TPM3C3V = 0;
		}
		loopMainADC();
		loopMainTPM();
		IIC_loop();
		//	 ref=leerLinea(dato,canal);
		//	 SCI_sendAscii(ref);
		//	 SCI_send(13);
	}
}

