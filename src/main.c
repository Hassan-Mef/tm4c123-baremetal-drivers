/***************************************************************************************************
 * FILENAME    : main.c
 * DESCRIPTION : Main application
 *
 * AUTHOR      : Hassan
 * 
 **************************************************************************************************/

/*************************************** Header Inclusion *****************************************/
#include <stdint.h>
/********************************************* Globals********************************************/

#define SYSCTL_RCGCGPIO_R   (*((volatile uint32_t *)0x400FE608))
#define GPIO_PORTF_DIR_R    (*((volatile uint32_t *)0x40025400))
#define GPIO_PORTF_DEN_R    (*((volatile uint32_t *)0x4002551C))
#define GPIO_PORTF_DATA_R   (*((volatile uint32_t *)0x400253FC))

/********************************************* Externs ********************************************/


/************************************** Main Implementation ***************************************/

/**
* @brief main : For testing whether our machine uses little Endian or Big Endian 
*
* @return int
*/
int main(void)
{
    /* Enable clock for GPIO Port F */
    SYSCTL_RCGCGPIO_R |= (1U << 5);

    /* Small delay to allow the clock to stabilize */
    for (volatile int i = 0; i < 1000; i++)
    {
    }

    /* Configure PF1 (Red LED) as output */
    GPIO_PORTF_DIR_R |= (1U << 1);

    /* Enable digital function on PF1 */
    GPIO_PORTF_DEN_R |= (1U << 1);

    /* Turn on Red LED */
    GPIO_PORTF_DATA_R |= (1U << 1);

    while (1)
    {
    }
}