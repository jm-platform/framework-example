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
 * mod MongoDB Connection include times
 *
 *---------------------------------------------------------------------------*/
#include "mod_db_connection.h" 

/*-----------------------------------------------------------------------------
 * mod MongoDB Connection functions
 *
 *---------------------------------------------------------------------------*/
inline bool mod_mongodb_ClientIsConnected(DBConnection &dbConnection)
{
	if ((dbConnection.mongoClient != NULL) &&
		(dbConnection.mongoURI != NULL))
	{
		return true;
	}

	return false;
}

/*---------------------------------------------------------------------------*/
inline bool mod_mongodb_CollectionIsConnected(DBConnection &dbConnection)
{
	if ((dbConnection.mongoURI != NULL) &&
		(dbConnection.mongoClient != NULL) &&
		(dbConnection.collection != NULL))
	{
		return true;
	}

	return false;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_ConnectClient(DBConnection &dbConnection, const String &uri)
{
	int returnCode = RETURN_CODE_OK;

	if (uri.length() == 0)
	{
		returnCode = RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		mongoc_uri_t *mongoURI = NULL;
		mongoc_client_t *mongoClient = NULL;

		mongoURI = mongoc_uri_new(uri.c_str());

		returnCode = RETURN_CODE_FAIL;

		if (mongoURI != NULL)
		{
			mongoClient = mongoc_client_new_from_uri(mongoURI);

			if (mongoClient != NULL)
			{
				if (mongoc_client_set_appname(mongoClient, MOD_MONGODB_SYSTEM_NAME))
				{
					dbConnection.mongoClient    = mongoClient;
					dbConnection.mongoURI 	    = mongoURI;

					char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
					sprintf(buffer, "uri=\"%s\" connected", uri.c_str());
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

					returnCode = RETURN_CODE_OK;
				}
			}
		}

		if (returnCode != RETURN_CODE_OK)
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
			sprintf(buffer, "uri=\"%s\" failed", uri.c_str());
			debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

			if (mongoURI != NULL) 
			{
				mongoc_uri_destroy(mongoURI);
			}

			if (mongoClient != NULL) 
			{
				mongoc_client_destroy(mongoClient);
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_mongodb_DisonnectClient(DBConnection &dbConnection)
{
	if (dbConnection.mongoClient != NULL)
	{
		mongoc_client_destroy(dbConnection.mongoClient);
		dbConnection.mongoClient = NULL;
	}

	if (dbConnection.mongoURI != NULL)
	{
		mongoc_uri_destroy(dbConnection.mongoURI);
		dbConnection.mongoURI = NULL;
	}

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "client disconnected from MongoDB");
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_CollectionConnect(DBConnection &dbConnection,
                                  const String &database,
								  const String &collection)
{
	int returnCode = RETURN_CODE_OK;

	if ((database.length() == 0) || (collection.length() == 0))
	{
		returnCode = RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
        returnCode = RETURN_CODE_FAIL;

		if (mod_mongodb_ClientIsConnected(dbConnection))
		{
			mongoc_collection_t *mongoCollection = NULL;

			mongoCollection = mongoc_client_get_collection(dbConnection.mongoClient,
														   database.c_str(),
														   collection.c_str());
			if (mongoCollection != NULL)
			{
				dbConnection.collection = mongoCollection;

				returnCode = RETURN_CODE_OK;

                char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
                sprintf(buffer, "database=\"%s\" collection=\"%s\" connected", 
                        database.c_str(), collection.c_str());
                debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
            }
		}

		if (returnCode != RETURN_CODE_OK)
		{
			char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
            sprintf(buffer, "database=\"%s\" collection=\"%s\" failed", 
                    database.c_str(), collection.c_str());
            debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
void mod_mongodb_CollectionDisconnect(DBConnection &dbConnection)
{
	if (dbConnection.collection != NULL)
	{
		mongoc_collection_destroy(dbConnection.collection);
		dbConnection.collection = NULL;
	}

	debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, "disconnected from MongoDB collection");
}

/*---------------------------------------------------------------------------*/