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
static lin_slaveConfigType slaveConfig;

/************************************* Static Declarations ****************************************/

/**
 * @brief lin_generatePid : Generates the Protected Identifier (PID).
 *
 * Generates the LIN Protected Identifier by calculating the
 * parity bits and appending them to the 6-bit identifier.
 *
 * @param identifier : LIN frame identifier.
 *
 * @return uint8_t : Protected Identifier (PID).
 */
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

/**
 * @brief lin_sendBreak : Generates the LIN Break field.
 *
 * Generates the LIN Break by temporarily reducing the UART baud
 * rate and transmitting 0x00. Since a UART frame containing 0x00
 * produces nine dominant bits (start bit + eight data bits), the
 * temporary baud rate is selected so that the dominant low period
 * equals thirteen nominal LIN bit times.
 *
 * Temporary Baud = (9 × LIN Baud) / 13
 *
 * @return void
 */
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
//     for (volatile uint32_t i = 0; i < 300; i++)
// {
//     __asm("NOP");
// }
}

/************************************* Function Implementations ***********************************/

/**
 * @brief lin_init : Initializes the LIN driver.
 *
 * Configures UART1 with the specified LIN baud rate and initializes
 * the internal LIN driver state.
 *
 * @param baudRate : LIN communication baud rate.
 *
 * @return lin_errorType
 */
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
    /* Register LIN RX callback */
    uartStatus = uart_setCallback(&uartConfig, lin_copyByte);
    

    if (uartStatus != UART_SUCCESS)
    {
        return LIN_ERROR_UART;
    }

    linReceiveIndex = 0U;
    linBaudRate = baudRate;

    lin_clearReceiveBuffer();

    return LIN_OK;
}

/**
 * @brief lin_sendFrame : Transmits a complete LIN frame.
 *
 * Generates the LIN Break field, transmits the Sync byte,
 * Protected Identifier (PID), data bytes, and checksum
 * according to the supplied LIN Protocol Data Unit.
 *
 * @param pdu : Pointer to the LIN Protocol Data Unit to transmit.
 *
 * @return lin_errorType
 */
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

  
lin_errorType lin_verifyChecksum(lin_checksumModType checksumModel)
{
    lin_pduType frame;

    if (checksumModel >= LIN_CHECKSUM_INVALID)
    {
        return LIN_ERROR_INVALID_CHECKSUM_MODEL;
    }

    if (linReceiveIndex < 4U)
    {
        return LIN_ERROR_FRAME;
    }

    if (linReceiveIndex > LIN_RX_BUFFER_SIZE)
    {
        return LIN_ERROR_BUFFER_OVERFLOW;
    }

    frame.identifier = linReceiveBuffer[1] & 0x3F;
    frame.dataLength = LIN_DATA_2_BYTE;
    frame.checksumMod = checksumModel;

    for (uint8_t index = 0U; index < frame.dataLength; index++)
    {
        frame.data[index] = linReceiveBuffer[index + 2U];
    }


    uint8_t calculatedChecksum;
    uint8_t receivedChecksum;

    calculatedChecksum = lin_calculateChecksum(&frame);

    receivedChecksum = linReceiveBuffer[linReceiveIndex - 1U];

    if (calculatedChecksum == receivedChecksum)
    {
        return LIN_OK;
    }

    return LIN_ERROR_INVALID_CHECKSUM;

}

/**
 * @brief lin_copyReceiveBuffer : Copies the LIN receive buffer.
 *
 * Copies the internal LIN receive buffer into the user-provided
 * buffer.
 *
 * @param buffer : Destination buffer.
 *
 * @return lin_errorType
 */
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

/**
 * @brief lin_clearReceiveBuffer : Clears the LIN receive buffer.
 *
 * Clears all received bytes and resets the receive index.
 *
 * @return lin_errorType
 */
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

/**
 * @brief lin_copyByte : Stores one received UART byte.
 *
 * Reads one byte from the UART receive register and stores it
 * into the internal LIN receive buffer.
 *
 * This function is intended to be called from the UART receive ISR.
 *
 * @return void
 */
void lin_copyByte(void)
{
    char receivedByte;

    if (uart_getReceivedCharacter(&uartConfig, &receivedByte) != UART_SUCCESS)
    {
        return;
    }

    if (linReceiveIndex < LIN_RX_BUFFER_SIZE)
    {
        linReceiveBuffer[linReceiveIndex] = (uint8_t)receivedByte;
        linReceiveIndex++;
    }
    else
    {
        lin_clearReceiveBuffer();
    }
}

lin_errorType lin_slaveInit(const lin_slaveConfigType *config)
{
    if (config == NULL)
    {
        return LIN_ERROR_NULL_POINTER;
    }

    if (config->identifier > LIN_MAX_IDENTIFIER)
    {
        return LIN_ERROR_INVALID_IDENTIFIER;
    }

    slaveConfig = *config;

    return LIN_OK;
}

lin_errorType lin_receiveFrame(lin_pduType *pdu)
{
    lin_errorType status;
    uint8_t index;

    if (pdu == NULL)
    {
        return LIN_ERROR_NULL_POINTER;
    }

    /* Minimum frame:
     * Sync + PID + 1 Data Byte + Checksum
     */
    if (linReceiveIndex < 5U)
    {
        return LIN_ERROR_FRAME;
    }

    if (linReceiveBuffer[0] != LIN_SYNC_BYTE)
    {
        lin_clearReceiveBuffer();
        return LIN_ERROR_SYNC;
    }

    /* Extract Identifier from PID */
    pdu->identifier = linReceiveBuffer[1U] & LIN_MAX_IDENTIFIER;    

    /* Check whether this frame belongs to this slave */
    if (pdu->identifier != slaveConfig.identifier)
    {
        lin_clearReceiveBuffer();
        return LIN_ERROR_FRAME;
    }

    /* Calculate received data length */
    pdu->dataLength = LIN_DATA_2_BYTE;
    /* Copy received data bytes */
    for (index = 0U; index < pdu->dataLength; index++)
    {
        pdu->data[index] = linReceiveBuffer[index + 2U];
    }

    pdu->checksumMod = LIN_CHECKSUM_CLASSIC;

    /* Verify checksum */
    status = lin_verifyChecksum(pdu->checksumMod);

    if (status != LIN_OK)
    {
        lin_clearReceiveBuffer();
        return status;
    }

    lin_clearReceiveBuffer();


    return LIN_OK;
}
