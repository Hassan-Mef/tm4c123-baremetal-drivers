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


/************************************** Callback ****************************************************/


/************************************** Main Implementation ***************************************/
int main(void)
{
    lin_errorType status;

    clock_init(&clockConfig);


    linApp_init();


    while (1)
    {
        linApp_stateMachine();
    }

}


