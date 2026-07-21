/***************************************************************************************************
 * FILENAME    : lin.c
 * DESCRIPTION : LIN Driver Source File
 *
 * AUTHOR      : Hassan
 *
 ***************************************************************************************************/

/*************************************** Header Inclusion *****************************************/

#include "lin.h"
#include "uart.h"

/********************************************* Macros *********************************************/

/********************************************* Globals ********************************************/

static uint8_t linReceiveBuffer[LIN_RX_BUFFER_SIZE];
static uint8_t linReceiveIndex = 0U;
static uint32_t linBaudRate = 0U;
static uart_configType uartConfig;

/********************************************* Externs ********************************************/

/************************************* Static Declarations ****************************************/
static uint8_t lin_generatePid(uint8_t identifier)
{
    uint8_t p0;
    uint8_t p1;

    identifier &= 0x3FU;

    /* even parity */
    p0 = ((identifier >> 0U) ^
          (identifier >> 1U) ^
          (identifier >> 2U) ^
          (identifier >> 4U)) &
         0x01U;

    /* odd parity */
    p1 = ~((identifier >> 1U) ^
           (identifier >> 3U) ^
           (identifier >> 4U) ^
           (identifier >> 5U)) &
         0x01U;

    return (identifier | (p0 << 6U) | (p1 << 7U));
}

/**
 * @brief lin_calculateChecksum : Calculates the LIN frame checksum.
 *
 * Calculates either the Classic or Enhanced checksum based on the
 * checksum model specified in the LIN Protocol Data Unit.
 *
 * @param pdu : Pointer to the LIN Protocol Data Unit.
 *
 * @return uint8_t : Calculated checksum byte.
 */
static uint8_t lin_calculateChecksum(const lin_pduType *pdu)
{
    uint16_t checksum = 0U;
    uint8_t index;

    /* Enhanced checksum includes the PID */
    if (pdu->checksumMod == LIN_CHECKSUM_ENHANCED)
    {
        checksum = lin_generatePid(pdu->identifier);
    }

    /* Add all data bytes */
    for (index = 0U; index < pdu->dataLength; index++)
    {
        checksum += pdu->data[index];

        /* Carry-around addition */
        if (checksum > 0xFFU)
        {
            checksum = (checksum & 0xFFU) + 1U;
        }
    }

    return (uint8_t)(~checksum);
}

static void lin_sendBreak(void)
{

    while (UART1->UARTFR & (1U << UARTFR_BUSY_BIT))
    {
    }

    uart_changeBaudRate(&uartConfig, (9U * linBaudRate) / 13U);

    uart_sendCharacter(&uartConfig, 0x00);
  
    while (UART1->UARTFR & (1U << UARTFR_BUSY_BIT))
    {
    }

    uart_changeBaudRate(&uartConfig, linBaudRate);
}
/************************************* Function Implementations ***********************************/
lin_errorType lin_init(uint32_t baudRate)
{
    uart_errorType uartStatus;

    /* Configure UART1 for LIN communication */

    uartConfig.number = UART_1;
    uartConfig.baudRate = (uart_baudRateType)baudRate;
    uartConfig.wordLength = UART_WORD_LENGTH_8;
    uartConfig.parity = UART_PARITY_NONE;
    uartConfig.stopBits = UART_STOP_BITS_1;
    uartConfig.interruptEnable = true;

    uartStatus = uart_init(&uartConfig);

    if (uartStatus != UART_SUCCESS)
    {
        return LIN_ERROR_UART;
    }

    linReceiveIndex = 0U;
    linBaudRate = baudRate;

    lin_clearReceiveBuffer();

    return LIN_OK;
}

lin_errorType lin_sendFrame(const lin_pduType *pdu)
{
    uint8_t pid;
    uint8_t checksum;
    uint8_t index;
    uart_errorType uartStatus;

    if (pdu == NULL)
    {
        return LIN_ERROR_UNKNOWN;
    }

    lin_sendBreak();

    uartStatus = uart_sendCharacter(&uartConfig, (char)LIN_SYNC_BYTE);
    if (uartStatus != UART_SUCCESS)
    {
        return LIN_ERROR_UART;
    }

    pid = lin_generatePid(pdu->identifier);

    uartStatus = uart_sendCharacter(&uartConfig, (char)pid);
    if (uartStatus != UART_SUCCESS)
    {
        return LIN_ERROR_UART;
    }

    for (index = 0U; index < pdu->dataLength; index++)
    {
        uartStatus = uart_sendCharacter(&uartConfig,
                                        (char)pdu->data[index]);

        if (uartStatus != UART_SUCCESS)
        {
            return LIN_ERROR_UART;
        }
    }

    checksum = lin_calculateChecksum(pdu);

    uartStatus = uart_sendCharacter(&uartConfig, (char)checksum);
    if (uartStatus != UART_SUCCESS)
    {
        return LIN_ERROR_UART;
    }

    return LIN_OK;
}

lin_errorType lin_copyReceiveBuffer(uint8_t *buffer)
{
    uint8_t index;

    if (buffer == NULL)
    {
        return LIN_ERROR_NULL_POINTER;
    }

    for (index = 0U; index < LIN_RX_BUFFER_SIZE; index++)
    {
        buffer[index] = linReceiveBuffer[index];
    }

    return LIN_OK;
}

lin_errorType lin_clearReceiveBuffer(void)
{
    uint8_t index;

    for (index = 0U; index < LIN_RX_BUFFER_SIZE; index++)
    {
        linReceiveBuffer[index] = 0U;
    }

    linReceiveIndex = 0U;

    return LIN_OK;
}

void lin_copyByte(void)
{
    char receivedByte;

    if (uart_receiveCharacter(&uartConfig, &receivedByte) != UART_SUCCESS)
    {
        return;
    }

    if (linReceiveIndex < LIN_RX_BUFFER_SIZE)
    {
        linReceiveBuffer[linReceiveIndex] = (uint8_t)receivedByte;
        linReceiveIndex++;
    }
}