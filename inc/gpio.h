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
#define GPIOA ((gpio_registersType *)GPIO_PORT_A_BASE)
#define GPIOB ((gpio_registersType *)GPIO_PORT_B_BASE)
#define GPIOC ((gpio_registersType *)GPIO_PORT_C_BASE)
#define GPIOD ((gpio_registersType *)GPIO_PORT_D_BASE)
#define GPIOE ((gpio_registersType *)GPIO_PORT_E_BASE)
#define GPIOF ((gpio_registersType *)GPIO_PORT_F_BASE)


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
} gpio_portType;

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
} gpio_pinType;


typedef enum
{
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE,
    GPIO_MODE_ANALOG,
    GPIO_MODE_INVALID
} gpio_modeType;


typedef enum
{
    GPIO_SUCCESS = 0,
    GPIO_ERROR_NULL_POINTER,
    GPIO_ERROR_INVALID_PORT,
    GPIO_ERROR_INVALID_PIN,
    GPIO_ERROR_INVALID_MODE,
    GPIO_ERROR_INVALID_STATE,
    GPIO_ERROR_UNDEFINED_MODE
} gpio_errorType;


/******************************************* Data Types *******************************************/

typedef struct 
{
    gpio_portType port;
    gpio_pinType pin;
    gpio_modeType mode;
} gpio_configType;

typedef struct
{
    /* 0x000 - 0x3FC */
    volatile uint32_t RESERVED0[255];
    
    /* 0x3FC  cuz pin */
    volatile uint32_t DATA;

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
    volatile uint32_t RESERVED1[55];

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
    volatile uint32_t RESERVED2[678];

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

} gpio_registersType;

/*************************************** Function Prototypes **************************************/

gpio_errorType gpio_init(gpio_configType * const config);
gpio_errorType gpio_digitalWrite(gpio_configType * const config, uint8_t pinState);
gpio_errorType gpio_digitalToggle(gpio_configType * const config);
gpio_errorType gpio_digitalRead(gpio_configType * const config, uint8_t *pinState);


#endif



