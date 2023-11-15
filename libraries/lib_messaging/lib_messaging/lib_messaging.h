/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef LIB_MESSAGING_H
#define LIB_MESSAGING_H

/*-----------------------------------------------------------------------------
 * Lib messaging include messagings
 *---------------------------------------------------------------------------*/
#include "../../gcc.h"
#include <boost/interprocess/ipc/message_queue.hpp>

/*---------------------------------------------------------------------------*/
#include "../../application_interface/application_interface.h"
#include "../../lib_defines/lib_defines.h"

/*-----------------------------------------------------------------------------
 * Lib messaging define values
 *
 *---------------------------------------------------------------------------*/
#define LIB_MESSAGING_MAX_QUEUE_SIZE                        1024
#define LIB_MESSAGING_MAX_TOPIC_SIZE                        32
#define LIB_MESSAGING_MAX_MESSAGE_SIZE                      256

/*---------------------------------------------------------------------------*/
#define LIB_MESSAGING_MAX_QUEUE_NAME_SIZE                   15
#define LIB_MESSAGING_QUEUE_NAME_PREFIX                     "jmp-queue-"

/*---------------------------------------------------------------------------*/
#define LIB_MESSAGING_MESSAGE_SEPARATOR                     ':'

/*---------------------------------------------------------------------------*/
#define LIB_MESSAGING_QUEUE_ID_FORBIDDEN                    0

/*-----------------------------------------------------------------------------
 * Lib messaging type definitions
 *
 *---------------------------------------------------------------------------*/
typedef uint8_t QueueID;

/*---------------------------------------------------------------------------*/
typedef struct IPCMessage
{
    unsigned int    priority;
    char            topic[LIB_MESSAGING_MAX_TOPIC_SIZE];
    char            message[LIB_MESSAGING_MAX_MESSAGE_SIZE];
    QueueID         source;

} IPCMessage;

/*---------------------------------------------------------------------------*/
typedef std::list<IPCMessage> IPCMessageList;

/*---------------------------------------------------------------------------*/
typedef std::list<QueueID> QueueList;

/*-----------------------------------------------------------------------------
 * Lib messaging exposed functions
 *
 *---------------------------------------------------------------------------*/
void lib_messaging_InitializeIPCMessage(IPCMessage &ipcMessage);
void lib_messaging_FormatIPCMessage(IPCMessage &ipcMessage, 
                                    const unsigned int priority,
                                    const char* topic,
                                    const char* message,
                                    const uint8_t source);

/*---------------------------------------------------------------------------*/                                    
int  lib_messaging_InitializeQueue(const uint8_t queueID);
bool lib_messaging_ReadQueue(const uint8_t queueID, IPCMessage &ipcMessage);
bool lib_messaging_WriteQueue(const uint8_t queueID, IPCMessage &ipcMessage, unsigned int priority = 0);
bool lib_messaging_WriteQueue(QueueList queueList, IPCMessage &ipcMessage, unsigned int priority = 0);

/*---------------------------------------------------------------------------*/
#endif /* LIB_MESSAGING_H											         */
/*---------------------------------------------------------------------------*/