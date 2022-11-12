/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define Vref 3.3			//参考电压
#define Res 10000			//参考电阻
#define BaseDC 500		//占空比基准值
#define Factor 1			//占空比系数

uint16_t BaseNum=20;		//预设温度
uint16_t GetTemp;				//当前温度
uint16_t ADC_Value;			//存放A/D转换结果
uint16_t Vrt;						  //存放热敏电阻两端电压
uint16_t Rrt;						//存放热敏电阻阻值
uint32_t Receive_data;  //存放自定义预设温度

uint16_t DC;						//定义一个PWM占空比变量		
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  HAL_Delay(200);           //上电延迟2秒
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	//KEY初始化
	HAL_GPIO_WritePin(KEY1_GPIO_Port,KEY1_Pin,GPIO_PIN_SET);			
	HAL_GPIO_WritePin(KEY2_GPIO_Port,KEY2_Pin,GPIO_PIN_SET);
	HAL_ADCEx_Calibration_Start(&hadc1);					//ADC1校准
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);			//开启定时器2通道3输出PWM波形
	//AIN1写入高电平，AIN2写入低电平，电机顺时针转动
	HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET) ;
  HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
  printf("===The default temperature is %d degrees Celsius, enter a positive integer and press Enter to change===\n",BaseNum);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    //串口读取预设温度
    Receive_data=100;
		scanf("%d",&Receive_data);			//读取控制台输入数字
//    Receive_data=getint();     //读取控制台输入数字
    if(Receive_data!=100)
    {
      BaseNum=Receive_data;
      printf("The preset temperature has been set to %d degrees Celsius\n",BaseNum);
    }
		//按键控制预设温度
		if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_RESET)			//上拉输入按下为0（RESET）
		{
			HAL_Delay(10);			//10ms消抖
			if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_RESET)		//再次确定
			{
        BaseNum++;
        printf("The preset temperature has been set to %d degrees Celsius\n",BaseNum);
		  }
    }
		if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==GPIO_PIN_RESET)
		{
			HAL_Delay(10);
			if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==GPIO_PIN_RESET)
			{
        BaseNum--;
        printf("The preset temperature has been set to %d degrees Celsius\n",BaseNum);
		  }
    }
		HAL_Delay(200);
		HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&ADC_Value,sizeof(&ADC_Value));			//启动ADC1的DMA转换
		HAL_ADC_PollForConversion(&hadc1,50);																		//等待转换结束，超时时间50s
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1),HAL_ADC_STATE_REG_EOC))			//判断是否设置转换完成标志位，HAL_ADC_STATE_REG_EOC为转换完成标志位
		{
			ADC_Value=HAL_ADC_GetState(&hadc1);	//读取A/D转换数据，数据为12位
			Vrt=(Vref/(4096-1))*ADC_Value;			//计算热敏电阻两端电压
			Rrt=Vrt/((Vref-Vrt)/Res);						//计算热敏电阻阻值
			GetTemp=0.05*Rrt;										//直接计算温度，不使用对照表
      //串口打印
      printf("The voltage across the thermistor is %d V",Vrt);
      printf("Thermistor resistance is %d ohms",Rrt);
      printf("Current ambient temperature %d degrees Celsius\n",GetTemp);
			DC=Factor*(GetTemp-BaseNum)+BaseDC;	//计算占空比
			if(GetTemp>BaseNum)
			{
				DC++;
        __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,DC);											//启动风扇
				HAL_Delay(50);
			}
			else
			{
				//AIN1和AIN2都写入高电平，电机制动
				HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);																	
				HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);
			}
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
