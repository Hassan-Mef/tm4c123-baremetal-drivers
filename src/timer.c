/**************************************************************************************************
* FILENAME : gpio.c
* DESCRIPTION : Contains GPIO driver function definitions.
*
* NOTES :
*
* AUTHOR : Hassan
***************************************************************************************************/

/*************************************** Header Inclusion*****************************************/
#include "timer.h"

/********************************************* Globals ********************************************/

/* TIMER Table */

static timer_registerType * const timerBase[] =
{
    TIMER0,
    TIMER1,
    TIMER2,
    TIMER3,
    TIMER4,
    TIMER5
};

/************************************* Function Implementations***********************************/
/**
 * @brief Timer_init : Initializes a timer.
 *
 * @param config : Pointer to timer configuration.
 *
 * @return timer_errorType
 */
timer_errorType timer_init(timer_configType * config)
{
    timer_registerType *timer = NULL;

    /* Verify configuration pointer */
    if(config == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Verify timer number */
    if(config->number >= TIMER_INVALID)
    {
        return TIMER_INVALID_TIMER;
    }

    timer = timerBase[config->number];

    SYSCTL_RCGCTIMER |= (1U << config->number);

    /* Disable Timer A before configuration */
    timer->GPTMCTL &= ~(1U << GPTMCTL_TAEN_BIT);

    /* Configure timer size */
    if(config->size == TIMER_SIZE_16_BIT)
    {
        timer->GPTMCFG = 0x4U;
    }
    else
    {
        timer->GPTMCFG = 0x0U;
    }

    /* Configure timer mode */
    switch (config->mode)
    {
        case TIMER_MODE_ONE_SHOT:
            timer->GPTMTAMR = 0x1U;
            break;

        case TIMER_MODE_PERIODIC:
            timer->GPTMTAMR = 0x2U;
            break;

        default:
            return TIMER_INVALID_MODE;
    }

    /* Configure count direction */
    if(config->direction == TIMER_COUNT_DOWN)
    {
        timer->GPTMTAMR &= ~(1U << GPTMTAMR_TACDIR_BIT);
    }
    else
    {
        timer->GPTMTAMR |= (1U << GPTMTAMR_TACDIR_BIT);
    }

    /* Configure prescaler */
    timer->GPTMTAPR = config->prescaler;

    return TIMER_SUCCESS;
}
