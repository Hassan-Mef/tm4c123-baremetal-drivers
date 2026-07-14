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

   gpio_configType redLed =
{
    .port = GPIO_PORT_F,
    .pin  = GPIO_PIN_1,
    .mode = GPIO_MODE_OUTPUT
};


static void ledCallback(void)
{
    GPIOF->DATA ^= (1U << GPIO_PIN_1);
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

gpio_init(&redLed);



timer_configType timer0 =
{
    .number     = TIMER_0,
    .channel    = TIMER_AB,
    .mode       = TIMER_MODE_PERIODIC,
    .direction  = TIMER_COUNT_DOWN,
    .size       = TIMER_SIZE_32_BIT,
    .prescaler  = 8U,
    .unit       = TIMER_MS,
    .interrupt  = TIMER_INTERRUPT_ENABLE
};

timer_init(&timer0);

timer_setCallback(TIMER_0, ledCallback);


timer_start(&timer0, 1000U);

while (1)
{
    // GPIOF->DATA ^= (1U << GPIO_PIN_1);
    //ledCallback();
    //  gpio_digitalToggle(&redLed);
    
}
}