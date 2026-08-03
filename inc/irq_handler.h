/***************************************************************************************************
* FILENAME : IRQ_HANDLER.H
* DESCRIPTION :  contains function prototypes 
*
* NOTES :
*
* AUTHOR : Hassan
***************************************************************************************************/
#ifndef __IRQ_HANDLER_H__
#define __IRQ_HANDLER_H__
/*************************************** Header Inclusion *****************************************/

/********************************************* Macros *********************************************/


/****************************************** Enumerations ******************************************/

/******************************************* Data Types *******************************************/


/*************************************** Function Prototypes **************************************/

/* Timer Interrupt Handlers */
void TIM0A_IRQHandler(void);
void TIM0B_IRQHandler(void);

void TIM1A_IRQHandler(void);
void TIM1B_IRQHandler(void);

void TIM2A_IRQHandler(void);
void TIM2B_IRQHandler(void);

void TIM3A_IRQHandler(void);
void TIM3B_IRQHandler(void);

void TIM4A_IRQHandler(void);
void TIM4B_IRQHandler(void);

void TIM5A_IRQHandler(void);
void TIM5B_IRQHandler(void);


/* UART Interrupt Handlers */
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);
void UART2_IRQHandler(void);
void UART3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);
void UART6_IRQHandler(void);
void UART7_IRQHandler(void);


#endif