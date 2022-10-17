#include "USARTx.h"

uint8_t SendBuff[SENDBUFF_SIZE];


void USARTx_Init_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	USART_DeInit(USART1);
    
    	//USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
   
        //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    	//USART1 参数配置

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
	USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE); 
    
     /* 清除发送完成标志 */
	USART_ClearFlag(USART1, USART_FLAG_TC|USART_FLAG_TXE|USART_FLAG_RXNE);       
}

void USARTx_DMA_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* 开启DMA时钟 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /*设置DMA源：串口数据寄存器地址*/  
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USART1->DR));	
  /*内存地址(要传输的变量的指针)*/
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SendBuff;
  /*方向：从内存到外设*/		
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
  /*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
  DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
  /*外设地址不增*/	    
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
  /*内存地址自增*/
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
  /*外设数据单位*/	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /*内存数据单位 8bit*/
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
  /*DMA模式：不断循环*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 
  /*优先级：中*/	
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
  /*禁止内存到内存的传输	*/
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  /*配置DMA1的4通道*/		   
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  /*使能DMA*/
  DMA_Cmd (DMA1_Channel4,ENABLE);			
  /* 配置DMA发送完成后产生中断 */
  DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  
}


/**
  * 函数功能: 重定向c库函数printf到USART1
  * 输入参数: 无
  * 返 回 值: 无
  */
int fputc(int ch, FILE *f)
{
  /* 发送一个字节数据到调试串口 */
  USART_SendData(USART1, (uint8_t) ch);

  /* 等待串口数据发送完毕 */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

  return (ch);
}

/**
  * 函数功能: 重定向c库函数getchar,scanf到USART1
  * 输入参数: 无
  */
int fgetc(FILE *f)
{
  /* 等待串口输入数据 */
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

  return (int)USART_ReceiveData(USART1);
}



