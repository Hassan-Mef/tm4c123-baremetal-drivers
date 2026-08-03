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

/************************************* Function Implementations ************************************/


/**
 * @brief TIM0A_IRQHandler : Handles Timer0A interrupt.
 *
 * Calls the timer driver interrupt handler for Timer0A.
 *
 * @return void
 */
void TIM0A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_0, TIMER_A);
}

/**
 * @brief TIM0B_IRQHandler : Handles Timer0B interrupt.
 *
 * Calls the timer driver interrupt handler for Timer0B.
 *
 * @return void
 */
void TIM0B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_0, TIMER_B);
}

/**
 * @brief TIM1A_IRQHandler : Handles Timer1A interrupt.
 *
 * Calls the timer driver interrupt handler for Timer1A.
 *
 * @return void
 */
void TIM1A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_1, TIMER_A);
}

/**
 * @brief TIM1B_IRQHandler : Handles Timer1B interrupt.
 *
 * Calls the timer driver interrupt handler for Timer1B.
 *
 * @return void
 */
void TIM1B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_1, TIMER_B);
}

/**
 * @brief TIM2A_IRQHandler : Handles Timer2A interrupt.
 *
 * Calls the timer driver interrupt handler for Timer2A.
 *
 * @return void
 */
void TIM2A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_2, TIMER_A);
}

/**
 * @brief TIM2B_IRQHandler : Handles Timer2B interrupt.
 *
 * Calls the timer driver interrupt handler for Timer2B.
 *
 * @return void
 */
void TIM2B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_2, TIMER_B);
}

/**
 * @brief TIM3A_IRQHandler : Handles Timer3A interrupt.
 *
 * Calls the timer driver interrupt handler for Timer3A.
 *
 * @return void
 */
void TIM3A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_3, TIMER_A);
}

/**
 * @brief TIM3B_IRQHandler : Handles Timer3B interrupt.
 *
 * Calls the timer driver interrupt handler for Timer3B.
 *
 * @return void
 */
void TIM3B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_3, TIMER_B);
}

/**
 * @brief TIM4A_IRQHandler : Handles Timer4A interrupt.
 *
 * Calls the timer driver interrupt handler for Timer4A.
 *
 * @return void
 */
void TIM4A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_4, TIMER_A);
}

/**
 * @brief TIM4B_IRQHandler : Handles Timer4B interrupt.
 *
 * Calls the timer driver interrupt handler for Timer4B.
 *
 * @return void
 */
void TIM4B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_4, TIMER_B);
}

/**
 * @brief TIM5A_IRQHandler : Handles Timer5A interrupt.
 *
 * Calls the timer driver interrupt handler for Timer5A.
 *
 * @return void
 */
void TIM5A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_5, TIMER_A);
}

/**
 * @brief TIM5B_IRQHandler : Handles Timer5B interrupt.
 *
 * Calls the timer driver interrupt handler for Timer5B.
 *
 * @return void
 */
void TIM5B_IRQHandler(void)
{
    timer_interruptHandler(TIMER_5, TIMER_B);
}

/**
 * @brief UART0_IRQHandler : Handles UART0 receive interrupt.
 *
 * Calls the UART driver interrupt handler for UART0.
 *
 * @return void
 */
void UART0_IRQHandler(void)
{
    uart_interruptHandler(UART_0);
}

/**
 * @brief UART1_IRQHandler : Handles UART1 receive interrupt.
 *
 * Calls the UART driver interrupt handler for UART1.
 *
 * @return void
 */
void UART1_IRQHandler(void)
{
    uart_interruptHandler(UART_1);
}

/**
 * @brief UART2_IRQHandler : Handles UART2 receive interrupt.
 *
 * Calls the UART driver interrupt handler for UART2.
 *
 * @return void
 */
void UART2_IRQHandler(void)
{
    uart_interruptHandler(UART_2);
}

/**
 * @brief UART3_IRQHandler : Handles UART3 receive interrupt.
 *
 * Calls the UART driver interrupt handler for UART3.
 *
 * @return void
 */
void UART3_IRQHandler(void)
{
    uart_interruptHandler(UART_3);
}

/**
 * @brief UART4_IRQHandler : Handles UART4 receive interrupt.
 *
 * Calls the UART driver interrupt handler for UART4.
 *
 * @return void
 */
void UART4_IRQHandler(void)
{
    uart_interruptHandler(UART_4);
}

/**
 * @brief UART5_IRQHandler : Handles UART5 receive interrupt.
 *
 * Calls the UART driver interrupt handler for UART5.
 *
 * @return void
 */
void UART5_IRQHandler(void)
{
    uart_interruptHandler(UART_5);
}

/**
 * @brief UART6_IRQHandler : Handles UART6 receive interrupt.
 *
 * Calls the UART driver interrupt handler for UART6.
 *
 * @return void
 */
void UART6_IRQHandler(void)
{
    uart_interruptHandler(UART_6);
}

/**
 * @brief UART7_IRQHandler : Handles UART7 receive interrupt.
 *
 * Calls the UART driver interrupt handler for UART7.
 *
 * @return void
 */
void UART7_IRQHandler(void)
{
    uart_interruptHandler(UART_7);
}