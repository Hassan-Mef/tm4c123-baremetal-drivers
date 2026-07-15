/***************************************************************************************************
* FILENAME : TIMER.H
* DESCRIPTION :  contains function prototypes 
*
* NOTES :
*
* AUTHOR : Hassan
***************************************************************************************************/

#ifndef __TIMER_H_
#define __TIMER_H_

/*************************************** Header Inclusion *****************************************/

#include <stddef.h>
#include <stdint.h>

/********************************************* Macros *********************************************/

/*************************************** System Clock *********************************************/

/* System Control Registers */
#define SYSCTL_RIS                     (*(volatile uint32_t *)(SYSCTL_BASE + 0x050U))
#define SYSCTL_RCC                     (*(volatile uint32_t *)(SYSCTL_BASE + 0x060U))
#define SYSCTL_RCC2                    (*(volatile uint32_t *)(SYSCTL_BASE + 0x070U))

/* RCC Register Fields */
#define RCC_XTAL_MASK                  (0x1FU << 6U)
#define RCC_XTAL_16MHZ                 (0x15U << 6U)
/* RCC Register Fields */
#define RCC_XTAL_MASK                  (0x1FU << 6U)
#define RCC_XTAL_16MHZ                 (0x15U << 6U)

#define RCC_USESYSDIV                  (1UL << 22U)

/* RCC2 Register Fields */
#define RCC2_USERCC2                   (1UL << 31U)
#define RCC2_DIV400                    (1UL << 30U)

#define RCC2_SYSDIV2_MASK              (0x3FU << 23U)
#define RCC2_SYSDIV2_80MHZ             (0x02U << 23U)

#define RCC2_SYSDIV2LSB                (1UL << 22U)

#define RCC2_PWRDN2                    (1UL << 13U)
#define RCC2_BYPASS2                   (1UL << 11U)

#define RCC2_OSCSRC2_MASK              (0x7U << 4U)
#define RCC2_OSCSRC2_MOSC              (0x0U << 4U)

/* RIS Register Fields */
#define RIS_PLLLRIS                    (1UL << 6U)



#define SYSCTL_BASE                    0x400FE000U

#define SYSCTL_RCGCTIMER_OFFSET        0x604U

#define SYSCTL_RCGCTIMER \
    (*(volatile uint32_t *)(SYSCTL_BASE + SYSCTL_RCGCTIMER_OFFSET))


#define SYSTEM_CLOCK_HZ                (80000000U)

#define TIMER_MAX_16_BIT_VALUE         (0xFFFFU)
#define TIMER_MAX_32_BIT_VALUE         (0xFFFFFFFFU)
#define TIMER_MICRO_DIVISION_FACTOR    (1000000U)
#define TIMER_MILLI_DIVISION_FACTOR    (1000U)
#define TIMER_CALIBRATION_DELAY_MS     (17U)
#define TIMER_CALIBRATION_DELAY_SEC    (17100U)


/* Timer Base Addresses */
#define TIMER0_BASE                    0x40030000U
#define TIMER1_BASE                    0x40031000U
#define TIMER2_BASE                    0x40032000U
#define TIMER3_BASE                    0x40033000U
#define TIMER4_BASE                    0x40034000U   
#define TIMER5_BASE                    0x40035000U


/* Timers */
#define TIMER0                         ((timer_registerType *) TIMER0_BASE)
#define TIMER1                         ((timer_registerType *) TIMER1_BASE)
#define TIMER2                         ((timer_registerType *) TIMER2_BASE)
#define TIMER3                         ((timer_registerType *) TIMER3_BASE)
#define TIMER4                         ((timer_registerType *) TIMER4_BASE)
#define TIMER5                         ((timer_registerType *) TIMER5_BASE)


#define GPTMCTL_TAEN_BIT               (0U)
#define GPTMCTL_TBEN_BIT               (8U)

#define GPTMTAMR_TACDIR_BIT            (4U)
#define GPTMTBMR_TBCDIR_BIT            (4U)

#define GPTMIMR_TATOIM_BIT             (0U)
#define GPTMIMR_TBTOIM_BIT             (8U)

#define GPTMRIS_TATORIS_BIT            (0U)
#define GPTMRIS_TBTORIS_BIT            (8U)

#define GPTMICR_TATOCINT_BIT           (0U)
#define GPTMICR_TBTOCINT_BIT           (8U)

#define GPTMCFG_16_BIT                 (0x4U)
#define GPTMCFG_32_BIT                 (0x0U)

#define GPTM_MODE_ONE_SHOT             (1U)
#define GPTM_MODE_PERIODIC             (2U)

#define IRQ_REGISTER_DIVISION_FACTOR   (32U)

#define NVIC_BASE                      0xE000E000U  
#define NVIC_EN0                       (*(volatile uint32_t *)(NVIC_BASE + 0x100U))
#define NVIC_EN1                       (*(volatile uint32_t *)(NVIC_BASE + 0x104U))
#define NVIC_EN2                       (*(volatile uint32_t *)(NVIC_BASE + 0x108U))
#define NVIC_EN3                       (*(volatile uint32_t *)(NVIC_BASE + 0x10CU))

#define NVIC_ENABLE_BASE               ((volatile uint32_t *)0xE000E100U)


/****************************************** Enumerations ******************************************/

typedef enum
{
    TIMER_0,
    TIMER_1,
    TIMER_2,
    TIMER_3,
    TIMER_4,
    TIMER_5,
    TIMER_INVALID
} timer_numberType;

