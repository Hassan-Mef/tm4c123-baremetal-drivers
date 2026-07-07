/***************************************************************************************************
 * FILENAME    : main.c
 * DESCRIPTION : Main application
 *
 * AUTHOR      : Hassan
 *
 **************************************************************************************************/

/*************************************** Header Inclusion *****************************************/
#include <stdint.h>

/********************************************* Globals ********************************************/

#define SYSCTL_RCGCGPIO_R   (*((volatile uint32_t *)0x400FE608))
#define GPIO_PORTF_DIR_R    (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_DEN_R    (*((volatile uint32_t *)0x4002551C))
#define GPIO_PORTF_DATA_R   (*((volatile uint32_t *)0x400253FC))

#define GPIO_PORTF_CLOCK    (1U << 5)
#define RED_LED             (1U << 1)

/********************************************* Externs ********************************************/


/************************************* Private Functions ******************************************/

static void delay(void)
{
    for (volatile uint32_t i = 0; i < 500000U; i++)
    {
    }
}


/************************************** Main Implementation ***************************************/

/**
 * @brief Main application.
 *
 * @return int
 */
int main(void)
{
    /* Enable clock for GPIO Port F */
    SYSCTL_RCGCGPIO_R |= GPIO_PORTF_CLOCK;

    /* Wait for clock to stabilize */
    delay();

    /* Configure PF1 as output */
    GPIO_PORTF_DIR_R |= RED_LED;

    /* Enable digital functionality */
    GPIO_PORTF_DEN_R |= RED_LED;

    while (1)
    {
        /* Toggle Red LED */
        GPIO_PORTF_DATA_R ^= RED_LED;

        delay();
    }
}