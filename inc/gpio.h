/***************************************************************************************************
* FILENAME : GPIO.H
* DESCRIPTION :  contains function prototypes 
*
* NOTES :
*
* AUTHOR : Hassan
***************************************************************************************************/

#ifndef __GPIO_H_
#define __GPIO_H_

/*************************************** Header Inclusion *****************************************/

#include <stddef.h>
#include <stdint.h>

/********************************************* Macros *********************************************/

#define SYSCTL_BASE    0x400FE000U
#define SYSCTL_RCGCGPIO_OFFSET    0x608U

#define SYSCTL_RCGCGPIO \
    (*(uint32_t *)(SYSCTL_BASE + SYSCTL_RCGCGPIO_OFFSET))


/* APB  Base Address*/
#define GPIO_PORT_A_BASE 0x40004000U
#define GPIO_PORT_B_BASE 0x40005000U
#define GPIO_PORT_C_BASE 0x40006000U
#define GPIO_PORT_D_BASE 0x40007000U
#define GPIO_PORT_E_BASE 0x40024000U
#define GPIO_PORT_F_BASE 0x40025000U


/* GPIO */
#define GPIOA ((GpioRegistersType *)GPIO_PORT_A_BASE)
#define GPIOB ((GpioRegistersType *)GPIO_PORT_B_BASE)
#define GPIOC ((GpioRegistersType *)GPIO_PORT_C_BASE)
#define GPIOD ((GpioRegistersType *)GPIO_PORT_D_BASE)
#define GPIOE ((GpioRegistersType *)GPIO_PORT_E_BASE)
#define GPIOF ((GpioRegistersType *)GPIO_PORT_F_BASE)


/* GPIO State Macro*/
#define GPIO_STATE_LOW  0
#define GPIO_STATE_HIGH 1



/****************************************** Enumerations ******************************************/

typedef enum 
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_F,
    GPIO_PORT_INVALID
} GpioPortType;

typedef enum
{
    GPIO_PIN_0 = 0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15,
    GPIO_PIN_INVALID
} GpioPinType;


typedef enum
{
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE,
    GPIO_MODE_ANALOG,
    GPIO_MODE_INVALID
} GpioModeType;


typedef enum
{
    GPIO_SUCCESS = 0,
    GPIO_ERROR_NULL_POINTER,
    GPIO_ERROR_INVALID_PORT,
    GPIO_ERROR_INVALID_PIN,
    GPIO_ERROR_INVALID_MODE,
    GPIO_ERROR_INVALID_STATE
} GpioErrorType;


/******************************************* Data Types *******************************************/

typedef struct 
{
    GpioPortType port;
    GpioPinType pin;
    GpioModeType mode;
} GpioConfigType;

typedef struct
{
    /* 0x000 - 0x3FC */
    volatile uint32_t DATA[256];

    /* 0x400 */
    volatile uint32_t DIR;

    /* 0x404 */
    volatile uint32_t IS;

    /* 0x408 */
    volatile uint32_t IBE;

    /* 0x40C */
    volatile uint32_t IEV;

    /* 0x410 */
    volatile uint32_t IM;

    /* 0x414 */
    volatile const uint32_t RIS;

    /* 0x418 */
    volatile const uint32_t MIS;

    /* 0x41C */
    volatile uint32_t ICR;

    /* 0x420 */
    volatile uint32_t AFSEL;

    /* Reserved: (0x500 - 0x424) / 4 = 55 words */
    volatile uint32_t RESERVED0[55];

    /* 0x500 */
    volatile uint32_t DR2R;

    /* 0x504 */
    volatile uint32_t DR4R;

    /* 0x508 */
    volatile uint32_t DR8R;

    /* 0x50C */
    volatile uint32_t ODR;

    /* 0x510 */
    volatile uint32_t PUR;

    /* 0x514 */
    volatile uint32_t PDR;

    /* 0x518 */
    volatile uint32_t SLR;

    /* 0x51C */
    volatile uint32_t DEN;

    /* 0x520 */
    volatile uint32_t LOCK;

    /* 0x524 */
    volatile uint32_t CR;

    /* 0x528 */
    volatile uint32_t AMSEL;

    /* 0x52C */
    volatile uint32_t PCTL;

    /* 0x530 */
    volatile uint32_t ADCCTL;

    /* 0x534 */
    volatile uint32_t DMACTL;

    /* Reserved bytes: 0x538 - 0xFCF */
    volatile uint32_t RESERVED1[678];

    /* 0xFD0 */
    volatile const uint32_t PeriphID4;

    /* 0xFD4 */
    volatile const uint32_t PeriphID5;

    /* 0xFD8 */
    volatile const uint32_t PeriphID6;

    /* 0xFDC */
    volatile const uint32_t PeriphID7;

    /* 0xFE0 */
    volatile const uint32_t PeriphID0;

    /* 0xFE4 */
    volatile const uint32_t PeriphID1;

    /* 0xFE8 */
    volatile const uint32_t PeriphID2;

    /* 0xFEC */
    volatile const uint32_t PeriphID3;

    /* 0xFF0 */
    volatile const uint32_t PCellID0;

    /* 0xFF4 */
    volatile const uint32_t PCellID1;

    /* 0xFF8 */
    volatile const uint32_t PCellID2;

    /* 0xFFC */
    volatile const uint32_t PCellID3;

} GpioRegistersType;

/*************************************** Function Prototypes **************************************/

GpioErrorType gpio_init(GpioConfigType * const config);
GpioErrorType gpio_digitalWrite(GpioConfigType * const config, uint8_t pinState);
GpioErrorType gpio_digitalToggle(GpioConfigType * const config);
GpioErrorType gpio_digitalRead(GpioConfigType * const config, uint8_t *pinState);


#endif



