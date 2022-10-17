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

    	//USART1 ��������

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
	USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE); 
    
     /* ���������ɱ�־ */
	USART_ClearFlag(USART1, USART_FLAG_TC|USART_FLAG_TXE|USART_FLAG_RXNE);       
}

void USARTx_DMA_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* ����DMAʱ�� */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /*����DMAԴ���������ݼĴ�����ַ*/  
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USART1->DR));	
  /*�ڴ��ַ(Ҫ����ı�����ָ��)*/
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SendBuff;
  /*���򣺴��ڴ浽����*/		
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
  /*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
  DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
  /*�����ַ����*/	    
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
  /*�ڴ��ַ����*/
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
  /*�������ݵ�λ*/	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /*�ڴ����ݵ�λ 8bit*/
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
  /*DMAģʽ������ѭ��*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 
  /*���ȼ�����*/	
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
  /*��ֹ�ڴ浽�ڴ�Ĵ���	*/
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  /*����DMA1��4ͨ��*/		   
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  /*ʹ��DMA*/
  DMA_Cmd (DMA1_Channel4,ENABLE);			
  /* ����DMA������ɺ�����ж� */
  DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  
}


/**
  * ��������: �ض���c�⺯��printf��USART1
  * �������: ��
  * �� �� ֵ: ��
  */
int fputc(int ch, FILE *f)
{
  /* ����һ���ֽ����ݵ����Դ��� */
  USART_SendData(USART1, (uint8_t) ch);

  /* �ȴ��������ݷ������ */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

  return (ch);
}

/**
  * ��������: �ض���c�⺯��getchar,scanf��USART1
  * �������: ��
  */
int fgetc(FILE *f)
{
  /* �ȴ������������� */
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

  return (int)USART_ReceiveData(USART1);
}



