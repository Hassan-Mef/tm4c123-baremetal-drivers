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

gpio_configType blueLed =
{
    .port = GPIO_PORT_F,
    .pin  = GPIO_PIN_2,
    .mode = GPIO_MODE_OUTPUT
};

gpio_configType greenLed =
{
    .port = GPIO_PORT_F,
    .pin  = GPIO_PIN_3,
    .mode = GPIO_MODE_OUTPUT
};

gpio_configType button =
{
    .port = GPIO_PORT_F,
    .pin  = GPIO_PIN_4,
    .mode = GPIO_MODE_INPUT
};


static void redCallback(void)
{
    gpio_digitalToggle(&redLed);
}

static void blueCallback(void)
{
    gpio_digitalToggle(&blueLed);
}

static void greenCallback(void)
{
    gpio_digitalToggle(&greenLed);
}
/************************************** Main Implementation ***************************************/

int main(void)
{
    /* Initialize GPIO pins for LEDs */
    gpio_init(&redLed);
    gpio_init(&blueLed);
    gpio_init(&greenLed);
    gpio_init(&button);

    /* Set up timer configuration */

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

    timer_configType timer1 =
    {
        .number     = TIMER_1,
        .channel    = TIMER_AB,
        .mode       = TIMER_MODE_PERIODIC,
        .direction  = TIMER_COUNT_DOWN,
        .size       = TIMER_SIZE_32_BIT,
        .prescaler  = 0U,
        .unit       = TIMER_MS,
        .interrupt  = TIMER_INTERRUPT_ENABLE
    };

    timer_configType timer3 =
    {
        .number     = TIMER_3,
        .channel    = TIMER_AB,
        .mode       = TIMER_MODE_ONE_SHOT,
        .direction  = TIMER_COUNT_DOWN,
        .size       = TIMER_SIZE_32_BIT,
        .prescaler  = 0U,
        .unit       = TIMER_MS,
        .interrupt  = TIMER_INTERRUPT_ENABLE
    };
    timer_configType timer4 =
    {
        .number     = TIMER_4,
        .channel    = TIMER_AB,
        .mode       = TIMER_MODE_ONE_SHOT,
        .direction  = TIMER_COUNT_DOWN,
        .size       = TIMER_SIZE_32_BIT,
        .prescaler  = 0U,
        .unit       = TIMER_MS,
        .interrupt  = TIMER_INTERRUPT_DISABLE
    };

    /* Initialize timers and set callbacks */
    timer_init(&timer0);
    timer_init(&timer1);
    timer_init(&timer3);
    timer_init(&timer4);

    timer_setCallback(TIMER_0, redCallback);
    timer_setCallback(TIMER_1, blueCallback);
    timer_setCallback(TIMER_3, greenCallback);

    timer_start(&timer0, 1000U);

    uint8_t blockingDemoDone = 0U;
    uint8_t oneShotDemoDone  = 0U;
    uint8_t buttonState;
    uint8_t previousState = 1U;

    while (1)
    {
        gpio_digitalRead(&button, &buttonState);

        if ((previousState == 1U) && (buttonState == 0U))
        {
            if (blockingDemoDone == 0U)
            {
                gpio_digitalToggle(&blueLed);
                timer_blockingDelay(&timer4, 2000U);
                gpio_digitalToggle(&blueLed);

                blockingDemoDone = 1U;
            }
            else if (oneShotDemoDone == 0U)
            {
                timer_start(&timer3, 4000U);

                oneShotDemoDone = 1U;
            }

            /* Wait for button release */
            do
            {
                gpio_digitalRead(&button, &buttonState);
            }
            while (buttonState == 0U);

            /* Simple debounce delay */
            timer_blockingDelay(&timer4, 20U);
        }

        previousState = buttonState;
    }
}