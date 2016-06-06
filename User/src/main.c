/**
  ******************************************************************************
  * @file    main.c
  * @author  ณฬัว
  * @version V1.0
  * @date    20-May-2013
  * @brief   
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x.h"

#include "mb.h"
/* Define  variable------------------------------------------------------------*/


/**
  * @brief  
  * @note   None
  * @param  None
  * @retval None
  */
int main(void)
{
	eMBInit(MB_RTU, 0x01, 0, 9600, MB_PAR_NONE);
	eMBEnable();	
	while(1)
	{
		eMBPoll();	
	}
}
