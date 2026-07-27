/***************************************************************************************************
 * FILENAME    : linApp.c
 * DESCRIPTION : LIN Application Source File
 *
 * AUTHOR      : Hassan
 *
 ***************************************************************************************************/

/*************************************** Header Inclusion*****************************************/

#include "linApp.h"
#include "lin.h"

/********************************************* Globals ********************************************/

static linApp_stateType appState = LIN_APP_INIT;

static lin_pduType txFrame;
static lin_pduType rxFrame;

static linApp_commandType currentCommand = LIN_APP_COMMAND_INVALID;

static uart_configType uartConfig;


static char commandBuffer[16];
static uint8_t commandIndex = 0;

static volatile int commandReady = 0;

/************************************* Function Implementations ***********************************/

static linApp_commandType linApp_parseCommand(const char *command)
{
    if (command == NULL)
    {
        return LIN_APP_COMMAND_INVALID;
    }

    if ((command[0] == 'R') &&
        (command[1] == 'E') &&
        (command[2] == 'D') &&
        (command[3] == '\0'))
    {
        return LIN_APP_COMMAND_RED;
    }

    if ((command[0] == 'G') &&
        (command[1] == 'R') &&
        (command[2] == 'E') &&
        (command[3] == 'E') &&
        (command[4] == 'N') &&
        (command[5] == '\0'))
    {
        return LIN_APP_COMMAND_GREEN;
    }

    if ((command[0] == 'B') &&
        (command[1] == 'L') &&
        (command[2] == 'U') &&
        (command[3] == 'E') &&
        (command[4] == '\0'))
    {
        return LIN_APP_COMMAND_BLUE;
    }

    if ((command[0] == 'O') &&
        (command[1] == 'F') &&
        (command[2] == 'F') &&
        (command[3] == '\0'))
    {
        return LIN_APP_COMMAND_OFF;
    }

    return LIN_APP_COMMAND_INVALID;
}

static void linApp_uartCallback(void)
{
    char ch;

    if (uart_getReceivedCharacter(&uartConfig, &ch) != UART_SUCCESS)
    {
        return;
    }

    if (ch == '\r')
     {
         commandBuffer[commandIndex] = '\0';
         commandReady = 1;
         commandIndex = 0U;
     }
     else if (ch == '\n')
     {
         /* Ignore LF */
     }
     else if (commandIndex < (sizeof(commandBuffer) - 1U))
     {
         commandBuffer[commandIndex++] = ch;
     }
}

static void linApp_prepareFrame(void)
{
    txFrame.identifier = 0x10U;          /* Command PID */
    txFrame.checksumMod = LIN_CHECKSUM_CLASSIC;

    switch (currentCommand)
    {
        case LIN_APP_COMMAND_RED:
            txFrame.data[0] = 'R';
            txFrame.data[1] = 'E';
            txFrame.data[2] = 'D';
            txFrame.dataLength = 3U;
            break;

        case LIN_APP_COMMAND_GREEN:
            txFrame.data[0] = 'G';
            txFrame.data[1] = 'R';
            txFrame.data[2] = 'E';
            txFrame.data[3] = 'E';
            txFrame.data[4] = 'N';
            txFrame.dataLength = 5U;
            break;

        case LIN_APP_COMMAND_BLUE:
            txFrame.data[0] = 'B';
            txFrame.data[1] = 'L';
            txFrame.data[2] = 'U';
            txFrame.data[3] = 'E';
            txFrame.dataLength = 4U;
            break;

        case LIN_APP_COMMAND_OFF:
            txFrame.data[0] = 'O';
            txFrame.data[1] = 'F';
            txFrame.data[2] = 'F';
            txFrame.dataLength = 3U;
            break;

        default:
            txFrame.dataLength = 0U;
            break;
    }
}

void linApp_init(void)
{
    /* Future:
     * - Initialize UART for PuTTY
     * - Initialize LIN
     * - Initialize LEDs
     */
    lin_errorType linStatus;
    uart_errorType uartStatus;

    
    linStatus = lin_init(19200U); 

    uartConfig.number = UART_0;
    uartConfig.baudRate = UART_BUAD_RATE_115200;
    uartConfig.wordLength = UART_WORD_LENGTH_8;
    uartConfig.parity = UART_PARITY_NONE;
    uartConfig.stopBits = UART_STOP_BITS_1;
    uartConfig.interruptEnable = true;

    uartStatus = uart_init(&uartConfig);
    
    if (uartStatus != UART_SUCCESS)
    {
        while (1)
        {
        }
    }

    uartStatus = uart_setCallback(&uartConfig, linApp_uartCallback);

     if (uartStatus != UART_SUCCESS)
    {
        while (1)
        {
        }
    }
  

    if (linStatus != LIN_OK)
    {
        while (1)
        {
        }
    }

    appState = LIN_APP_IDLE;

}

void linApp_stateMachine(void)
{
    switch(appState)
    {
        case LIN_APP_INIT:
            linApp_init();
            break;

        case LIN_APP_IDLE:
            if (commandReady)
            {
                currentCommand = linApp_parseCommand(commandBuffer);
            
                commandReady = 0;
            
                if (currentCommand != LIN_APP_COMMAND_INVALID)
                {
                    appState = LIN_APP_SEND_COMMAND;
                }
                else
                {
                    uart_sendString(&uartConfig, "Invalid Command\r\n");
                }
            }
            break;

        case LIN_APP_SEND_COMMAND:

            linApp_prepareFrame();

            if (lin_sendFrame(&txFrame) == LIN_OK)
            {
                /* Clear the Commnad so it doesnt send again */
                currentCommand = LIN_APP_COMMAND_INVALID;
                appState = LIN_APP_WAIT_RESPONSE;
            }

            break;

        case LIN_APP_WAIT_RESPONSE:
                appState = LIN_APP_IDLE;
            break;

        case LIN_APP_PROCESS_COMMAND:

            break;

        default:
            appState = LIN_APP_INIT;
            break;
    }
}

void linApp_setCommand(linApp_commandType command)
{
    currentCommand = command;

}