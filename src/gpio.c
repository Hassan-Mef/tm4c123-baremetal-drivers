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

/* GPIO Port Table */
static gpio_registersType * const gpioPorts[] =
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
 * @return gpio_errorType
 */
gpio_errorType gpio_init(gpio_configType * const config)
{
    gpio_registersType *gpio = NULL;

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
            gpio->AFSEL |= (1U << config->pin);
            break;
        case GPIO_MODE_ANALOG:
            return GPIO_ERROR_UNDEFINED_MODE;
            break;
        default:
            return GPIO_ERROR_INVALID_MODE;
    }   

    /* Enable digital function */
    gpio->DEN |= (1U << config->pin);

    return GPIO_SUCCESS;
}

/**
 * @brief gpio_digitalWrite : Writes a digital value to a GPIO pin.
 *
 * @param config : Pointer to GPIO configuration.
 * @param pinState : Logic state to write (0 or 1).
 *
 * @return gpio_errorType
 */
gpio_errorType gpio_digitalWrite(gpio_configType * const config, uint8_t pinState)
{
    gpio_registersType *gpio = NULL;

    /* Validate configuration parameters */


    if (config == NULL)
    {
        return GPIO_ERROR_NULL_POINTER;
    }

    if (pinState > GPIO_STATE_HIGH)
    {
        return GPIO_ERROR_INVALID_STATE;
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

    /* Get pointer to selected GPIO peripheral */
    gpio = gpioPorts[config->port];

    /* Set or clear the selected GPIO pin */
    switch(pinState)  
    {
        case GPIO_STATE_HIGH:
            gpio->DATA |= (1U << config->pin);
            break;
        case GPIO_STATE_LOW:
            gpio->DATA &= ~(1U << config->pin);
            break;
        default:
            return GPIO_ERROR_INVALID_STATE;
    }
    
    return GPIO_SUCCESS ;

}

/**
 * @brief gpio_digitalToggle : Toggles the state of a GPIO pin.
 *
 * @param config : Pointer to GPIO configuration.
 *
 * @return gpio_errorType
 */
gpio_errorType gpio_digitalToggle(gpio_configType * const config)
{

    gpio_registersType *gpio = NULL;

    /* Validate configuration parameters */

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

    gpio->DATA ^=(1U << config->pin);

    return GPIO_SUCCESS;

}

/**
 * @brief gpio_digitalRead : Reads the state of a GPIO pin.
 *
 * @param config : Pointer to GPIO configuration.
 * @param pinState : Pointer to store the read logic state (0 or 1).
 *
 * @return gpio_errorType
 */
gpio_errorType gpio_digitalRead(gpio_configType * const config, uint8_t * pinState)
{
    gpio_registersType *gpio = NULL;

    /* Validate configuration parameters */

    if (config == NULL)
    {
        return GPIO_ERROR_NULL_POINTER;
    }

    if (pinState == NULL)
    {
        return GPIO_ERROR_INVALID_STATE;
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

    /* Get pointer to selected GPIO peripheral */
    gpio = gpioPorts[config->port];

    /* Read the state of the selected GPIO pin */
    if(gpio->DATA & (1U << config->pin))
    {
        *pinState = GPIO_STATE_HIGH;
    }
    else
    {
        *pinState = GPIO_STATE_LOW;
    }

    return GPIO_SUCCESS;
}
