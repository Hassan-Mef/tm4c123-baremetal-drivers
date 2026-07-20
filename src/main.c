/***************************************************************************************************
 * FILENAME    : main.c
 * DESCRIPTION : UART Driver Validation
 *
 * AUTHOR      : Hassan
 ***************************************************************************************************/

 /*************************************** Header Inclusion *****************************************/

#include "uart.h"
#include "gpio.h"

/**************************************** UART Configuration ****************************************/
uart_configType uart0 =
{
    .number = UART_0,
    .baudRate = UART_BUAD_RAE_115200,
    .interruptEnable = 0
};

uart_configType uart1 =
{
    .number = UART_1,
    .baudRate = UART_BUAD_RAE_115200,
    .interruptEnable = 1
};


gpio_configType blueLed =
{
    .port = GPIO_PORT_F,
    .pin  = GPIO_PIN_2,
    .mode = GPIO_MODE_OUTPUT
};

/************************************** Callback ****************************************************/

void upperCaseCallback(void)
{
    char ch;

    if (uart_getReceivedCharacter(UART_0, &ch) == UART_SUCCESS)
    {
        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= ('a' - 'A');
        }

        uart_sendCharacter(&uart0, ch);
    }
}

void uart1Callback(void )
{
    gpio_digitalToggle(&blueLed);
}

/************************************* Private Functions ******************************************/


void blockingEchoTest(void)
{
    char ch;
    
    uart0.interruptEnable = 0;
    
    uart_init(&uart0);
    
    uart_sendString(&uart0, "\r\n====================================\r\n");
    uart_sendString(&uart0, "UART BLOCKING ECHO TEST\r\n");
    uart_sendString(&uart0, "====================================\r\n");
    uart_sendString(&uart0, "Type any character...\r\n");

    while (1)
    {
        uart_receiveCharacter(&uart0, &ch);
        uart_sendCharacter(&uart0, ch);
    }
}

void interruptEchoTest(void)
{
    uart0.interruptEnable = 1;
    
    uart_init(&uart0);
    
    uart_sendString(&uart0, "\r\n====================================\r\n");
    uart_sendString(&uart0, "UART INTERRUPT ECHO TEST\r\n");
    uart_sendString(&uart0, "====================================\r\n");
    uart_sendString(&uart0, "Default callback will echo characters.\r\n");
    
    while (1)
    {
    }
}

void callbackOverrideTest(void)
{
    uart0.interruptEnable = 1;
    
    uart_init(&uart0);
    
    uart_setCallback(UART_0, upperCaseCallback);
    
    uart_sendString(&uart0, "\r\n====================================\r\n");
    uart_sendString(&uart0, "UART CALLBACK OVERRIDE TEST\r\n");
    uart_sendString(&uart0, "====================================\r\n");
    uart_sendString(&uart0, "Lowercase letters become uppercase.\r\n");
    
    while (1)
    {
    }
}

void uart0ToUart1Test(void)
{
    char ch;

    uart0.interruptEnable = 0;
    uart1.interruptEnable = 0;

    uart_init(&uart0);
    uart_init(&uart1);

    uart_sendString(&uart0, "\r\nUART0 -> UART1 Test\r\n");
    uart_sendString(&uart0, "Type here. Characters will appear on UART1.\r\n");

    while (1)
    {
        uart_receiveCharacter(&uart0, &ch);
        uart_sendCharacter(&uart1, ch);
    }
}

void uart1ToUart0Test(void)
{
    char ch;

    uart0.interruptEnable = 0;
    uart1.interruptEnable = 0;

    uart_init(&uart0);
    uart_init(&uart1);

    uart_sendString(&uart0, "\r\nUART1 -> UART0 Test\r\n");
    uart_sendString(&uart0, "Type on the USB-TTL terminal.\r\n");

    while (1)
    {
        uart_receiveCharacter(&uart1, &ch);

    if (ch == '\r')
    {
        uart_sendString(&uart0, "\r\n");
    }
    else if (ch == '\n')
    {
        uart_sendString(&uart0, "\r\n");
    }
    else
    {
        uart_sendCharacter(&uart0, ch);
    }
    }
}

/************************************** Main Implementation ***************************************/

int main(void)
{
    

    //blockingEchoTest();

    // interruptEchoTest();

    // callbackOverrideTest();

    //uart0ToUart1Test();

    uart1ToUart0Test();
    while (1)
    {
    }
}