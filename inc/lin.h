/***************************************************************************************************
 * FILENAME    : lin.h
 * DESCRIPTION : LIN Driver Header File
 *
 * AUTHOR      : Hassan
 *
***************************************************************************************************/

#ifndef LIN_H_
#define LIN_H_

/*************************************** Header Inclusion *****************************************/

#include <stdint.h>
#include <stdbool.h>

#include "uart.h"


/********************************************* Macros *********************************************/

#define LIN_RX_BUFFER_SIZE             (11U)
#define LIN_SYNC_BYTE                  (0x55U)

#define LIN_MAX_DATA_LENGTH            (8U)
#define LIN_MIN_DATA_LENGTH            (1U)

#define LIN_MAX_IDENTIFIER             (0x3FU)
#define LIN_MAX_FRAME_IDENTIFIER       (0x3BU)

#define LIN_DIAG_MASTER_REQUEST_ID     (0x3CU)
#define LIN_DIAG_SLAVE_RESPONSE_ID     (0x3DU)

#define LIN_RESERVED_ID0               (0x3EU)
#define LIN_RESERVED_ID1               (0x3FU)

/****************************************** Enumerations ******************************************/

typedef enum
{
    LIN_OK = 48,
    LIN_ERROR_NULL_POINTER,
    LIN_ERROR_INVALID_BAUDRATE,
    LIN_ERROR_INVALID_IDENTIFIER,
    LIN_ERROR_INVALID_DATA_LENGTH,
    LIN_ERROR_INVALID_CHECKSUM,
    LIN_ERROR_INVALID_CHECKSUM_MODEL,
    LIN_ERROR_UART,
    LIN_ERROR_BUSY,
    LIN_ERROR_TIMEOUT,
    LIN_ERROR_FRAME,
    LIN_ERROR_BREAK,
    LIN_ERROR_SYNC,
    LIN_ERROR_PARITY,
    LIN_ERROR_BUFFER_OVERFLOW,
    LIN_ERROR_UNKNOWN

} lin_errorType;


typedef enum
{
    LIN_CHECKSUM_CLASSIC = 0,
    LIN_CHECKSUM_ENHANCED,

    LIN_CHECKSUM_INVALID

} lin_checksumModType;

typedef enum
{
    LIN_DATA_1_BYTE = 1,
    LIN_DATA_2_BYTE,
    LIN_DATA_3_BYTE,
    LIN_DATA_4_BYTE,
    LIN_DATA_5_BYTE,
    LIN_DATA_6_BYTE,
    LIN_DATA_7_BYTE,
    LIN_DATA_8_BYTE,
}lin_dataLengthType;

/******************************************* Data Types *******************************************/
typedef struct
{
    uint8_t identifier;
    lin_dataLengthType dataLength;
    uint8_t data[LIN_MAX_DATA_LENGTH];
    lin_checksumModType checksumMod;

} lin_pduType;

typedef struct
{
    uint8_t identifier;
} lin_slaveConfigType;

/*************************************** Function Prototypes **************************************/

lin_errorType lin_init(uint32_t baudRate);
lin_errorType lin_sendFrame(const lin_pduType *pdu);
lin_errorType lin_verifyChecksum(lin_checksumModType checksumModel);
lin_errorType lin_copyReceiveBuffer(uint8_t *buffer);
lin_errorType lin_clearReceiveBuffer(void);
void lin_copyByte(void);

lin_errorType lin_slaveInit(const lin_slaveConfigType *config);
lin_errorType lin_receiveFrame(lin_pduType *pdu);


#endif