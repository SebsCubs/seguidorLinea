/*
 * IIC.h
 *
 *  Created on: Jul 13, 2019
 *      Author: asces
 */

#ifndef IIC_H_
#define IIC_H_

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


//typedef union {
//  word EE_Address;
//  struct {
//    byte Address_H;
//    byte Address_L;
//  }
//  Bytes;
//}
//
//tAddr;
//tAddr ADDR;

extern volatile signed char RD_data;
extern volatile signed int dataGyro[3];

void I2C_init(void);
byte IIC_read_byte_ByteAddress(byte addr);
void IIC_write_byte_ByteAddress(byte addr, byte data);
void IIC_write_byte(word addr, byte data);
void IIC_loop(void);
void IIC_main(void);

#endif /* IIC_H_ */
