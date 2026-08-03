/***************************************************************************************************
* FILENAME    : clock.h
* DESCRIPTION : Contains system clock configuration definitions and APIs.
*
* NOTES :
*
* AUTHOR      : Hassan
***************************************************************************************************/

#ifndef __CLOCK_H_
#define __CLOCK_H_

/*************************************** Header Inclusion *****************************************/

#include <stdint.h>
#include <stddef.h>

/********************************************* Macros *********************************************/

/************************************ System Control Base *****************************************/

#define SYSCTL_BASE                     (0x400FE000U)

/************************************ System Control Registers ************************************/

#define SYSCTL_RIS                      (*(volatile uint32_t *)(SYSCTL_BASE + 0x050U))
#define SYSCTL_RCC                      (*(volatile uint32_t *)(SYSCTL_BASE + 0x060U))
#define SYSCTL_RCC2                     (*(volatile uint32_t *)(SYSCTL_BASE + 0x070U))

/*************************************** RCC Register **********************************************/

/* Crystal Value */
#define RCC_XTAL_MASK                   (0x1FU << 6U)
#define RCC_XTAL_16MHZ                  (0x15U << 6U)

/* Enable System Divider */
#define RCC_USESYSDIV                   (1UL << 22U)

/************************************** RCC2 Register **********************************************/

/* Use RCC2 Register */
#define RCC2_USERCC2                    (1UL << 31U)

/* Enable 400 MHz PLL */
#define RCC2_DIV400                     (1UL << 30U)

/* System Divider */
/* System Divider */
#define RCC2_SYSDIV2_MASK              (0x3FU << 23U)
#define RCC2_SYSDIV2_80MHZ             (0x02U << 23U)
#define RCC2_SYSDIV2LSB                (1UL << 22U)

/* PLL Control */
#define RCC2_PWRDN2                     (1UL << 13U)
#define RCC2_BYPASS2                    (1UL << 11U)

/* Oscillator Source */
#define RCC2_OSCSRC2_MASK               (0x7U << 4U)
#define RCC2_OSCSRC2_MOSC               (0x0U << 4U)

/*************************************** RIS Register **********************************************/

#define RIS_PLLLRIS                     (1UL << 6U)

/************************************** Clock Frequencies *****************************************/

#define CLOCK_FREQ_16MHZ                (16000000U)
#define CLOCK_FREQ_80MHZ                (80000000U)
#define SYSTEM_CLOCK_HZ 80000000U

/****************************************** Enumerations ******************************************/

typedef enum
{
    CLOCK_SUCCESS = 0,
    CLOCK_INVALID_CONFIG,
    CLOCK_INVALID_FREQUENCY

} clock_errorType;

typedef enum
{
    CLOCK_SOURCE_MOSC = 0,
    CLOCK_SOURCE_PLL,
    CLOCK_SOURCE_INVALID

} clock_sourceType;

/******************************************* Data Types ********************************************/

typedef struct
{
    clock_sourceType source;
    uint32_t frequency;

} clock_configType;

/*************************************** Function Prototypes **************************************/

clock_errorType clock_init(const clock_configType *config);
uint32_t clock_getSystemFrequency(void);

#endif /* __CLOCK_H_ */