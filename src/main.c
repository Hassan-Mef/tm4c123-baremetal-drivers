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
    gpio_digitalToggle(&redLed);
}
/************************************** Main Implementation ***************************************/

int main(void)
{


gpio_init(&redLed);



timer_configType timer0 =
{
    .number     = TIMER_0,
    .channel    = TIMER_AB,
    .mode       = TIMER_MODE_PERIODIC,
    .direction  = TIMER_COUNT_DOWN,
    .size       = TIMER_SIZE_32_BIT,
    .prescaler  = 0U,
    .unit       = TIMER_MS,
    .interrupt  = TIMER_INTERRUPT_ENABLE
};

timer_init(&timer0);

timer_setCallback(TIMER_0, ledCallback);


timer_start(&timer0, 1000U);

while (1)
{
    // status = gpio_digitalToggle(&redLed);


    // status = timer_blockingDelay(&timer0, 5000U); // 50 seconds delay
}
}