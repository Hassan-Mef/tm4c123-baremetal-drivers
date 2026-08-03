/***************************************************************************************************
 * FILENAME : clock.c
 * DESCRIPTION : Contains system clock driver function definitions.
 *
 * NOTES :
 *
 * AUTHOR : Hassan
 ***************************************************************************************************/

/*************************************** Header Inclusion *****************************************/

#include "clock.h"

/********************************************* Globals ********************************************/

/* Current system clock frequency */
static uint32_t systemClockFrequency = CLOCK_FREQ_16MHZ;

/************************************* Function Implementations ************************************/

/**
 * @brief clock_init : Initializes the system clock.
 *
 * @param config : Pointer to clock configuration.
 *
 * @return clock_errorType
 */
clock_errorType clock_init(const clock_configType *config)
{
    /* Verify configuration pointer */
    if (config == NULL)
    {
        return CLOCK_INVALID_CONFIG;
    }

    /* Currently only 80 MHz PLL configuration is supported */
    if ((config->source != CLOCK_SOURCE_PLL) ||
        (config->frequency != CLOCK_FREQ_80MHZ))
    {
        return CLOCK_INVALID_FREQUENCY;
    }

    /* Use RCC2 register for advanced clock configuration */
    SYSCTL_RCC2 |= RCC2_USERCC2;

    /* Enable system clock divider */
    SYSCTL_RCC |= RCC_USESYSDIV;

    /* Bypass PLL while configuring the clock */
    SYSCTL_RCC2 |= RCC2_BYPASS2;

    /* Configure the external crystal frequency to 16 MHz */
    SYSCTL_RCC &= ~RCC_XTAL_MASK;
    SYSCTL_RCC |= RCC_XTAL_16MHZ;

    /* Select Main Oscillator (MOSC) as PLL clock source */
    SYSCTL_RCC2 &= ~RCC2_OSCSRC2_MASK;
    SYSCTL_RCC2 |= RCC2_OSCSRC2_MOSC;

    /* Enable system clock divider */
    SYSCTL_RCC |= RCC_USESYSDIV;

    /* Power up the PLL */
    SYSCTL_RCC2 &= ~RCC2_PWRDN2;

    /* Enable 400 MHz PLL operation */
    SYSCTL_RCC2 |= RCC2_DIV400;

    /* Configure system clock divider for 80 MHz */
    SYSCTL_RCC2 &= ~RCC2_SYSDIV2_MASK;
    SYSCTL_RCC2 |= RCC2_SYSDIV2_80MHZ;

    /* Clear additional divider LSB */
    SYSCTL_RCC2 &= ~RCC2_SYSDIV2LSB;

    /* Wait until PLL locks */
    while ((SYSCTL_RIS & RIS_PLLLRIS) == 0U)
    {

    }

    /* Switch system clock source to PLL */
    SYSCTL_RCC2 &= ~RCC2_BYPASS2;

    /* Save current system frequency */
    systemClockFrequency = CLOCK_FREQ_80MHZ;

    return CLOCK_SUCCESS;
}

/**
 * @brief clock_getSystemFrequency : Returns current system clock frequency.
 *
 * @return uint32_t
 */
uint32_t clock_getSystemFrequency(void)
{
    return systemClockFrequency;
}