#include "stm32f10x.h"
#include "led.h"
#include "USARTx.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];

//不精确延时函数
static void Delay(uint32_t time)
{
  uint32_t i,j;
  for(i=0;i<time;++i)
  {
    for(j=0;j<10000;++j)
    {       
      //空循环，什么都不做        
    }
  }
}

int main(void)
{

    uint16_t i;
    
	LED_Init(); //LED初始化
    USARTx_Init_Config();//USART1初始化
    USARTx_DMA_Config(); //DMA初始化

    printf("使用DMA方式传输串口数据\n");
  
    /*输入要发送的数据，这里选用A作为DMA传送的数据源*/
    for(i=0;i<SENDBUFF_SIZE;i++)
    {
        SendBuff[i]	 = 'A';
    }
    /* USART1 向 DMA发出TX请求 */
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); 
   
	while(1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		Delay(1000);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		Delay(1000);	
	}

}





