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
#include "clock.h"

/********************************************* Macros *********************************************/
#define SYSCTL_RCGCUART_OFFSET         (0x618U)

#define SYSCTL_RCGCUART \
    (*(volatile uint32_t *)(SYSCTL_BASE + SYSCTL_RCGCUART_OFFSET))


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
#define UARTLCRH_BRK_BIT               (0U)
#define UARTLCRH_PEN_BIT               (1U)
#define UARTLCRH_EPS_BIT               (2U)
#define UARTLCRH_STP2_BIT              (3U)
#define UARTLCRH_FEN_BIT               (4U)
#define UARTLCRH_WLEN_BIT              (5U)
#define UARTLCRH_SPS_BIT               (7U)

/* UART Interrupts */
#define UARTIM_RXIM_BIT                (4U)
#define UARTMIS_RXMIS_BIT              (4U)
#define UARTICR_RXIC_BIT               (4U)


#define UART_DATA_LENGTH_8             (0x3U)
#define UART_FIFO_ENABLE               (1U)

#define GPIO_PCTL_PIN_OFFSET           (4U)
#define GPIO_PCTL_UART                 (0x1U)


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
    UART_WORD_LENGTH_5 = 0,
    UART_WORD_LENGTH_6,
    UART_WORD_LENGTH_7,
    UART_WORD_LENGTH_8

} uart_wordLengthType;

typedef enum
{
    UART_PARITY_NONE,
    UART_PARITY_EVEN,
    UART_PARITY_ODD

} uart_parityType;

typedef enum
{
    UART_STOP_BITS_1,
    UART_STOP_BITS_2

} uart_stopBitsType;

typedef enum
{
    UART_BUAD_RATE_9600 = 9600,
    UART_BUAD_RATE_19200 = 19200,
    UART_BUAD_RATE_38400 = 38400,   
    UART_BUAD_RATE_115200 = 115200,
    UART_BAUD_RATE_1000000 = 1000000,
    UART_BUAD_RATE_INVALID
} uart_baudRateType;

typedef enum
{
    UART0_IRQ = 5,
    UART1_IRQ = 6,
    UART2_IRQ = 33,
    UART3_IRQ = 59,
    UART4_IRQ = 60,
    UART5_IRQ = 61,
    UART6_IRQ = 62,
    UART7_IRQ = 63

} uart_irqNumberType;




/******************************************* Data Types *******************************************/

typedef struct
{
    uart_numberType number;
    uart_baudRateType baudRate;
    uart_wordLengthType wordLength;
    uart_parityType parity;
    uart_stopBitsType stopBits;
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
uart_errorType uart_init(uart_configType* config);
uart_errorType uart_deInit(uart_configType* config);
uart_errorType uart_changeBaudRate(uart_configType *config,uint32_t baudRate);
uart_errorType uart_sendCharacter(uart_configType* config, char data);
uart_errorType uart_sendString(uart_configType* config, char* data);
uart_errorType uart_receiveCharacter(uart_configType* config, char* data);
uart_errorType uart_receiveNB(uart_configType* config, char* data);
uart_errorType uart_getReceivedCharacter(uart_configType* config ,char* data);
uart_errorType uart_interruptHandler(uart_numberType uartNumber);
uart_errorType uart_setCallback(uart_configType* config, void (*callback)(void));

#endif
