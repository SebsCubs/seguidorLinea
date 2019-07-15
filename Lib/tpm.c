///******************************************************************************
///*  UNIVERSIDAD NACIONAL DE COLOMBIA - FACULTAD DE INGENIERÍA - SEDE BOGOTÁ   *
///*     Departamento de Ing. Mecánica y Mecatrónica  - Microcontroladores      *
///******************************************************************************
///*Fecha: 06/2019															  *
///*                														      *
///*Autor: Sebastián Cubides, David Fonseca y Paola Medina					  *
///*                															  *
///*Descripción: Source TPM, control de motores con PWM y lectura de encoders,  *
///*             comprobado por osciloscopio                              	  *
///*                															  *
///*Documentación:	-Hoja de datos	para MCU QE16 NXP	                      *
///*****************************************************************************/

#include <hidef.h> 
#include "derivative.h"
#include "sci.h"

/*Declaración de variables */ 
volatile unsigned int encoder1 = 0;
volatile unsigned int encoder2 = 0,error[2],kp;
volatile unsigned int frecuenciam1[6],frecuenciam2[6],frecuencia[2],var = 0;
volatile unsigned int velocidad[2] = 0;	
volatile unsigned char i2,im1,im2,counterVar,varTPM1,varTPM2,sentido1,sentido2 = 0;
volatile unsigned char contadorPulsos,contadorPulsos2 = 0;
volatile unsigned int contadorTiempoRTC = 0;
volatile unsigned int t1 = 2*250;
volatile unsigned int t2 = 2*250;
volatile unsigned int t3 = 2*250;
volatile unsigned int t4 = 2*250; 
 


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

void mainTPM(){
 
  /*Puertos GPIO del TPM 3 en 0 (Motores apagados y PWM disabled)*/
   PTCDD = 0b00001111;
   PTCD = 0b00000000;  
  
   kp=3;
  TPM1_init();
  TPM2_init();
  TPM3_init(); 
}

void loopMainTPM(void){

	
	for(im1 =5; im1>0; im1--){
	    frecuenciam1[im1] = frecuenciam1[im1-1];
	}
	frecuenciam1[0] = 31250/(encoder1*2);
	if(frecuenciam1[0]==frecuenciam1[1]){
		if(frecuenciam1[1]==frecuenciam1[2]){
			if(frecuenciam1[2]==frecuenciam1[3]){
				if(frecuenciam1[3]==frecuenciam1[4]){
					if(frecuenciam1[4]==frecuenciam1[5]){
						frecuencia[0]=0;
					}
				}
			}
		}
	}else{
		for(im1 =0; im1<6; im1++){
			frecuencia[0]+=frecuenciam1[im1];
		}
		frecuencia[0]=frecuencia[0]/6;
	}
	
	for(im2 =5; im2>0; im2--){
		frecuenciam2[im2] = frecuenciam2[im2-1];
	}
	frecuenciam2[0] = 31250/(encoder2*2);
	if(frecuenciam2[0]==frecuenciam2[1]){
		if(frecuenciam2[1]==frecuenciam2[2]){
			if(frecuenciam2[2]==frecuenciam2[3]){
				if(frecuenciam2[3]==frecuenciam2[4]){
					if(frecuenciam2[4]==frecuenciam2[5]){
						frecuencia[1]=0;
					}
				}
			}
		}
	}else{
		for(im2 =0; im2<6; im2++){
			frecuencia[1]+=frecuenciam2[im2];
		}
		frecuencia[1]=frecuencia[1]/6;
	}
	for(i2 = 0;i2<2;i2++){
		var = frecuencia[i2];
//		SCI_sendAscii(var);
//		velocidad[i2] = velocidad[i2] + var; // Filtrar (Enviando solo un dato)
//		counterVar++;
//	   if(counterVar == 5){
//		   var = velocidad[i2]/5;
////		   SCI_sendNumber(var);	
//		   i2 = 0;
//		   counterVar=0;
//	   }
		if(i2==0)
		{
//			SCI_send(32);
		}
	   
	   
	   }
//	SCI_send(13);
//	SCI_send(10);
	/* Enviar datos de sentido y velocidad (?)
	   for(i2 = 0;i2<2;i2++){
	    i = 0;
	    if(i2<1){
	    SCI_send(32);	//enviar " "  
	    }
	    
	    
	    }
	   
	    if(sentido1 == 1){
	    SCI_send(32);	//enviar " " 
	    SCI_send(49);	 
	    }else if(sentido1 == 0){
	    SCI_send(32);	//enviar " " 
	    SCI_send(48);
	    }
	    if(sentido2 == 1){
	    SCI_send(32);	//enviar " " 
	    SCI_send(49);	 
	    }else if(sentido2 == 0){
	    SCI_send(32);	//enviar " " 
	    SCI_send(48);
	    
	    }
	    	
	    SCI_send(32);	//enviar " " 
	    SCI_send(13);	//enviar retorno de carrito
	    SCI_send(10);	//enviar line finish*/
}

   
    


interrupt VectorNumber_Vtpm1ch0 void TPM1C0_isr(){
	TPM1C0SC_CH0F = 0;

	encoder1 = TPM1C0V;
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


