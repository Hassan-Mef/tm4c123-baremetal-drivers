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
    TIMER5
};

typedef struct
{
    timer_irqNumberType A;
    timer_irqNumberType B;
} timer_irqChannelType;

static const timer_irqChannelType timerIrqTable[] =
{
    {.A = TIMER0A_IRQ, .B = TIMER0B_IRQ},
    {.A = TIMER1A_IRQ, .B = TIMER1B_IRQ},
    {.A = TIMER2A_IRQ, .B = TIMER2B_IRQ},
    {.A = TIMER3A_IRQ, .B = TIMER3B_IRQ},
    {.A = TIMER4A_IRQ, .B = TIMER4B_IRQ},
    {.A = TIMER5A_IRQ, .B = TIMER5B_IRQ}
};
        

static void (*timerCallback[TIMER_INVALID])(void) = {NULL, NULL, NULL, NULL, NULL, NULL};
     
/************************************* Function Implementations************************************/

/**
 * @brief timer_loadAndStart : Loads timer value and starts the timer.
 *
 * @param timer : Pointer to timer registers.
 * @param config : Pointer to timer configuration.
 * @param timerCounts : Timer load value.
 */
static void timer_loadAndStart(timer_registerType *timer, timer_configType *config, uint32_t timerCounts)
{
    switch (config->channel)
    {
    case TIMER_A:

        timer->GPTMTAILR = timerCounts;
        timer->GPTMICR = (1U << GPTMICR_TATOCINT_BIT);
        timer->GPTMCTL |= (1U << GPTMCTL_TAEN_BIT);

        break;

    case TIMER_B:

        timer->GPTMTBILR = timerCounts;
        timer->GPTMICR = (1U << GPTMICR_TBTOCINT_BIT);
        timer->GPTMCTL |= (1U << GPTMCTL_TBEN_BIT);

        break;

    case TIMER_AB:

        timer->GPTMTAILR = timerCounts;
        timer->GPTMICR = (1U << GPTMICR_TATOCINT_BIT);
        timer->GPTMCTL |= (1U << GPTMCTL_TAEN_BIT);

        break;

    default:
        break;
    }
}

/**
 * @brief timer_validateConfig : Validates timer configuration parameters.
 *
 * @param config : Pointer to timer configuration structure.
 *
 * @return timer_errorType
 */
static timer_errorType timer_validateConfig(timer_configType *config)
{
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

    return TIMER_SUCCESS;
}

/**
 * @brief timer_calculateCounts : Calculates timer load value.
 *
 * @param config : Pointer to timer configuration.
 * @param delay : Delay value provided by application.
 * @param timerCounts : Pointer to calculated timer counts.
 *
 * @return timer_errorType
 */
