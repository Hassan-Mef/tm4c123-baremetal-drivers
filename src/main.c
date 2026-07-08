/***************************************************************************************************
 * FILENAME    : main.c
 * DESCRIPTION : GPIO Driver Test
 *
 * AUTHOR      : Hassan
 *
***************************************************************************************************/

/*************************************** Header Inclusion *****************************************/

#include "gpio.h"
#include "timer.h"

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
    // gpio_configType redLed =
    // {
    //     .port = GPIO_PORT_F,
    //     .pin = GPIO_PIN_1,
    //     .mode = GPIO_MODE_OUTPUT
    // };

    // gpio_configType blueLed =
    // {
    //     .port = GPIO_PORT_F,
    //     .pin = GPIO_PIN_2,
    //     .mode = GPIO_MODE_OUTPUT
    // };

    // /* Initialize LEDs */
    // gpio_init(&redLed);
    // gpio_init(&blueLed);

    // while (1)
    // {
    //     gpio_digitalToggle(&redLed);
    //     delay();

    //     gpio_digitalToggle(&blueLed);
    //     delay();
    // }


    timer_configType timer0 =
    {
        .number     = TIMER_0,
        .mode       = TIMER_MODE_PERIODIC,
        .direction  = TIMER_COUNT_DOWN,
        .size       = TIMER_SIZE_16_BIT,
        .prescaler  = 79U
    };

    timer_init(&timer0);

    while (1)
    {
    }
}