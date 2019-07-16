/*
 * kbi.h
 *
 *  Created on: Jul 15, 2019
 *      Author: david
 */

#ifndef KBI_H_
#define KBI_H_

extern volatile unsigned char stopKBI;

void KBI_init(void);
interrupt VectorNumber_Vkeyboard void kbi_isr();
#endif /* KBI_H_ */
