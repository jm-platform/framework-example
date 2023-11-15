/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Lib messaging include messagings
 *
 *---------------------------------------------------------------------------*/
#include "lib_messaging.h" 
#include <boost/interprocess/ipc/message_queue.hpp>

/*-----------------------------------------------------------------------------
 * Lib messaging functions
 *
 *---------------------------------------------------------------------------*/
void lib_messaging_InitializeIPCMessage(IPCMessage &ipcMessage)
{
    ipcMessage.priority = 0;
    ipcMessage.source   = 0;

    memset(ipcMessage.topic, 0, sizeof(ipcMessage.topic));
    memset(ipcMessage.message, 0, sizeof(ipcMessage.message));
}

/*---------------------------------------------------------------------------*/
void lib_messaging_FormatIPCMessage(IPCMessage &ipcMessage, 
                                    const unsigned int priority,
                                    const char* topic,
                                    const char* message,
                                    const uint8_t source)
{
    ipcMessage.priority = priority;
    ipcMessage.source   = source;

    strcpy(ipcMessage.topic, topic);
    strcpy(ipcMessage.message, message);
}

/*---------------------------------------------------------------------------*/
int lib_messaging_InitializeQueue(const uint8_t queueID)
{
    char queueName[LIB_MESSAGING_MAX_QUEUE_NAME_SIZE];
    sprintf(queueName, "%s%d", LIB_MESSAGING_QUEUE_NAME_PREFIX, queueID);

    BOOST_TRY
    {
        boost::interprocess::message_queue::remove(queueName);
        boost::interprocess::message_queue queue(boost::interprocess::create_only,
                                                 queueName,
                                                 LIB_MESSAGING_MAX_QUEUE_SIZE,
                                                 sizeof(IPCMessage));
        
        return RETURN_CODE_OK;
    }

    BOOST_CATCH(boost::interprocess::interprocess_exception &ex)
    {
        std::cout << "error: " << ex.what() << std::endl;
      
        return RETURN_CODE_FAIL;
    } BOOST_CATCH_END
}

/*---------------------------------------------------------------------------*/
bool lib_messaging_ReadQueue(const uint8_t queueID, IPCMessage &ipcMessage)
{
    char queueName[LIB_MESSAGING_MAX_QUEUE_NAME_SIZE];
    sprintf(queueName, "%s%d", LIB_MESSAGING_QUEUE_NAME_PREFIX, queueID);

    BOOST_TRY
    {
        boost::interprocess::message_queue queue(boost::interprocess::open_only, queueName);

        boost::interprocess::message_queue::size_type receivedSize = 0;

        bool received = queue.try_receive(&ipcMessage, 
                                          sizeof(ipcMessage), 
                                          receivedSize, 
                                          ipcMessage.priority);
        
        if ((received) && (receivedSize == sizeof(ipcMessage)))
        {
            return true;
        }
        
        return false;
    }

    BOOST_CATCH(boost::interprocess::interprocess_exception &ex)
    {
        return false;
    } BOOST_CATCH_END
}

/*---------------------------------------------------------------------------*/
bool lib_messaging_WriteQueue(const uint8_t queueID, IPCMessage &ipcMessage, unsigned int priority)
{
    char queueName[LIB_MESSAGING_MAX_QUEUE_NAME_SIZE];
    sprintf(queueName, "%s%d", LIB_MESSAGING_QUEUE_NAME_PREFIX, queueID);

    BOOST_TRY
    {
        boost::interprocess::message_queue queue(boost::interprocess::open_only,
                                                 queueName);

        return queue.try_send(&ipcMessage, sizeof(ipcMessage), priority);
    }

    BOOST_CATCH(boost::interprocess::interprocess_exception &ex)
    {      
        return false;
    } BOOST_CATCH_END
}

/*---------------------------------------------------------------------------*/
bool lib_messaging_WriteQueue(QueueList queueList, IPCMessage &ipcMessage, unsigned int priority)
{
    int messageSend = 0;

    for (QueueList::iterator queueIt = queueList.begin(); queueIt != queueList.end(); queueIt++)
	{
		messageSend += lib_messaging_WriteQueue(*queueIt, ipcMessage, priority);
	}

    return (bool)messageSend;
}

/*---------------------------------------------------------------------------*/