/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "led_driver.h"
//#include "plane.h"
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
static const uint8_t LEDMAT_ADD = 0x75 << 1;  //LED Address
uint16_t Xaxis = 0;
uint16_t Yaxis = 0;
uint16_t rawValues[2];
char msg[32];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
////the code below is an interrupt that is triggered on an Update event (i.e. when the timer reaches its ARR value)
////if you look at TIM2 in the .ioc file (the GUI tab) you will see it has a source of 84MHz, an ARR of 84000-1 and
////a prescaler of 10, meaning this interrupt will occur every 10 ms.
uint8_t tank_position = 2;
uint16_t score = 0; //game score
uint8_t poweroff = 0; // game on/off

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim2){
  	////this updates the X and Y axes of my joystick
	  for(uint8_t i = 0; i<hadc1.Init.NbrOfConversion; i++){
		  Xaxis = (uint16_t) rawValues[0];
		  Yaxis = (uint16_t) rawValues[1];
	  }
	 ////This allows me to write to the serial monitor similar to the serial monitor on Arduino
	 ////Note STM32CUBEIDE does not have a serial monitor. You will need to download PuTTY to view
	 ////your serial output.
//	  sprintf(msg, "X axis: %hu \r\t\t", Xaxis);
//	  HAL_UART_Transmit(&huart2, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
//
//	  sprintf(msg, "Y axis: %hu \r\n", Yaxis);
//	  HAL_UART_Transmit(&huart2, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	  if (poweroff == 0){
		  sprintf(msg, "Score: %hu \r\n", score);
		  HAL_UART_Transmit(&huart2, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);
	  }

	  if (Xaxis >3000){
		  removecar(0,tank_position);
		  tank_position--;
	  }
	  if (Xaxis <1200){
		  removecar(0,tank_position);
		  tank_position++;
	  }
  }
uint8_t button_press_count = 0;
uint8_t times_count = 0; // smooth input



/* USER CODE END 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET && button_press_count==2 ) // 进入第二次时候 加一个暂停的中断
    {
		button_press_count++;
		// 在串口中打印当前时间和按钮按下次数
		sprintf(msg, "Time:%d Button Pressed: %d\r\n", times_count, button_press_count);
		HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

    }
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET && button_press_count==3 ) // 进入暂停后再离开回到第二次
	{
		button_press_count = 2;
			// 在串口中打印当前时间和按钮按下次数
		sprintf(msg, "Time:%d Button Pressed: %d\r\n", times_count, button_press_count);
		HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

	}
}
/**
  * @brief  The application entry point.
  * @retval int
  */


int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  ////This begins the process of storing our ADC readings into the DMA. The DMA can be thought of a temporary storage location.
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *) rawValues, 2);
  ////This begins our timer 2
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
      {
      if( HAL_I2C_IsDeviceReady(&hi2c1, LEDMAT_ADD, 2, HAL_MAX_DELAY) == HAL_OK && poweroff == 0){
    	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET && times_count%10 == 9) {
  		   // 如果按下，增加按下次数
    		  button_press_count++;
    	  }

    	  times_count++;
		  HAL_Delay(25); // 增加延迟以避免按下时多次触发
		  sprintf(msg, "Time:%d Button Pressed: %d\r\n",times_count,button_press_count);
		  HAL_UART_Transmit(&huart2, (uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);


		  if (button_press_count == 1 && times_count%100 == 99) { // pwm display
			  turnoffscreen();
			  turnonscreen();
			  setpwm(0x04);
			  HAL_Delay(1);
			  for(uint8_t colu=0;colu<7;colu++){
				  for (uint8_t ro=0;ro<11;ro++){
					  setpwm(0x04+ro*4);
					  addpixel(ro,colu);
				  }
			  }

			  for(uint8_t colu=0;colu<7;colu++){ // 复位
				  for (uint8_t ro=0;ro<11;ro++){
					  setpwm(0x04+ro*4);
					  removepixel(ro,colu);
				  }
			  }
			  times_count = 0;
			  button_press_count = 0;
			  clearscreen();
		  }

		   if (button_press_count == 3 && times_count%100 == 99) { // pause
			 clearscreen();
			 while(button_press_count == 3){
				 Cube2();
				 addcar(0,tank_position);
			 }
		   }


  		   // 如果按下次数达到 2，则执行后续操作
		   if (button_press_count == 2 && times_count%100 == 99 && poweroff ==0 ) {

			   times_count =195;
			   uint8_t c = tank_position;
			   // 执行 LED 点亮的 for 循环
			   turnoffscreen();
			   turnonscreen();
			   clearscreen();
			   Cube2();

			   while(c == tank_position && button_press_count == 2 ){
				   addcar(0,c);
				   HAL_Delay(100);

				   if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)== GPIO_PIN_RESET){
					   uint8_t flag = 0;
					   for(uint8_t time = 0; time <10; time++){
						   HAL_Delay(200);
						   if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)!= GPIO_PIN_RESET){
							   flag = 1;
							   break;
						   }
					   }
					   if (flag == 0){
						   poweroff = 1;
						   break;
					   }
				   }


				   if ((c <=2)||(c>=5)){
					   for(uint8_t r = 0; r <6; r++){

						   addpixel(r+3,c);
						   HAL_Delay(100);
						   removepixel(r+3,c);
					   }
				   }
				   else{
					   score++;
					   for(uint8_t r = 0; r <8; r++){
							 Cube2();
							 addpixel(r+3,c);
							 HAL_Delay(100);
							 removepixel(r+3,c);
					   }
				   }
			   }
		   }

      }

      else{ // End game
    	  turnoffscreen();
    	  sprintf(msg, "End of Game. \n Final Score: %hu \r\n", score);
    	  HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    	  break;
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
