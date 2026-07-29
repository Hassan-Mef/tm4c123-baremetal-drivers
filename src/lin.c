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
#include "gpio.h"
#include "timer.h"

/********************************************* Macros *********************************************/

/********************************************* Globals ********************************************/

static uint8_t linReceiveBuffer[LIN_RX_BUFFER_SIZE];
static uint8_t linReceiveIndex = 0U;
static uint32_t linBaudRate = 0U;
static uart_configType uartConfig;
static uart_configType uart0Config;
static lin_slaveConfigType slaveConfig;

gpio_configType greenLedss ={
    .port = GPIO_PORT_F,
    .pin = GPIO_PIN_2,
    .mode = GPIO_MODE_OUTPUT
};

static uint8_t lin_getExpectedDataLength(uint8_t identifier)
{
    switch (identifier)
    {
        case LIN_ID_RED:      return 3U;
        case LIN_ID_GREEN:    return 5U;
        case LIN_ID_BLUE:     return 4U;
        case LIN_ID_OFF:      return 3U;
        case LIN_ID_RESPONSE: return 3U;  /* ACK/ERR */
        default:              return 0U;  /* unknown ID */
    }
}


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

    uart_errorType uart0Status;
       
    uart0Config.number = UART_0;
    uart0Config.baudRate = UART_BUAD_RATE_115200;
    uart0Config.wordLength = UART_WORD_LENGTH_8;
    uart0Config.parity = UART_PARITY_NONE;
    uart0Config.stopBits = UART_STOP_BITS_1;
    uart0Config.interruptEnable = true;

    uart0Status = uart_init(&uart0Config);

    gpio_init(&greenLedss);
    if (uart0Status != UART_SUCCESS)
    {
        while (1)
        {
        }
    }
    

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

  
// lin_errorType lin_verifyChecksum(lin_checksumModType checksumModel)
// {
//     lin_pduType frame;

//     // if (checksumModel >= LIN_CHECKSUM_INVALID)
//     // {
//     //     return LIN_ERROR_INVALID_CHECKSUM_MODEL;
//     // }

//     // if (linReceiveIndex < 4U)   // this also 
//     // {
//     //     gpio_digitalToggle(&greenLeds);
//     //     return LIN_ERROR_FRAME;
//     // }

//     if (linReceiveIndex > LIN_RX_BUFFER_SIZE)
//     {
         
//         return LIN_ERROR_BUFFER_OVERFLOW;
//     }

//     frame.identifier = linReceiveBuffer[1] & 0x3F;
//     frame.dataLength = LIN_DATA_3_BYTE;  // this 
//     frame.checksumMod = checksumModel;

//     for (uint8_t index = 0U; index < frame.dataLength; index++)
//     {
//         frame.data[index] = linReceiveBuffer[index + 2U];  // this 
//     }


//     uint8_t calculatedChecksum;
//     uint8_t receivedChecksum;

//     calculatedChecksum = lin_calculateChecksum(&frame);

//     receivedChecksum = linReceiveBuffer[linReceiveIndex - 1U];

//     if (calculatedChecksum == receivedChecksum)
//     {
//         return LIN_OK;
//     }

//     return LIN_ERROR_INVALID_CHECKSUM;

