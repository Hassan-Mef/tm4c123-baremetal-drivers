/**************************************************************************************************
* FILENAME : gpio.c
* DESCRIPTION : Contains GPIO driver function definitions.
*
* NOTES :
*
* AUTHOR : Hassan
***************************************************************************************************/

/*************************************** Header Inclusion*****************************************/
#include "gpio.h"

/********************************************* Globals ********************************************/

/* GPIO Port Lookup Table */
static GpioRegistersType * const gpioPorts[] =
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF
};

/************************************* Function Implementations***********************************/

/**
 * @brief Gpio_init : Initializes a GPIO pin.
 *
 * @param config : Pointer to GPIO configuration.
 *
 * @return GpioErrorType
 */
GpioErrorType gpio_init(GpioConfigType * const config)
{
    GpioRegistersType *gpio = NULL;

    if (config == NULL)
    {
        return GPIO_ERROR_NULL_POINTER;
    }

    if (config->port >= GPIO_PORT_INVALID)
    {
        return GPIO_ERROR_INVALID_PORT;
    }

    if (config->pin >= GPIO_PIN_INVALID)
    {
        return GPIO_ERROR_INVALID_PIN;
    }

    if (config->mode >= GPIO_MODE_INVALID)
    {
        return GPIO_ERROR_INVALID_MODE;
    }

    /* Clock Enable for port */
    SYSCTL_RCGCGPIO |= (1U << config->port);



    /* Get pointer to selected GPIO peripheral */
    gpio = gpioPorts[config->port];

    /* Configure pin direction */  
    switch(config->mode)
    {
        case GPIO_MODE_INPUT:
            gpio->DIR &= ~(1U << config->pin); 
            break;
        case GPIO_MODE_OUTPUT:
            gpio->DIR |= (1U << config->pin);
            break;
        case GPIO_MODE_ALTERNATE:
            break;
        case GPIO_MODE_ANALOG:
            break;
        default:
            return GPIO_ERROR_INVALID_MODE;
    }   

    /* Enable digital function */
    gpio->DEN |= (1U << config->pin);

    return GPIO_SUCCESS;
}

GpioErrorType Gpio_digitalWrite(GpioConfigType * const config, uint8_t pinState)
{
    GpioRegistersType *gpio = NULL;

    /* Validation */

    if (config == NULL)
    {
        return GPIO_ERROR_NULL_POINTER;
    }

    if (pinState == NULL)
    {
        return GPIO_ERROR_NULL_POINTER;
    }

    if (config->port >= GPIO_PORT_INVALID)
    {
        return GPIO_ERROR_INVALID_PORT;
    }

    if (config->pin >= GPIO_PIN_INVALID)
    {
        return GPIO_ERROR_INVALID_PIN;
    }

    if (config->mode >= GPIO_MODE_INVALID)
    {
        return GPIO_ERROR_INVALID_MODE;
    }

    gpio = gpioPorts[config->port];

    if(pinState)
    {
        gpio->DATA[255] |= (1U << config->pin);
    }
    else 
    {
        gpio->DATA[255] &= ~(1U << config->pin);
    }
    return GPIO_SUCCESS ;

}

GpioErrorType Gpio_digitalToggle(GpioConfigType * const config)
{

    GpioRegistersType *gpio = NULL;

    /* Validation */

    if (config == NULL)
    {
        return GPIO_ERROR_NULL_POINTER;
    }

    if (config->port >= GPIO_PORT_INVALID)
    {
        return GPIO_ERROR_INVALID_PORT;
    }

    if (config->pin >= GPIO_PIN_INVALID)
    {
        return GPIO_ERROR_INVALID_PIN;
    }

    if (config->mode >= GPIO_MODE_INVALID)
    {
        return GPIO_ERROR_INVALID_MODE;
    }

    gpio = gpioPorts[config->port];

    gpio->DATA[255] ^=(1U << config->pin);

    return GPIO_SUCCESS;

}

GpioErrorType Gpio_digitalRead(GpioConfigType * const config, uint8_t * pinState)
{
    GpioRegistersType *gpio = NULL;

    /* Validation */

    if (config == NULL)
    {
        return GPIO_ERROR_NULL_POINTER;
    }

    if (config->port >= GPIO_PORT_INVALID)
    {
        return GPIO_ERROR_INVALID_PORT;
    }

    if (config->pin >= GPIO_PIN_INVALID)
    {
        return GPIO_ERROR_INVALID_PIN;
    }

    if (config->mode >= GPIO_MODE_INVALID)
    {
        return GPIO_ERROR_INVALID_MODE;
    }

    gpio = gpioPorts[config->port];

    if(gpio->DATA[255] & (1U << config->pin))
    {
        *pinState = 1;
    }
    else
    {
        *pinState = 0;
    }

    return GPIO_SUCCESS;
}
