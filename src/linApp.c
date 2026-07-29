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
static linApp_commandType receivedCommand = LIN_APP_COMMAND_INVALID;

static volatile uint8_t ackTimeout = 0U;

static timer_configType ackTimer =
{
    .number     = TIMER_4,
    .channel    = TIMER_AB,
    .mode       = TIMER_MODE_ONE_SHOT,
    .direction  = TIMER_COUNT_DOWN,
    .size       = TIMER_SIZE_32_BIT,
    .prescaler  = 0U,
    .unit       = TIMER_MS,
    .interrupt  = TIMER_INTERRUPT_ENABLE
};



gpio_configType blueLeds ={
        .port = GPIO_PORT_F,
        .pin = GPIO_PIN_2,
        .mode = GPIO_MODE_OUTPUT
    };

    gpio_configType greenLed ={
        .port = GPIO_PORT_F,
        .pin = GPIO_PIN_3,
        .mode = GPIO_MODE_OUTPUT
    };

    gpio_configType redLed ={
        .port = GPIO_PORT_F,
        .pin = GPIO_PIN_1,
        .mode = GPIO_MODE_OUTPUT
    };


/************************************* Function Implementations ***********************************/

static void linApp_printPrompt(void)
{
    uart_sendString(&uartConfig, "<S> ");
}

static void linApp_ackTimeoutCallback(void)
{
    ackTimeout = 1U;
}

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

static linApp_commandType linApp_decodeFrame(const lin_pduType *frame)
{
    if (frame == NULL)
    {
        return LIN_APP_COMMAND_INVALID;
    }
    
        
        // gpio_digitalToggle(&greenLed);

    if (
        (frame->data[0] == 'R') &&
        (frame->data[1] == 'E') &&
        (frame->data[2] == 'D'))
        {
            //  gpio_digitalToggle(&blueLeds);
        return LIN_APP_COMMAND_RED;
    }

    if (
        (frame->data[0] == 'G') &&
        (frame->data[1] == 'R') &&
        (frame->data[2] == 'E') &&
        (frame->data[3] == 'E') &&
        (frame->data[4] == 'N'))
    {
        return LIN_APP_COMMAND_GREEN;
    }

    if (
        (frame->data[0] == 'B') &&
        (frame->data[1] == 'L') &&
        (frame->data[2] == 'U') &&
        (frame->data[3] == 'E'))
    {
        return LIN_APP_COMMAND_BLUE;
    }

    if (
        (frame->data[0] == 'O') &&
        (frame->data[1] == 'F') &&
        (frame->data[2] == 'F'))
    {
        return LIN_APP_COMMAND_OFF;
    }

    return LIN_APP_COMMAND_INVALID;
}

static void linApp_prepareResponse(linApp_responseType response)
{
    txFrame.identifier = 0x10U;
    txFrame.checksumMod = LIN_CHECKSUM_ENHANCED;

    txFrame.data[0] = 0U;
    txFrame.data[1] = 0U;
    txFrame.data[2] = 0U;
    txFrame.data[3] = 0U;
    txFrame.data[4] = 0U;
    txFrame.dataLength = 5U;

    if (response == LIN_APP_RESPONSE_ACK)
    {
        txFrame.data[0] = 'A';
        txFrame.data[1] = 'C';
        txFrame.data[2] = 'K';
    }
    else
    {
        txFrame.data[0] = 'E';
        txFrame.data[1] = 'R';
        txFrame.data[2] = 'R';
    }
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
    txFrame.identifier = 0x10U;
    txFrame.checksumMod = LIN_CHECKSUM_ENHANCED;

    /* Zero all 5 slots first — guarantees deterministic padding */
    txFrame.data[0] = 0U;
    txFrame.data[1] = 0U;
    txFrame.data[2] = 0U;
    txFrame.data[3] = 0U;
    txFrame.data[4] = 0U;
    txFrame.dataLength = 5U;   /* ALWAYS 5, no matter which command */

    switch (currentCommand)
    {
    case LIN_APP_COMMAND_RED:
        txFrame.data[0] = 'R';
        txFrame.data[1] = 'E';
        txFrame.data[2] = 'D';
        break;

    case LIN_APP_COMMAND_GREEN:
        txFrame.data[0] = 'G';
        txFrame.data[1] = 'R';
        txFrame.data[2] = 'E';
        txFrame.data[3] = 'E';
        txFrame.data[4] = 'N';
        break;

    case LIN_APP_COMMAND_BLUE:
        txFrame.data[0] = 'B';
        txFrame.data[1] = 'L';
        txFrame.data[2] = 'U';
        txFrame.data[3] = 'E';
        break;

    case LIN_APP_COMMAND_OFF:
        txFrame.data[0] = 'O';
        txFrame.data[1] = 'F';
        txFrame.data[2] = 'F';
        break;

    default:
        break;
    }
}

