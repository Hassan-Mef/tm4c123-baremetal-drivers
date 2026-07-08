/***************************************************************************************************
 * FILENAME    : main.c
 * DESCRIPTION : GPIO Driver Test
 *
 * AUTHOR      : Hassan
 *
***************************************************************************************************/

/*************************************** Header Inclusion *****************************************/

#include "gpio.h"

/************************************* Private Functions ******************************************/

static void delay(void)
{
    for (volatile uint32_t i = 0; i < 500000U; i++)
    {
    }
}

/************************************** Main Implementation ***************************************/

int main(void)
{
    GpioConfigType redLed =
    {
        .port = GPIO_PORT_F,
        .pin = GPIO_PIN_1,
        .mode = GPIO_MODE_OUTPUT
    };

    GpioConfigType blueLed =
    {
        .port = GPIO_PORT_F,
        .pin = GPIO_PIN_2,
        .mode = GPIO_MODE_OUTPUT
    };

    /* Initialize LEDs */
    gpio_init(&redLed);
    gpio_init(&blueLed);

    while (1)
    {
        gpio_digitalToggle(&redLed);
        delay();

        gpio_digitalToggle(&blueLed);
        delay();
    }
}