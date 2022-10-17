#ifndef __USARTx_H
#define __USARTx_H

#include "stm32f10x.h"
#include <stdio.h>

#define  SENDBUFF_SIZE    100

void USARTx_Init_Config(void);
void USARTx_DMA_Config(void);

int fputc(int ch, FILE *f);
int fgetc(FILE *f);

#endif


