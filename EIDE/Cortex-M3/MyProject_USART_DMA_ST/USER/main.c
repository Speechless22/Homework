#include "stm32f10x.h"
#include "led.h"
#include "USARTx.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];

//����ȷ��ʱ����
static void Delay(uint32_t time)
{
  uint32_t i,j;
  for(i=0;i<time;++i)
  {
    for(j=0;j<10000;++j)
    {       
      //��ѭ����ʲô������        
    }
  }
}

int main(void)
{

    uint16_t i;
    
	LED_Init(); //LED��ʼ��
    USARTx_Init_Config();//USART1��ʼ��
    USARTx_DMA_Config(); //DMA��ʼ��

    printf("ʹ��DMA��ʽ���䴮������\n");
  
    /*����Ҫ���͵����ݣ�����ѡ��A��ΪDMA���͵�����Դ*/
    for(i=0;i<SENDBUFF_SIZE;i++)
    {
        SendBuff[i]	 = 'A';
    }
    /* USART1 �� DMA����TX���� */
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); 
   
	while(1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		Delay(1000);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		Delay(1000);	
	}

}





