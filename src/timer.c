/***************************************************************************************************
 * FILENAME : timer.c
 * DESCRIPTION : Contains timer driver function definitions.
 *
 * NOTES :
 *
 * AUTHOR : Hassan
 ***************************************************************************************************/

/*************************************** Header Inclusion******************************************/
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
        TIMER5};

/************************************* Function Implementations************************************/
/**
 * @brief Timer_init : Initializes a timer.
 *
 * @param config : Pointer to timer configuration.
 *
 * @return timer_errorType
 */
timer_errorType timer_init(timer_configType *config)
{
    timer_registerType *timer = NULL;

    /* Verify configuration pointer */
    if (config == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Verify timer number */
    if (config->number >= TIMER_INVALID)
    {
        return TIMER_INVALID_TIMER;
    }

    /* Verify timer channel */
    if (config->channel >= TIMER_CHANNEL_INVALID)
    {
        return TIMER_INVALID_CHANNEL;
    }

    /* Validate supported timer size and channel combination */
    if ((config->size == TIMER_SIZE_32_BIT) &&
        (config->channel != TIMER_AB))
    {
        return TIMER_INVALID_CONFIG;
    }
    if ((config->size == TIMER_SIZE_16_BIT) &&
        (config->channel == TIMER_AB))
    {
        return TIMER_INVALID_CONFIG;
    }

    timer = timerBase[config->number];

    /* Enable Timer Clock for bus */
    SYSCTL_RCGCTIMER |= (1U << config->number);

    /* Disable selected timer before applying configuration */
    switch (config->channel)
    {
    case TIMER_A:
        timer->GPTMCTL &= ~(1U << GPTMCTL_TAEN_BIT);
        break;

    case TIMER_B:
        timer->GPTMCTL &= ~(1U << GPTMCTL_TBEN_BIT);
        break;

    case TIMER_AB:
        timer->GPTMCTL &= ~(1U << GPTMCTL_TAEN_BIT);
        break;
    }

    /* Configure timer as 16-bit split or 32-bit concatenated mode */
    if (config->size == TIMER_SIZE_16_BIT)
    {
        timer->GPTMCFG = 0x4U;
    }
    else
    {
        timer->GPTMCFG = 0x0U;
    }

    switch (config->channel)
    {
    case TIMER_A:
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
        if (config->direction == TIMER_COUNT_DOWN)
        {
            timer->GPTMTAMR &= ~(1U << GPTMTAMR_TACDIR_BIT);
        }
        else
        {
            timer->GPTMTAMR |= (1U << GPTMTAMR_TACDIR_BIT);
        }

        /* Configure prescaler */
        timer->GPTMTAPR = config->prescaler;
        break;

    case TIMER_B:
        /* Configure timer mode */
        switch (config->mode)
        {
        case TIMER_MODE_ONE_SHOT:
            timer->GPTMTBMR = 0x1U;
            break;

        case TIMER_MODE_PERIODIC:
            timer->GPTMTBMR = 0x2U;
            break;

        default:
            return TIMER_INVALID_MODE;
        }

        /* Configure count direction */
        if (config->direction == TIMER_COUNT_DOWN)
        {
            timer->GPTMTBMR &= ~(1U << GPTMTBMR_TBCDIR_BIT);
        }
        else
        {
            timer->GPTMTBMR |= (1U << GPTMTBMR_TBCDIR_BIT);
        }

        /* Configure prescaler */
        timer->GPTMTBPR = config->prescaler;
        break;
    case TIMER_AB:
        /* Configure concatenated 32-bit timer using Timer A registers */
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
        if (config->direction == TIMER_COUNT_DOWN)
        {
            timer->GPTMTAMR &= ~(1U << GPTMTAMR_TACDIR_BIT);
        }
        else
        {
            timer->GPTMTAMR |= (1U << GPTMTAMR_TACDIR_BIT);
        }

        /* Configure prescaler */
        timer->GPTMTAPR = config->prescaler;

        break;
    default:
        return TIMER_INVALID_CHANNEL;
    }

    return TIMER_SUCCESS;
}

timer_errorType timer_blockingDelay(timer_configType *config, uint32_t delay)
{
    timer_registerType *timer = NULL;

    /* Verify configuration pointer */
    if (config == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Verify timer number */
    if (config->number >= TIMER_INVALID)
    {
        return TIMER_INVALID_TIMER;
    }

    /* Verify timer channel */
    if (config->channel >= TIMER_CHANNEL_INVALID)
    {
        return TIMER_INVALID_CHANNEL;
    }

    timer = timerBase[config->number];

    /* Disable selected timer before applying configuration */
    switch (config->channel)
    {
    case TIMER_A:
        timer->GPTMCTL &= ~(1U << GPTMCTL_TAEN_BIT);
        break;

    case TIMER_B:
        timer->GPTMCTL &= ~(1U << GPTMCTL_TBEN_BIT);
        break;

    case TIMER_AB:
        timer->GPTMCTL &= ~(1U << GPTMCTL_TAEN_BIT);
        break;
    }

    /* Calculate Delay value */

    uint32_t timerFrequency;
    uint32_t timerCounts;

    timerFrequency = SYSTEM_CLOCK_HZ / (config->prescaler + 1U);

    switch (config->unit)
    {
    case TIMER_US:

        timerCounts = (timerFrequency / 1000000U) * delay;
        break;

    case TIMER_MS:

        timerCounts = (timerFrequency / 1000U) * delay;
        break;

    case TIMER_SEC:

        timerCounts = timerFrequency * delay;
        break;

    default:

        return TIMER_INVALID_CONFIG;
    }

    volatile uint32_t *loadRegister = NULL;
    uint32_t enableBit;
    uint32_t timeoutBit;
    uint32_t clearBit;

    switch (config->channel)
    {
    case TIMER_A:
        loadRegister = &timer->GPTMTAILR;
        enableBit = GPTMCTL_TAEN_BIT;
        timeoutBit = GPTMRIS_TATORIS_BIT;
        clearBit = GPTMICR_TATOCINT_BIT;
        break;

    case TIMER_B:
        loadRegister = &timer->GPTMTBILR;
        enableBit = GPTMCTL_TBEN_BIT;
        timeoutBit = GPTMRIS_TBTORIS_BIT;
        clearBit = GPTMICR_TBTOCINT_BIT;
        break;

    case TIMER_AB:
        loadRegister = &timer->GPTMTAILR;
        enableBit = GPTMCTL_TAEN_BIT;
        timeoutBit = GPTMRIS_TATORIS_BIT;
        clearBit = GPTMICR_TATOCINT_BIT;
        break;

    default:
        return TIMER_INVALID_CHANNEL;
    }

    /* Load delay value */
    *loadRegister = timerCounts;

    /* Clear previous timeout */
    timer->GPTMICR = (1U << clearBit);

    /* Enable timer */
    timer->GPTMCTL |= (1U << enableBit);

    /* Wait for timeout */
    while ((timer->GPTMRIS & (1U << timeoutBit)) == 0U)
        ;

    /* Clear timeout flag */
    timer->GPTMICR = (1U << clearBit);

    /* Disable timer */
    timer->GPTMCTL &= ~(1U << enableBit);

    return TIMER_SUCCESS;
}