void linApp_init(void)
{

    lin_errorType linStatus;
    uart_errorType uartStatus;

    linStatus = lin_init(19200U);
    if (linStatus != LIN_OK)
    {
        while (1)
        {
        }
    }

    gpio_init(&blueLeds);
    gpio_init(&greenLed);
    gpio_init(&redLed);
    
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

#if (LIN_NODE_TYPE == LIN_MASTER_NODE)

    if (timer_init(&ackTimer) != TIMER_SUCCESS)
    {
        while (1)
        {
        }
    }

    if (timer_setCallback(TIMER_4, linApp_ackTimeoutCallback) != TIMER_SUCCESS)
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

#elif (LIN_NODE_TYPE == LIN_SLAVE_NODE)

    lin_slaveConfigType slave =
    {
        .identifier = 0x10U
    };

    linStatus = lin_slaveInit(&slave);

    if (linStatus != LIN_OK)
    {
        while (1)
        {
        }
    }
    // gpio_digitalToggle(&blueLeds);

#endif
    appState = LIN_APP_IDLE;

#if (LIN_NODE_TYPE == LIN_MASTER_NODE)
    linApp_printPrompt();
#endif



}


void linApp_stateMachine(void)
{
    switch (appState)
    {
    case LIN_APP_INIT:
        linApp_init();
        break;

    case LIN_APP_IDLE:
         
#if (LIN_NODE_TYPE == LIN_MASTER_NODE)

        if (commandReady)
        {
            currentCommand = linApp_parseCommand(commandBuffer);

            commandReady = 0;

            if (currentCommand != LIN_APP_COMMAND_INVALID)
            {
                appState = LIN_APP_SEND_COMMAND;   // this is not working 
            }
            else
            {
                uart_sendString(&uartConfig, "Invalid Command\r\n");
                linApp_printPrompt();
            }
        }
        gpio_digitalToggle(&greenLed);

#elif (LIN_NODE_TYPE == LIN_SLAVE_NODE)

        //  gpio_digitalToggle(&greenLed);
         lin_errorType ret = lin_receiveFrame(&rxFrame);

         if ( ret == LIN_OK)
         {
            //  gpio_digitalToggle(&greenLed);
            appState = LIN_APP_PROCESS_COMMAND;

        }

#endif
        break;

    case LIN_APP_SEND_COMMAND:

        linApp_prepareFrame();
    // linApp_printPrompt();

        if (lin_sendFrame(&txFrame) == LIN_OK)
        {   
            /* Clear the Commnad so it doesnt send again */
            currentCommand = LIN_APP_COMMAND_INVALID;
            
            ackTimeout = 0U;

            timer_start(&ackTimer, 3000U);

            appState = LIN_APP_WAIT_RESPONSE;
        }

        break;

    case LIN_APP_WAIT_RESPONSE:
        
        if (lin_receiveFrame(&rxFrame) == LIN_OK)
        {
            /* Decode ACK / ERR */

            timer_stop(&ackTimer);

            ackTimeout = 0U;

            linApp_printPrompt();

            appState = LIN_APP_IDLE;
        }
        else if (ackTimeout)
        {
            timer_stop(&ackTimer);

            ackTimeout = 0U;

            uart_sendString(&uartConfig,
                            "ACK NOT RECEIVED!\r\n");

            linApp_printPrompt();

            appState = LIN_APP_IDLE;
        }
        break;

    case LIN_APP_PROCESS_COMMAND:

        receivedCommand = linApp_decodeFrame(&rxFrame);
        // gpio_digitalToggle(&redLed);
        switch (receivedCommand)
        {
        case LIN_APP_COMMAND_RED:

            gpio_digitalToggle(&redLed);

            linApp_prepareResponse(LIN_APP_RESPONSE_ACK);

            break;

        case LIN_APP_COMMAND_GREEN:

            gpio_digitalToggle(&greenLed);

            linApp_prepareResponse(LIN_APP_RESPONSE_ACK);

            break;

        case LIN_APP_COMMAND_BLUE:

            gpio_digitalToggle(&blueLeds);

            linApp_prepareResponse(LIN_APP_RESPONSE_ACK);

            break;

        case LIN_APP_COMMAND_OFF:

            gpio_digitalWrite(&redLed, 0);
            gpio_digitalWrite(&blueLeds, 0);
            gpio_digitalWrite(&greenLed, 0);
            // gpio_digitalToggle(&blueLeds);
            linApp_prepareResponse(LIN_APP_RESPONSE_ACK);

            break;

        default:

            linApp_prepareResponse(LIN_APP_RESPONSE_ERR);

            break;
        }

        appState = LIN_APP_SEND_RESPONSE;
        break;

    case LIN_APP_SEND_RESPONSE:

        if (lin_sendFrame(&txFrame) == LIN_OK)
        {
            appState = LIN_APP_IDLE;
        }

        break;

    default:
        appState = LIN_APP_INIT;
        break;
    }
}

