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
#include <stdbool.h>

#include "lin.h"

/********************************************* Macros *********************************************/



/****************************************** Enumerations ******************************************/
typedef enum
{
    LIN_APP_INIT,
    LIN_APP_IDLE,
    LIN_APP_WAIT_COMMAND,
    LIN_APP_SEND_COMMAND,
    LIN_APP_WAIT_RESPONSE,
    LIN_APP_PROCESS_COMMAND
} linApp_stateType;


typedef enum
{
    LIN_APP_CMD_RED,
    LIN_APP_CMD_GREEN,
    LIN_APP_CMD_BLUE,
    LIN_APP_CMD_OFF,
    LIN_APP_CMD_ACK,
    LIN_APP_CMD_ERR,
    LIN_APP_CMD_INVALID
} linApp_commandType;

/******************************************* Data Types *******************************************/

/*************************************** Function Prototypes **************************************/

void linApp_init(void);
void linApp_stateMachine(void);
#endif