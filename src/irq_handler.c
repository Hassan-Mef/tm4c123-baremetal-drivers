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