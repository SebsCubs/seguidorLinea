/*
 * IIC.c
 *
 *  Created on: Jul 13, 2019
 *      Author: asces
 */

#include <hidef.h> 
#include "derivative.h"

#define Address ADDR.EE_Address
#define Addr_L ADDR.Bytes.Address_L
#define Addr_H ADDR.Bytes.Address_H
#define IIC_SLAVE 0x68
#define PWR_MGMT_1 0x6b 
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

typedef union {
  word EE_Address;
  struct {
    byte Address_H;
    byte Address_L;
  }
  Bytes;
}

tAddr;
tAddr ADDR;

void I2C_init(void);
byte IIC_read_byte(word addr);
void IIC_write_byte(word addr, byte data);
byte IIC_read_byte_ByteAddress(byte addr);
void IIC_write_byte_ByteAddress(byte addr, byte data);
void IIC_loop(void);
void IIC_main(void);
void IIC_write_byte(word addr, byte data);

volatile unsigned char tempI2C;
volatile signed char RD_data, charToInt[2];
volatile signed int dataGyro[3];

void I2C_init(void)
{
	IICF = 0x99; // Debe estar en maximo 400kHz
	IICC1_IICEN = 1; // Enable I2C;
	IICC1_TXAK = 1; // not generate ACK by master after transfer;
	IICC1_MST = 1; // slave mode para iniciar;
	
	
	IICS_SRW = 0; // R/W bit = 0;
	
}

void IIC_main(void){
	//Here the module will be set up to power up by changing 
	//Power management register PWR_MGMT_1
	I2C_init();
	IIC_write_byte_ByteAddress(PWR_MGMT_1,0); //Colocar el power management en 0 para despertar el módulo
}

void IIC_loop(void){
	//Read sensor value from gyros
	charToInt[0] = IIC_read_byte_ByteAddress(GYRO_XOUT_H);
	charToInt[1] = IIC_read_byte_ByteAddress(GYRO_XOUT_L);
	dataGyro[0] =  charToInt[0] | charToInt[1] << 8; //put them into an integer
}

byte IIC_read_byte_ByteAddress(byte addr)
{ 
	tempI2C  = IIC_SLAVE << 1| 0b00000000 ; //0 en el ultimo bit para write address
	IICC1_TXAK = 0; // RX/TX = 1; MS/SL = 1; TXAK = 0;
	IICC1 |= 0x30; // And generate START condition;
	IICD = tempI2C ; //0 en el ultimo bit para write address
	while (!IICS_IICIF); // wait until IBIF;
		IICS_IICIF=1; // clear the interrupt event flag;
	while(IICS_RXAK); // check for RXAK;
	
 //-----Slave ACK occurred------------
	//IICC1_MST = 1; //PELIGRO
	IICD = addr; // Send low byte of word address;
	while (!IICS_IICIF); // wait until IBIF;
	IICS_IICIF=1; // clear the interrupt event flag;
	while(IICS_RXAK); // check for RXAK;
	
 //-----Slave ACK occurred------------
	IICC1_RSTA = 1; // set up repeated start;
	//IICC1_MST = 1; //PELIGRO
	IICD = tempI2C | 1; // (slave_address) | (RW = 1);
	while (!IICS_IICIF); // wait until IBIF;
 	 	 IICS_IICIF=1; // clear the interrupt event flag;
 	while (IICS_RXAK); // check for RXAK;
 	
  //-----Slave ACK occurred------------
 	IICC1_TX = 0; // set up to receive;
 	IICC1_TXAK = 1; // acknowledge disable;
 	RD_data = IICD; // dummy read; Revisar si se está recibiendo el registro siguiente
 	//IICC1_MST = 1; //PELIGRO
 	while (!IICS_IICIF); // wait until IBIF;
 		IICS_IICIF=1; // clear the interrupt event flag;
 		IICC1_MST = 0; // generate stop signal;
 		RD_data = IICD; // read right data;
  return RD_data;
 }

void IIC_write_byte_ByteAddress(byte addr, byte data)
{
 //Address = addr; // load address; La dirección del módulo es 68 hex, de 7 bits
	//tempI2C = IICD; //dummyread
 tempI2C = IIC_SLAVE  ;//write bit 0 primeros 7 bits dirección del slave
 IICC1_TXAK = 0; // RX/TX = 1; MS/SL = 1; TXAK = 0;
 IICC1_TX = 1;
 IICC1_MST = 1;// And generate START condition;
 //-------start of transmit first byte to IIC bus-----
 IICD = tempI2C << 1; // 0 en el ultimo bit para write
 while (!IICS_IICIF); // wait until IBIF;
 IICS_IICIF=1; // clear the interrupt event flag;
 while(IICS_RXAK); // check for RXAK;
 //-----Slave ACK occurred------------
 tempI2C = addr;
 //IICC1_MST = 1; //PELIGRO
 IICD = tempI2C; // Send low byte of the word address;
 while (!IICS_IICIF); // wait until IBIF;
 IICS_IICIF=1; // clear the interrupt event flag;
 while(IICS_RXAK); // check for RXAK;
 //-----Slave ACK occurred------------
 //IICC1_MST = 1; //PELIGRO
 IICD = data;
 while (!IICS_IICIF); // wait until IBIF;
 IICS_IICIF=1; // clear the interrupt event flag;
 while(IICS_RXAK); // check for RXAK;
 //-----Slave ACK occurred------------
 IICS_IICIF=1; // clear the interrupt event flag;
 IICC1_MST = 0; // generate STOP condition;
}

void IIC_write_byte(word addr, byte data)
{
 Address = addr; // load address;
 tempI2C = (Addr_H & 0x07) << 1;
 IICC1_TXAK = 0; // RX/TX = 1; MS/SL = 1; TXAK = 0;
 IICC1_TX = 1;
 IICC1 |= 0x30; // And generate START condition;

 
 //-------start of transmit first byte to IIC bus-----
 IICD = IIC_SLAVE | tempI2C; // Address the slave and set up for master transmit;
 while (!IICS_IICIF); // wait until IBIF;
 IICS_IICIF=1; // clear the interrupt event flag;
 while(IICS_RXAK); // check for RXAK;
 //-----Slave ACK occurred------------
 IICC1_MST = 1;
 IICD = Addr_L; // Send low byte of the word address;
 while (!IICS_IICIF); // wait until IBIF;
 IICS_IICIF=1; // clear the interrupt event flag;
 while(IICS_RXAK); // check for RXAK;
 //-----Slave ACK occurred------------
 IICD = data;
 IICC1_MST = 1;
 while (!IICS_IICIF); // wait until IBIF;
 IICS_IICIF=1; // clear the interrupt event flag;
 while(IICS_RXAK); // check for RXAK;
 //-----Slave ACK occurred------------
 IICS_IICIF=1; // clear the interrupt event flag;
 IICC1_MST = 0; // generate STOP condition;
}