static timer_errorType timer_calculateCounts(timer_configType *config, uint32_t delay, uint32_t *timerCounts)
{
    uint32_t timerFrequency;
    uint32_t systemClock = clock_getSystemFrequency();

    /* Verify output pointer */
    if (timerCounts == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Calculate timer frequency */
    if(config->size == TIMER_SIZE_16_BIT)
    {
        timerFrequency = systemClock / (config->prescaler + 1U);
    }
    else if(config->size == TIMER_SIZE_32_BIT)
    {
        timerFrequency = systemClock ;
    }
    else
    {
        return TIMER_INVALID_SIZE;
    }

    /* Calculate timer counts based on selected time unit */
    switch (config->unit)
    {
    case TIMER_US:
        *timerCounts = (timerFrequency / TIMER_MICRO_DIVISION_FACTOR) * delay;
        break;

    case TIMER_MS:
        *timerCounts = ((timerFrequency / TIMER_MILLI_DIVISION_FACTOR) - TIMER_CALIBRATION_DELAY_MS) * delay;
        break;

    case TIMER_SEC:
        *timerCounts = (timerFrequency- TIMER_CALIBRATION_DELAY_SEC) * delay;
        break;

    default:
        return TIMER_INVALID_CONFIG;
    }

    return TIMER_SUCCESS;
}

/**
 * @brief timer_disable : Disables selected timer channel.
 *
 * @param timer : Pointer to timer registers.
 * @param channel : Timer channel.
 */
static void timer_disable(timer_registerType *timer, timer_subType channel)
{
    /* Disable selected timer channel */
    switch (channel)
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

    default:
        break;
    }
}

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
    timer_irqNumberType irqNumber;

    /* Validate configuration */
    timer_errorType status = timer_validateConfig(config);
    if (status != TIMER_SUCCESS)
    {
        return status;
    }

    timer = timerBase[config->number];

    /* Enable Timer Clock for bus */
    SYSCTL_RCGCTIMER |= (1U << config->number);

    /* Disable selected timer before applying configuration */
    timer_disable(timer, config->channel);

    /* Configure timer as 16-bit split or 32-bit concatenated mode */
    if (config->size == TIMER_SIZE_16_BIT)
    {
        timer->GPTMCFG = GPTMCFG_16_BIT;
    }
    else
    {
        timer->GPTMCFG = GPTMCFG_32_BIT;
    }

    /* Configure timer channel */
    switch (config->channel)
    {
    case TIMER_A:
        /* Configure timer mode */
        switch (config->mode)
        {
        case TIMER_MODE_ONE_SHOT:
            timer->GPTMTAMR = GPTM_MODE_ONE_SHOT;
            break;

        case TIMER_MODE_PERIODIC:
            timer->GPTMTAMR = GPTM_MODE_PERIODIC;
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

        /*Check for interrupt */
        if (config->interrupt == TIMER_INTERRUPT_ENABLE)
        {
            timer->GPTMIMR |= (1U << GPTMIMR_TATOIM_BIT);
            /* Enable NVIC interrupt for the timer */
            irqNumber = timerIrqTable[config->number].A;

            NVIC_ENABLE_BASE[irqNumber / IRQ_REGISTER_DIVISION_FACTOR] |= (1U << (irqNumber % IRQ_REGISTER_DIVISION_FACTOR));
        }
        else
        {
            timer->GPTMIMR &= ~(1U << GPTMIMR_TATOIM_BIT);
        }

        break;

    case TIMER_B:
        /* Configure timer mode */
        switch (config->mode)
        {
        case TIMER_MODE_ONE_SHOT:
            timer->GPTMTBMR = GPTM_MODE_ONE_SHOT;
            break;

        case TIMER_MODE_PERIODIC:
            timer->GPTMTBMR = GPTM_MODE_PERIODIC;
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

        /*Check for interrupt */
        if (config->interrupt == TIMER_INTERRUPT_ENABLE)
        {
            timer->GPTMIMR |= (1U << GPTMIMR_TBTOIM_BIT);
            /* Enable NVIC interrupt for the timer */
            irqNumber = timerIrqTable[config->number].B;

            NVIC_ENABLE_BASE[irqNumber / IRQ_REGISTER_DIVISION_FACTOR] |= (1U << (irqNumber % IRQ_REGISTER_DIVISION_FACTOR));
        }
        else
        {
            timer->GPTMIMR &= ~(1U << GPTMIMR_TBTOIM_BIT);
        }

        break;

    case TIMER_AB:
        /* Configure concatenated 32-bit timer using Timer A registers */
        /* Configure timer mode */
        switch (config->mode)
        {
        case TIMER_MODE_ONE_SHOT:
            timer->GPTMTAMR = GPTM_MODE_ONE_SHOT;
            break;

        case TIMER_MODE_PERIODIC:
            timer->GPTMTAMR = GPTM_MODE_PERIODIC;
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
        timer->GPTMTAPR = 0;

        /*Check for interrupt */
        if (config->interrupt == TIMER_INTERRUPT_ENABLE)
        {
            timer->GPTMIMR |= (1U << GPTMIMR_TATOIM_BIT);
            /* Enable NVIC interrupt for the timer */
            irqNumber = timerIrqTable[config->number].A;

            NVIC_ENABLE_BASE[irqNumber / IRQ_REGISTER_DIVISION_FACTOR] |= (1U << (irqNumber % IRQ_REGISTER_DIVISION_FACTOR));
        }
        else
        {
            timer->GPTMIMR &= ~(1U << GPTMIMR_TATOIM_BIT);
        }
        break;

    default:
        return TIMER_INVALID_CHANNEL;
    }

    return TIMER_SUCCESS;
}

/**
 * @brief timer_blockingDelay : Generates a blocking delay using a timer.
 *
 *
 * @param config : Pointer to timer configuration.
 * @param delay : Delay value.
 *
 * @return timer_errorType
 */
timer_errorType timer_blockingDelay(timer_configType *config, uint32_t delay)
{
    timer_registerType *timer = NULL;

    timer_errorType status;
    uint32_t timerCounts;
    
    /* Register pointers and bits */
    volatile uint32_t *loadRegister = NULL;
    uint32_t enableBit;
    uint32_t timeoutBit;
    uint32_t clearBit;

    /* Validate timer configuration */
    status = timer_validateConfig(config);

    if (status != TIMER_SUCCESS)
    {
        return status;
    }

    /* Calculate timer load value */
    status = timer_calculateCounts(config, delay, &timerCounts);

    if (status != TIMER_SUCCESS)
    {
        return status;
    }

    /* Get timer base address */
    timer = timerBase[config->number];

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
    while ((timer->GPTMRIS & (1U << timeoutBit)) == 0U);

    /* Clear timeout flag */
    timer->GPTMICR = (1U << clearBit);

    /* Disable timer */
    timer->GPTMCTL &= ~(1U << enableBit);

    return TIMER_SUCCESS;
}

/**
 * @brief timer_start : Starts a configured timer.
 *
 *
 * @param config : Pointer to timer configuration.
 * @param delay : Delay value.
 *
 * @return timer_errorType
 */
timer_errorType timer_start(timer_configType *config, uint32_t delay)
{
    timer_registerType *timer = NULL;
    /* Calculate timer counts */
    uint32_t timerCounts;

    /* Verify configuration pointer */
    timer_errorType status = timer_validateConfig(config);
    if (status != TIMER_SUCCESS)
    {
        return status;
    }

    timer = timerBase[config->number];

    /* Calculate timer load value */
    status = timer_calculateCounts(config, delay, &timerCounts);

    if (status != TIMER_SUCCESS)
    {
        return status;
    }

    /* Load timer value and start timer */
    timer_loadAndStart(timer, config, timerCounts);

    return TIMER_SUCCESS;
}

/**
 * @brief timer_interruptHandler : Handles timer timeout interrupt.
 * Clears the interrupt flag and executes the registered callback,
 * if one has been registered for the timer.
 *
 * @param timer : Timer number.
 * @param channel : Timer channel.
 */
timer_errorType timer_interruptHandler(timer_numberType timer, timer_subType channel)
{
    if (timer >= TIMER_INVALID)
    {
        return TIMER_INVALID_TIMER;
    }

    switch (channel)
    {
    case TIMER_A:
        timerBase[timer]->GPTMICR = (1U << GPTMICR_TATOCINT_BIT);
        break;

    case TIMER_B:
        timerBase[timer]->GPTMICR = (1U << GPTMICR_TBTOCINT_BIT);
        break;

    default:
        return TIMER_INVALID_CHANNEL;
    }

    if (timerCallback[timer] != NULL)
    {
        timerCallback[timer]();
    }

    return TIMER_SUCCESS;
}

/**
 * @brief timer_setCallback : Registers a callback function for a timer.
 * Stores the callback function associated with the specified timer.
 *
 * @param timer : Timer number.
 * @param callback : Pointer to callback function.
 *
 * @return timer_errorType
 */
timer_errorType timer_setCallback(timer_numberType timer, void (*callback)(void))
{
    /* Verify timer number */
    if (timer >= TIMER_INVALID)
    {
        return TIMER_INVALID_TIMER;
    }

    /* Verify callback pointer */
    if (callback == NULL)
    {
        return TIMER_NULL_POINTER;
    }

    /* Register callback function */
    timerCallback[timer] = callback;

    return TIMER_SUCCESS;
}