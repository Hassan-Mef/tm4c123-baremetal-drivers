/***************************************************************************************************
 * FILENAME    : linApp.h
 * DESCRIPTION : LIN Driver Header File
 *
 * AUTHOR      : Hassan
 *
***************************************************************************************************/

#ifndef _LINAPP_H_
#define _LINAPP_H_

/*************************************** Header Inclusion *****************************************/

#include <stdint.h>
#include "lin.h"
#include "timer.h"
#include "gpio.h"

/********************************************* Macros *********************************************/



/****************************************** Enumerations ******************************************/
typedef enum
{
    LIN_APP_INIT,
    LIN_APP_IDLE,
    LIN_APP_WAIT_COMMAND,
    LIN_APP_SEND_COMMAND,
    LIN_APP_WAIT_RESPONSE,
    LIN_APP_PROCESS_COMMAND,
    LIN_APP_SEND_RESPONSE
} linApp_stateType;


typedef enum
{
    LIN_APP_COMMAND_RED,
    LIN_APP_COMMAND_GREEN,
    LIN_APP_COMMAND_BLUE,
    LIN_APP_COMMAND_OFF,
    LIN_APP_COMMAND_ACK,
    LIN_APP_COMMAND_ERR,
    LIN_APP_COMMAND_INVALID
} linApp_commandType;

typedef enum
{
    LIN_APP_RESPONSE_ACK,
    LIN_APP_RESPONSE_ERR

} linApp_responseType;

/******************************************* Data Types *******************************************/

/*************************************** Function Prototypes **************************************/

void linApp_init(void);
void linApp_stateMachine(void);
void linApp_setCommand(linApp_commandType command);

void linApp_sendTestFrame(void);
#endif
