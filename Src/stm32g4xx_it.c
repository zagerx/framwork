/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "usart.h"
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */
#define rt_uint32_t unsigned int
struct exception_info
{
    rt_uint32_t exc_return;
    rt_uint32_t r4;
    rt_uint32_t r5;
    rt_uint32_t r6;
    rt_uint32_t r7;
    rt_uint32_t r0;
    rt_uint32_t r1;
    rt_uint32_t r2;
    rt_uint32_t r3;
    rt_uint32_t r12;
    rt_uint32_t lr;
    rt_uint32_t pc;
    rt_uint32_t psr;
};
/*
 * fault exception handler
 */
void rt_hw_hard_fault_exception(struct exception_info * exception_info)
{
    printf("psr: 0x%08x\r\n", exception_info->psr);
    printf("r00: 0x%08x\r\n", exception_info->r0);
    printf("r01: 0x%08x\r\n", exception_info->r1);
    printf("r02: 0x%08x\r\n", exception_info->r2);
    printf("r03: 0x%08x\r\n", exception_info->r3);
    printf("r04: 0x%08x\r\n", exception_info->r4);
    printf("r05: 0x%08x\r\n", exception_info->r5);
    printf("r06: 0x%08x\r\n", exception_info->r6);
    printf("r07: 0x%08x\r\n", exception_info->r7);
    printf("r12: 0x%08x\r\n", exception_info->r12);
    printf(" lr: 0x%08x\r\n", exception_info->lr);
    printf(" pc: 0x%08x\r\n", exception_info->pc);
	printf("\r\n");
    while (1);
}


void hard_fault_handler_c(unsigned int * hardfault_args, unsigned lr_value)
{
			unsigned int stacked_r0;
			unsigned int stacked_r1;
			unsigned int stacked_r2;
			unsigned int stacked_r3;
			unsigned int stacked_r12;
			unsigned int stacked_lr;
			unsigned int stacked_pc;
			unsigned int stacked_psr;
			stacked_r0 = ((unsigned long) hardfault_args[0]);
			stacked_r1 = ((unsigned long) hardfault_args[1]);
			stacked_r2 = ((unsigned long) hardfault_args[2]);
			stacked_r3 = ((unsigned long) hardfault_args[3]);
			stacked_r12 = ((unsigned long) hardfault_args[4]);
			stacked_lr = ((unsigned long) hardfault_args[5]);
			stacked_pc = ((unsigned long) hardfault_args[6]);
			stacked_psr = ((unsigned long) hardfault_args[7]);
      printf ("[Hard fault handler]\r\n");
			printf ("R0  = %x\r\n", stacked_r0);
			printf ("R1  = %x\r\n", stacked_r1);
			printf ("R2  = %x\r\n", stacked_r2);
			printf ("R3  = %x\r\n", stacked_r3);
			printf ("R12 = %x\r\n", stacked_r12);
			printf ("Stacked LR  = %x\r\n", stacked_lr);
			printf ("Stacked PC  = %x\r\n", stacked_pc);
			printf ("Stacked PSR = %x\r\n", stacked_psr);
			printf ("Current LR  = %x\r\n", lr_value);	
			while(1)
			{

 
			for(int i = 10000;i>0;i--)
			    for(int j = 1000;j>0;j--);
			}
			
			//while(1); // endless loop
}








/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */
  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
// void HardFault_Handler(void)
// {
//   /* USER CODE BEGIN HardFault_IRQn 0 */
// //     printf("hardfault irq\r\n");

//   /* USER CODE END HardFault_IRQn 0 */
//   while (1)
//   {
//     /* USER CODE BEGIN W1_HardFault_IRQn 0 */
//     /* USER CODE END W1_HardFault_IRQn 0 */
//   }
// }

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  USER_UART_IRQHandler(&huart1);
  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
