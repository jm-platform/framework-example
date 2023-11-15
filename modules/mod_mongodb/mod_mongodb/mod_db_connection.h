/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_DB_CONNECTION_H
#define MOD_DB_CONNECTION_H

/*-----------------------------------------------------------------------------
 * mod MongoDB Connection include files
 *---------------------------------------------------------------------------*/
#include "mod_mongodb_defines.h" 

/*-----------------------------------------------------------------------------
 * mod MongoDB Connection define values
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * MQTT Server type definitions
 *
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * mod MongoDB Connection exposed functions
 *
 *---------------------------------------------------------------------------*/
inline bool mod_mongodb_ClientIsConnected(DBConnection &dbConnection);
inline bool mod_mongodb_CollectionIsConnected(DBConnection &dbConnection);
int mod_mongodb_ConnectClient(DBConnection &dbConnection, const String &uri);
void mod_mongodb_DisonnectClient(DBConnection &dbConnection);
int mod_mongodb_CollectionConnect(DBConnection &dbConnection,
                                  const String &database,
								  const String &collection);
void mod_mongodb_CollectionDisconnect(DBConnection &dbConnection);     

/*---------------------------------------------------------------------------*/
#endif /* MOD_DB_CONNECTION_H											     */
/*---------------------------------------------------------------------------*/