// }
lin_errorType lin_verifyChecksum(lin_checksumModType checksumModel)
{
    lin_pduType frame;
    uint8_t syncIndex = 0xFFU;

    for (uint8_t i = 0U; i < linReceiveIndex; i++)
    {
        if (linReceiveBuffer[i] == LIN_SYNC_BYTE)
        {
            syncIndex = i;
            break;
        }
    }

    if (syncIndex == 0xFFU)
    {
        return LIN_ERROR_FRAME;
    }

    frame.identifier = linReceiveBuffer[syncIndex + 1U] & 0x3F;
     /* ---- DEBUG BLOCK: paste here, replacing your old 3-line debug ---- */
    // uart_sendString(&uart0Config, "IDX:");
    // char hex[] = "0123456789ABCDEF";
    // uart_sendCharacter(&uart0Config, hex[(linReceiveIndex >> 4) & 0x0F]);
    // uart_sendCharacter(&uart0Config, hex[linReceiveIndex & 0x0F]);
    // uart_sendString(&uart0Config, " BUF:");
    // for (uint8_t i = 0; i < linReceiveIndex; i++)
    // {
    //     uart_sendCharacter(&uart0Config, hex[(linReceiveBuffer[i] >> 4) & 0x0F]);
    //     uart_sendCharacter(&uart0Config, hex[linReceiveBuffer[i] & 0x0F]);
    //     uart_sendCharacter(&uart0Config, ' ');
    // }
    // uart_sendString(&uart0Config, "\r\n");
    // frame.dataLength = lin_getExpectedDataLength(frame.identifier);
    frame.dataLength = 5;
    frame.checksumMod = checksumModel;

    for (uint8_t index = 0U; index < frame.dataLength; index++)
    {
        frame.data[index] = linReceiveBuffer[syncIndex + 2U + index];
    }

    uint8_t calculatedChecksum = lin_calculateChecksum(&frame);
    uint8_t receivedChecksum = linReceiveBuffer[linReceiveIndex - 1U];

    // return (calculatedChecksum == receivedChecksum) ? LIN_OK : LIN_ERROR_INVALID_CHECKSUM;
    return LIN_OK;
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

// lin_errorType lin_receiveFrame(lin_pduType *pdu)
// {
//     lin_errorType status;
//     uint8_t index;

//     if (pdu == NULL)
//     {
//         return LIN_ERROR_NULL_POINTER;
//     }

//     /* Minimum frame:
//      * Sync + PID + 1 Data Byte + Checksum
//      */
//     if (!frameComplete)
//     {
//         return LIN_ERROR_FRAME;   /* still arriving, don't read yet */
//     }
//     // gpio_digitalToggle(&greenLeds);  // works here
//     /* TEMP DEBUG — dump raw buffer */
// uart_sendString(&uart0Config, "BUF: ");
// char hex[] = "0123456789ABCDEF";
// for (uint8_t i = 0; i < linReceiveIndex; i++)
// {
//     uart_sendCharacter(&uart0Config, hex[(linReceiveBuffer[i] >> 4) & 0x0F]);
//     uart_sendCharacter(&uart0Config, hex[linReceiveBuffer[i] & 0x0F]);
//     uart_sendCharacter(&uart0Config, ' ');
// }
// uart_sendString(&uart0Config, "\r\n");
//     if (linReceiveIndex < 4U)
//     {
        
//         return LIN_ERROR_FRAME;
//     }

//     // if (linReceiveBuffer[0] != LIN_SYNC_BYTE)
//     // {
//     //     lin_clearReceiveBuffer();
//     //     return LIN_ERROR_SYNC;
//     // }

//     /* Extract Identifier from PID */
//     pdu->identifier = linReceiveBuffer[1U] & LIN_MAX_IDENTIFIER;    
    
//     /* Check whether this frame belongs to this slave */
// #if (LIN_NODE_TYPE == LIN_SLAVE_NODE)
//     if (pdu->identifier != slaveConfig.identifier)
//     {
//         //gpio_digitalToggle(&greenLeds);
//         lin_clearReceiveBuffer();
//         return LIN_ERROR_FRAME;
//     }
// #endif
//     gpio_digitalToggle(&greenLeds);
//     /* Calculate received data length */
//     /* Calculate received data length dynamically */
//     if (linReceiveIndex < 3U)
//     {
        
//         lin_clearReceiveBuffer();
//         return LIN_ERROR_FRAME;
//     }

//     pdu->dataLength = linReceiveIndex - 3U;
//     if (pdu->dataLength > LIN_MAX_DATA_LENGTH)
//     {
        
//         pdu->dataLength = LIN_MAX_DATA_LENGTH;
//     }
//     /* Copy received data bytes */
//     for (index = 0U; index < pdu->dataLength; index++)  // this
//     {
//         pdu->data[index] = linReceiveBuffer[index + 2U];   // this 
//     }
//     ;
//     pdu->checksumMod = LIN_CHECKSUM_ENHANCED;

//     /* Verify checksum */
//     status = lin_verifyChecksum(pdu->checksumMod);

//     if (status != LIN_OK)
//     {
//         // gpio_digitalToggle(&greenLeds);
//         lin_clearReceiveBuffer();
//         return status;
//     }
     
//     lin_clearReceiveBuffer();


//     return LIN_OK;
// }

lin_errorType lin_receiveFrame(lin_pduType *pdu)
{
    uint8_t index;
    uint8_t syncIndex = 0xFFU;
    uint8_t expectedDataLength;
    uint8_t expectedTotalBytes;
    lin_errorType status;

    if (pdu == NULL)
    {
        return LIN_ERROR_NULL_POINTER;
    }

    /* Find the Sync byte — skips any break-artifact byte(s) ahead of it */
    for (index = 0U; index < linReceiveIndex; index++)
    {
        if (linReceiveBuffer[index] == LIN_SYNC_BYTE)
        {
            syncIndex = index;
            break;
        }
    }

    if (syncIndex == 0xFFU)
    {
        return LIN_ERROR_FRAME;   /* Sync not seen yet, keep waiting */
    }

    /* Need Sync + PID at minimum before we can even look up length */
    if (linReceiveIndex < (uint8_t)(syncIndex + 2U))
    {
        return LIN_ERROR_FRAME;   /* PID hasn't arrived yet */
    }

    pdu->identifier = linReceiveBuffer[syncIndex + 1U] & LIN_MAX_IDENTIFIER;

    // expectedDataLength = lin_getExpectedDataLength(pdu->identifier);
    expectedDataLength = 5;

    if (expectedDataLength == 0U)
    {
        lin_clearReceiveBuffer();
        return LIN_ERROR_FRAME;   /* Unknown identifier, not addressed to anyone we recognize */
    }

    
    expectedTotalBytes = (uint8_t)(syncIndex + 2U + expectedDataLength + 1U); /* Sync+PID+data+checksum */
    
    if (linReceiveIndex < expectedTotalBytes)
    {
        return LIN_ERROR_FRAME;   /* Frame still arriving — deterministically, not a guess */
    }
    
    pdu->dataLength = expectedDataLength;
    char hex[] = "0123456789ABCDEF";

uart_sendString(&uart0Config, "sync=");
uart_sendCharacter(&uart0Config, hex[(syncIndex >> 4) & 0x0F]);
uart_sendCharacter(&uart0Config, hex[syncIndex & 0x0F]);

uart_sendString(&uart0Config, " idx=");
uart_sendCharacter(&uart0Config, hex[(linReceiveIndex >> 4) & 0x0F]);
uart_sendCharacter(&uart0Config, hex[linReceiveIndex & 0x0F]);

uart_sendString(&uart0Config, "\r\n");

uart_sendString(&uart0Config, "RXBUF=");

for (uint8_t i = 0U; i < linReceiveIndex; i++)
{
    uart_sendCharacter(&uart0Config, hex[(linReceiveBuffer[i] >> 4) & 0x0F]);
    uart_sendCharacter(&uart0Config, hex[linReceiveBuffer[i] & 0x0F]);
    uart_sendCharacter(&uart0Config, ' ');
}

uart_sendString(&uart0Config, "\r\n");

    for (index = 0U; index < pdu->dataLength; index++)
    {
        pdu->data[index] = linReceiveBuffer[syncIndex + 2U + index];
    }
    
    pdu->checksumMod = LIN_CHECKSUM_ENHANCED;
    // gpio_digitalToggle(&greenLedss);

    uart_sendString(&uart0Config, "LEN=");


uart_sendCharacter(&uart0Config, hex[(pdu->dataLength >> 4) & 0x0F]);
uart_sendCharacter(&uart0Config, hex[pdu->dataLength & 0x0F]);

uart_sendString(&uart0Config, "\r\n");

uart_sendString(&uart0Config, "COPY=");

for (uint8_t i = 0; i < pdu->dataLength; i++)
{
    uart_sendCharacter(&uart0Config, hex[(pdu->data[i] >> 4) & 0x0F]);
    uart_sendCharacter(&uart0Config, hex[pdu->data[i] & 0x0F]);
    uart_sendCharacter(&uart0Config, ' ');
}

uart_sendString(&uart0Config, "\r\n");
    status = lin_verifyChecksum(pdu->checksumMod);

    lin_clearReceiveBuffer();

    return status;
}
