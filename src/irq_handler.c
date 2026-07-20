/***************************************************************************************************
 * FILENAME : irq_handler.c
 * DESCRIPTION : Timer interrupt service routines.
 *
 * NOTES :
 *
 * AUTHOR : Hassan
 ***************************************************************************************************/

/*************************************** Header Inclusion******************************************/
#include "irq_handler.h"
#include "timer.h"
#include "uart.h"

/********************************************* Globals ********************************************/

/************************************* Function Implementations************************************/


void TIM0A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_0, TIMER_A);
}

void TIM0B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_0, TIMER_B);
}

void TIM1A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_1, TIMER_A);
}

void TIM1B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_1, TIMER_B);
}

void TIM2A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_2, TIMER_A);
}

void TIM2B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_2, TIMER_B);
}

void TIM3A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_3, TIMER_A);
}

void TIM3B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_3, TIMER_B);
}

void TIM4A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_4, TIMER_A);
}

void TIM4B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_4, TIMER_B);
}

void TIM5A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_5, TIMER_A);
}

void TIM5B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_5, TIMER_B);
}

void UART0_IRQHandler(void)
{
    uart_interruptHandler(UART_0);
}

void UART1_IRQHandler(void)
{
    uart_interruptHandler(UART_1);
}

void UART2_IRQHandler(void)
{
    uart_interruptHandler(UART_2);
}

void UART3_IRQHandler(void)
{
    uart_interruptHandler(UART_3);
}

void UART4_IRQHandler(void)
{
    uart_interruptHandler(UART_4);
}

void UART5_IRQHandler(void)
{
    uart_interruptHandler(UART_5);
}

void UART6_IRQHandler(void)
{
    uart_interruptHandler(UART_6);
}

void UART7_IRQHandler(void)
{
    uart_interruptHandler(UART_7);
}

