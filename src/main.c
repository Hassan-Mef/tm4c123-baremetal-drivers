/***************************************************************************************************
 * FILENAME    : main.c
 * DESCRIPTION : UART Driver Validation
 *
 * AUTHOR      : Hassan
 ***************************************************************************************************/

 /*************************************** Header Inclusion *****************************************/

#include "lin.h"
#include "gpio.h"
#include "linApp.h"

/**************************************** Configuration *******************************************/

const clock_configType clockConfig =
{
    .source = CLOCK_SOURCE_PLL,
    .frequency = CLOCK_FREQ_80MHZ
};

timer_configType timer0 =
{
    .number = TIMER_4,
    .channel = TIMER_AB,
    .mode = TIMER_MODE_ONE_SHOT,
    .direction = TIMER_COUNT_DOWN,
    .size = TIMER_SIZE_32_BIT,
    .unit = TIMER_MS,
    .prescaler = 0U,
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

static void ledCallback(void)
{
    gpio_digitalToggle(&redLed);
}


/************************************** Callback ****************************************************/


/************************************** Main Implementation ***************************************/
int main(void)
{
    lin_errorType status;

    clock_init(&clockConfig);

    timer_init(&timer0);

//     status = lin_init(19200U);
//     gpio_init(&blueLed);
//     gpio_init(&redLed);

//     if (status != LIN_OK)
//     {
//         while (1)
//         {
//         }
//     }

// #if (LIN_NODE_TYPE == LIN_MASTER_NODE)

//     lin_pduType frame =
//     {
//         .identifier = 0x12U,
//         .dataLength = LIN_DATA_2_BYTE,
//         .data = {0xAAU, 0xBBU},
//         .checksumMod = LIN_CHECKSUM_ENHANCED
//     };

//     while (1)
//     {
//         status = lin_sendFrame(&frame);

//         if (status != LIN_OK)
//         {
//             /* Transmission failed */
//             while (1)
//             {
//             }
//         }

//          timer_blockingDelay(&timer0, 10U);
//     }

// #elif (LIN_NODE_TYPE == LIN_SLAVE_NODE)


//     lin_slaveConfigType slave =
//     {
//         .identifier = 0x10U
//     };

//     lin_pduType receivedFrame;

//     status = lin_slaveInit(&slave);

//     gpio_digitalToggle(&blueLed);

//     if (status != LIN_OK)
//     {
//         while (1)
//         {
//         }
//     }

//     while (1)
//     {
//         status = lin_receiveFrame(&receivedFrame);

//         if (status == LIN_OK)
//         {
//             gpio_digitalToggle(&redLed);
//         }
//     }

// #endif

linApp_init();

// timer_setCallback(TIMER_0, ledCallback);

// timer_registerType *timer = TIMER0;

// timer_start(&timer0, 3000U);

while (1)
{


     linApp_stateMachine();

    //  gpio_digitalToggle(&redLed);
    //  linApp_sendTestFrame();

    //  status = gpio_digitalToggle(&redLed);
    // timer_blockingDelay(&timer0, U);
}

}


