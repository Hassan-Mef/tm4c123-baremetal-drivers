/***************************************************************************************************
* FILENAME : uart.H
* DESCRIPTION :  contains function prototypes 
*
* NOTES :
*
* AUTHOR : Hassan
***************************************************************************************************/
#ifndef __UART_H_
#define __UART_H_

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


#define SYSCTL_RCGCUART_OFFSET         (0x618U)

#define SYSCTL_RCGCUART \
    (*(volatile uint32_t *)(SYSCTL_BASE + SYSCTL_RCGCUART_OFFSET))


#define SYSTEM_CLOCK_HZ                (80000000U)



/* Timer Base Addresses */
#define UART0_BASE                     0x4000C000U
#define UART1_BASE                     0x4000D000U
#define UART2_BASE                     0x4000E000U
#define UART3_BASE                     0x4000F000U
#define UART4_BASE                     0x40010000U
#define UART5_BASE                     0x40011000U
#define UART6_BASE                     0x40012000U
#define UART7_BASE                     0x40013000U


/* Timers */
#define UART0                          ((uart_registerType *)UART0_BASE)
#define UART1                          ((uart_registerType *)UART1_BASE)
#define UART2                          ((uart_registerType *)UART2_BASE)
#define UART3                          ((uart_registerType *)UART3_BASE)
#define UART4                          ((uart_registerType *)UART4_BASE)
#define UART5                          ((uart_registerType *)UART5_BASE)
#define UART6                          ((uart_registerType *)UART6_BASE)
#define UART7                          ((uart_registerType *)UART7_BASE)


/* UARTFR Register */
#define UARTFR_TXFF_BIT                (5U)
#define UARTFR_RXFE_BIT                (4U)
#define UARTFR_BUSY_BIT                (3U)

/* UARTCTL Register */
#define UARTCTL_UARTEN_BIT             (0U)
#define UARTCTL_TXE_BIT                (8U)
#define UARTCTL_RXE_BIT                (9U)

/* UARTLCRH Register */
#define UARTLCRH_FEN_BIT               (4U)
#define UARTLCRH_WLEN_BIT              (5U)

/* UART Interrupts */
#define UARTIM_RXIM_BIT                (4U)
#define UARTMIS_RXMIS_BIT              (4U)
#define UARTICR_RXIC_BIT               (4U)


#define UART_WORD_LENGTH_8             (0x3U)
#define UART_FIFO_ENABLE               (1U)

#define GPIO_PCTL_PIN_OFFSET           (4U)
#define GPIO_PCTL_UART          (0x1U)



#define NVIC_BASE                      0xE000E000U  
#define NVIC_EN0                       (*(volatile uint32_t *)(NVIC_BASE + 0x100U))
#define NVIC_EN1                       (*(volatile uint32_t *)(NVIC_BASE + 0x104U))
#define NVIC_EN2                       (*(volatile uint32_t *)(NVIC_BASE + 0x108U))
#define NVIC_EN3                       (*(volatile uint32_t *)(NVIC_BASE + 0x10CU))

#define NVIC_ENABLE_BASE               ((volatile uint32_t *)0xE000E100U)


/****************************************** Enumerations ******************************************/

typedef enum
{
    UART_0 = 0,
    UART_1,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    UART_7,
    UART_INVALID

} uart_numberType;

typedef enum
{
    UART_SUCCESS = 32,
    UART_INVALID_UART,
    UART_INVALID_CONFIG,
    UART_INVALID_BAUDRATE,
    UART_NULL_POINTER,
    UART_BUSY,
    UART_TIMEOUT,
    UART_INVALID_DATA,
    UART_ERROR

} uart_errorType;

typedef enum
{
    UART_BAUD_RATE_9600 = 9600,
    UART_BAUD_RATE_19200 = 19200,
    UART_BAUD_RATE_38400 = 38400,   
    UART_BUAD_RAE_115200 = 115200,
    UART_BAUD_RATE_INVALID
} uart_baudRateType;


/******************************************* Data Types *******************************************/

typedef struct
{
    uart_numberType number;

    uint32_t baudRate;

    uint8_t interruptEnable;

} uart_configType;
typedef struct
{
    /* 0x000 : Data */
    volatile uint32_t UARTDR;

    /* 0x004 : Receive Status / Error Clear */
    volatile uint32_t UARTRSR_ECR;

    /* 0x008 - 0x014 : Reserved */
    volatile uint32_t RESERVED0[4];

    /* 0x018 : Flag */
    volatile const uint32_t UARTFR;

    /* 0x01C : Reserved */
    volatile uint32_t RESERVED1;

    /* 0x020 : IrDA Low-Power */
    volatile uint32_t UARTILPR;

    /* 0x024 : Integer Baud Rate Divisor */
    volatile uint32_t UARTIBRD;

    /* 0x028 : Fractional Baud Rate Divisor */
    volatile uint32_t UARTFBRD;

    /* 0x02C : Line Control */
    volatile uint32_t UARTLCRH;

    /* 0x030 : Control */
    volatile uint32_t UARTCTL;

    /* 0x034 : Interrupt FIFO Level Select */
    volatile uint32_t UARTIFLS;

    /* 0x038 : Interrupt Mask */
    volatile uint32_t UARTIM;

    /* 0x03C : Raw Interrupt Status */
    volatile const uint32_t UARTRIS;

    /* 0x040 : Masked Interrupt Status */
    volatile const uint32_t UARTMIS;

    /* 0x044 : Interrupt Clear */
    volatile uint32_t UARTICR;

    /* 0x048 : DMA Control */
    volatile uint32_t UARTDMACTL;

    /* 0x04C - 0x0A0 : Reserved */
    volatile uint32_t RESERVED2[22];

    /* 0x0A4 : 9-Bit Self Address */
    volatile uint32_t UART9BITADDR;

    /* 0x0A8 : 9-Bit Self Address Mask */
    volatile uint32_t UART9BITAMASK;

    /* 0x0AC - 0xFBF : Reserved */
    volatile uint32_t RESERVED3[773];

    /* 0xFC0 : Peripheral Properties */
    volatile const uint32_t UARTPP;

    /* 0xFC4 : Reserved */
    volatile uint32_t RESERVED4;

    /* 0xFC8 : Clock Configuration */
    volatile uint32_t UARTCC;

} uart_registerType;

/*************************************** Function Prototypes **************************************/
uart_errorType uart_init(uart_configType *config);
uart_errorType uart_sendCharacter(uart_configType *config, char data);
uart_errorType uart_sendString(uart_configType *config, char *data);
uart_errorType uart_receiveCharacter(uart_configType *config, char *data);


#endif