typedef enum
{
    TIMER_SUCCESS = 16,
    TIMER_INVALID_TIMER,
    TIMER_INVALID_CONFIG,
    TIMER_INVALID_MODE,
    TIMER_NULL_POINTER,
    TIMER_INVALID_CHANNEL,
    TIMER_INVALID_SIZE,
} timer_errorType;

typedef enum
{
    TIMER_MODE_ONE_SHOT = 0,
    TIMER_MODE_PERIODIC,
    TIMER_MODE_CAPTURE,
    TIMER_MODE_RTC,
    TIMER_MODE_EDGE_COUNT,
    TIMER_MODE_EDGE_TIME,
    TIMER_MODE_PWM,
    TIMER_MODE_INVALID
} timer_modeType;

typedef enum
{
    TIMER_COUNT_UP = 0,
    TIMER_COUNT_DOWN,
    TIMER_COUNT_INVALID
} timer_countDirType;

typedef enum
{
    TIMER_SIZE_16_BIT = 0,
    TIMER_SIZE_32_BIT,
    TIMER_SIZE_INVALID
} timer_sizeType;

typedef enum
{
    TIMER_A,
    TIMER_B,
    TIMER_AB,
    TIMER_CHANNEL_INVALID
} timer_subType;

typedef enum
{
    TIMER_US,
    TIMER_MS,
    TIMER_SEC
} timer_timeUnitType;

/* Interrupt related fields */
typedef enum
{
    TIMER_INTERRUPT_DISABLE,
    TIMER_INTERRUPT_ENABLE
} timer_interruptType;


typedef enum
{
    TIMER0A_IRQ = 19,
    TIMER0B_IRQ = 20,

    TIMER1A_IRQ = 21,
    TIMER1B_IRQ = 22,

    TIMER2A_IRQ = 23,
    TIMER2B_IRQ = 24,

    TIMER3A_IRQ = 35,
    TIMER3B_IRQ = 36,

    TIMER4A_IRQ = 70,
    TIMER4B_IRQ = 71,

    TIMER5A_IRQ = 92,
    TIMER5B_IRQ = 93

} timer_irqNumberType;

/******************************************* Data Types *******************************************/

typedef struct 
{
    timer_numberType number;
    timer_subType channel;

    timer_modeType mode;
    timer_countDirType direction;
    timer_sizeType size;

    timer_timeUnitType unit;
    uint8_t prescaler;
    /*interrupt related fields*/
    timer_interruptType interrupt;

} timer_configType;

typedef struct
{
    /* 0x000 : Configuration */
    volatile uint32_t GPTMCFG;

    /* 0x004 : Timer A Mode */
    volatile uint32_t GPTMTAMR;

    /* 0x008 : Timer B Mode */
    volatile uint32_t GPTMTBMR;

    /* 0x00C : Control */
    volatile uint32_t GPTMCTL;

    /* 0x010 : Synchronize */
    volatile uint32_t GPTMSYNC;

    /* 0x014 : Reserved */
    volatile uint32_t RESERVED0;

    /* 0x018 : Interrupt Mask */
    volatile uint32_t GPTMIMR;

    /* 0x01C : Raw Interrupt Status */
    volatile const uint32_t GPTMRIS;

    /* 0x020 : Masked Interrupt Status */
    volatile const uint32_t GPTMMIS;

    /* 0x024 : Interrupt Clear */
    volatile uint32_t GPTMICR;

    /* 0x028 : Timer A Interval Load */
    volatile uint32_t GPTMTAILR;

    /* 0x02C : Timer B Interval Load */
    volatile uint32_t GPTMTBILR;

    /* 0x030 : Timer A Match */
    volatile uint32_t GPTMTAMATCHR;

    /* 0x034 : Timer B Match */
    volatile uint32_t GPTMTBMATCHR;

    /* 0x038 : Timer A Prescale */
    volatile uint32_t GPTMTAPR;

    /* 0x03C : Timer B Prescale */
    volatile uint32_t GPTMTBPR;

    /* 0x040 : Timer A Prescale Match */
    volatile uint32_t GPTMTAPMR;

    /* 0x044 : Timer B Prescale Match */
    volatile uint32_t GPTMTBPMR;

    /* 0x048 : Timer A Register */
    volatile const uint32_t GPTMTAR;

    /* 0x04C : Timer B Register */
    volatile const uint32_t GPTMTBR;

    /* 0x050 : Timer A Value */
    volatile uint32_t GPTMTAV;

    /* 0x054 : Timer B Value */
    volatile uint32_t GPTMTBV;

    /* 0x058 : RTC Predivide */
    volatile const uint32_t GPTMRTCPD;

    /* 0x05C : Timer A Prescale Snapshot */
    volatile const uint32_t GPTMTAPS;

    /* 0x060 : Timer B Prescale Snapshot */
    volatile const uint32_t GPTMTBPS;

    /* 0x064 : Timer A Prescale Value */
    volatile const uint32_t GPTMTAPV;

    /* 0x068 : Timer B Prescale Value */
    volatile const uint32_t GPTMTBPV;

    /* 0x06C - 0xFBF : Reserved */
    volatile uint32_t RESERVED1[981];

    /* 0xFC0 : Peripheral Properties */
    volatile const uint32_t GPTMPP;

} timer_registerType;

/*************************************** Function Prototypes **************************************/

timer_errorType timer_init(timer_configType * config);
timer_errorType timer_blockingDelay(timer_configType *config, uint32_t delay);
timer_errorType timer_setCallback(timer_numberType timer, void (*callback)(void));
timer_errorType timer_interruptHandler(timer_numberType timer, timer_subType channel);
timer_errorType timer_start(timer_configType *config, uint32_t delay);
#endif
