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

/* TIMER Table */
static timer_registerType *const timerBase[] =
    {
        TIMER0,
        TIMER1,
        TIMER2,
        TIMER3,
        TIMER4,
        TIMER5
    };

/************************************* Function Implementations************************************/

void TIM0A_IRQHandler(void)
{
    timer_interruptHandler(TIMER_0);

}
