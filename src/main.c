/***************************************************************************************************
 * FILENAME    : main.c
 * DESCRIPTION : UART Driver Validation
 *
 * AUTHOR      : Hassan
 ***************************************************************************************************/

 /*************************************** Header Inclusion *****************************************/

#include "lin.h"
#include "timer.h"
#include "gpio.h"

/**************************************** UART Configuration ****************************************/
/* Select LIN Node */
#define LIN_MASTER_NODE    (1U)
#define LIN_SLAVE_NODE     (2U)

#define LIN_NODE_TYPE      LIN_SLAVE_NODE
/* Change to LIN_SLAVE_NODE to test slave */

const clock_configType clockConfig =
{
    .source = CLOCK_SOURCE_PLL,
    .frequency = CLOCK_FREQ_80MHZ
};

timer_configType timer0 =
{
    .number = TIMER_0,
    .channel = TIMER_A,
    .mode = TIMER_MODE_PERIODIC,
    .direction = TIMER_COUNT_DOWN,
    .size = TIMER_SIZE_16_BIT,
    .unit = TIMER_MS,
    .prescaler = 79U,
    .interrupt = TIMER_INTERRUPT_DISABLE
};

gpio_configType blueLed ={
        .port = GPIO_PORT_F,
        .pin = GPIO_PIN_2,
        .mode = GPIO_MODE_OUTPUT
    };

    gpio_configType redLed ={
        .port = GPIO_PORT_F,
        .pin = GPIO_PIN_1,
        .mode = GPIO_MODE_OUTPUT
    };

    void uart1callBack()
    {
        
    }
/************************************** Callback ****************************************************/


/************************************** Main Implementation ***************************************/
int main(void)
{
    lin_errorType status;

    clock_init(&clockConfig);

    timer_init(&timer0);

    status = lin_init(19200U);
    gpio_init(&blueLed);
    gpio_init(&redLed);

    if (status != LIN_OK)
    {
        while (1)
        {
        }
    }

#if (LIN_NODE_TYPE == LIN_MASTER_NODE)

    lin_pduType frame =
    {
        .identifier = 0x12U,
        .dataLength = LIN_DATA_2_BYTE,
        .data = {0xAAU, 0xBBU},
        .checksumMod = LIN_CHECKSUM_CLASSIC
    };

    while (1)
    {
        status = lin_sendFrame(&frame);

        if (status != LIN_OK)
        {
            /* Transmission failed */
            while (1)
            {
            }
        }

         timer_blockingDelay(&timer0, 10U);
    }

#elif (LIN_NODE_TYPE == LIN_SLAVE_NODE)


    lin_slaveConfigType slave =
    {
        .identifier = 0x12U
    };

    lin_pduType receivedFrame;

    status = lin_slaveInit(&slave);

    gpio_digitalToggle(&blueLed);

    if (status != LIN_OK)
    {
        while (1)
        {
        }
    }

    while (1)
    {
        status = lin_receiveFrame(&receivedFrame);

        if (status == LIN_OK)
        {
            gpio_digitalToggle(&redLed);
        }
    }

#endif
}