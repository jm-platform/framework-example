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
 * mod MongoDB device core include times
 *
 *---------------------------------------------------------------------------*/
#include "mod_mdb_device_core.h" 
#include "mod_db_device.h" 

/*-----------------------------------------------------------------------------
 * MongoDB settings and workers
 *
 *---------------------------------------------------------------------------*/
extern MongoDBSettings mongodbSettings;
extern MongoDBWorkers  mongodbWorkers;

/*-----------------------------------------------------------------------------
 * mod MongoDB device core read functions
 *
 *---------------------------------------------------------------------------*/
int mod_mdb_device_core_ReadDeviceById(DBDevice &dbDevice, const String &deviceId)
{
	int returnCode = RETURN_CODE_OK;

	if (deviceId.length() == 0)
	{
		returnCode = RETURN_CODE_FAIL;
	}

	returnCode = mod_mongodb_ConnectClient(mongodbWorkers.dbConnection, 
										   mongodbSettings.mongoDBUri);

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = mod_mongodb_CollectionConnect(mongodbWorkers.dbConnection,
												   MOD_MONGODB_DATABASE_NAME_DEVICES,
												   MOD_MONGODB_COLLECTION_NAME_DEVICES);

		if (returnCode == RETURN_CODE_OK)
		{							
			bson_t *filter = BCON_NEW(BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_ID), BCON_UTF8(deviceId.c_str()));
			bson_t *opts = BCON_NEW("projection", "{", "_id", BCON_BOOL(false), "}");

			mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(mongodbWorkers.dbConnection.collection,
																	   filter,
																	   opts,
																	   NULL);
			const bson_t *item = NULL;
			char *json = NULL;
			bson_error_t error;

			if (mongoc_cursor_next(cursor, &item))
			{
				if (!mongoc_cursor_error(cursor, &error))
				{
					json = bson_as_json(item, NULL);
				
					mod_mongodb_DeviceReadFromJson(dbDevice, json);

					bson_free(json);
				}
			}

			else
			{
				char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
				sprintf(buffer, "device \"%s\" not found", deviceId.c_str());
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

				returnCode = RETURN_CODE_ERR_DEVICE_UNKNOWN;
			}

			if (mongoc_cursor_error(cursor, &error))
			{
				size_t bufferLength = sizeof("MongoDB error=\"\"") +
						 			  sizeof(error.message);
				char buffer[bufferLength];
				snprintf(buffer, bufferLength, "MongoDB error=\"%s\"", error.message);
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

				returnCode = RETURN_CODE_FAIL;
			}

			mongoc_cursor_destroy(cursor);
			bson_destroy(opts);
			bson_destroy(filter);

			mod_mongodb_CollectionDisconnect(mongodbWorkers.dbConnection);

		}

		mod_mongodb_DisonnectClient(mongodbWorkers.dbConnection);
	}

	return returnCode;
}



/*-----------------------------------------------------------------------------
 * mod MongoDB device core write functions
 *
 *---------------------------------------------------------------------------*/
int mod_mdb_device_core_UpdateOnlineById(const bool &online, const String &deviceId)
{
	int returnCode = RETURN_CODE_OK;

	if (deviceId.length() == 0)
	{
		returnCode = RETURN_CODE_FAIL;
	}

	returnCode = mod_mongodb_ConnectClient(mongodbWorkers.dbConnection, 
										   mongodbSettings.mongoDBUri);

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = mod_mongodb_CollectionConnect(mongodbWorkers.dbConnection,
												   MOD_MONGODB_DATABASE_NAME_DEVICES,
												   MOD_MONGODB_COLLECTION_NAME_DEVICES);

		if (returnCode == RETURN_CODE_OK)
		{							
			bson_t *selector = BCON_NEW(BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_ID), 
										BCON_UTF8(deviceId.c_str()));
			bson_t *update = BCON_NEW("$set", "{", MOD_MONGODB_VALUE_NAME_DEVICE_ONLINE, 
									BCON_BOOL(online), "}");
			bson_error_t error;

			if (!mongoc_collection_update_one(mongodbWorkers.dbConnection.collection, 
												selector, update, NULL, NULL, &error)) 
			{
				size_t bufferLength = sizeof("MongoDB error=\"\"") + sizeof(error.message);
				char buffer[bufferLength];
				snprintf(buffer, bufferLength, "MongoDB error=\"%s\"", error.message);
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
			}

			else
			{
				char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
				sprintf(buffer, "DBDevice \"%s\" updated", deviceId.c_str());
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
			}

			bson_destroy(selector);
			bson_destroy(update);

			mod_mongodb_CollectionDisconnect(mongodbWorkers.dbConnection);

		}

		mod_mongodb_DisonnectClient(mongodbWorkers.dbConnection);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mdb_device_core_AddDeviceValue(const DeviceValue &deviceValue)
{
	int returnCode = RETURN_CODE_OK;

	if (deviceValue.deviceId.length() == 0)
	{
		returnCode = RETURN_CODE_FAIL;
	}

	returnCode = mod_mongodb_ConnectClient(mongodbWorkers.dbConnection, 
										   mongodbSettings.mongoDBUri);

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = mod_mongodb_CollectionConnect(mongodbWorkers.dbConnection,
												   MOD_MONGODB_DATABASE_NAME_DEVICES,
												   MOD_MONGODB_COLLECTION_NAME_VALUE);

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = RETURN_CODE_FAIL;

			bson_t *selector = BCON_NEW(BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_ID), 
										BCON_UTF8(deviceValue.deviceId.c_str()),
										BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_TYPE), 
										BCON_UTF8(deviceValue.type.c_str()),
										BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_NAME), 
										BCON_UTF8(deviceValue.name.c_str()));

			bson_t *opts = BCON_NEW(BCON_UTF8("upsert"), BCON_BOOL(true));
			bson_error_t error;
			bson_t *update = NULL;
			
			if (deviceValue.type == LIB_DEFINE_VALUE_TYPE_STATUS_STR)
			{
				returnCode = RETURN_CODE_OK;

				update = BCON_NEW("$set", "{",
									MOD_MONGODB_VALUE_NAME_DEVICE_ID, 
									BCON_UTF8(deviceValue.deviceId.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_TYPE,
									BCON_UTF8(deviceValue.type.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_NAME, 
									BCON_UTF8(deviceValue.name.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_DATE, 
									BCON_UTF8(deviceValue.date.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_ICON, 
									BCON_UTF8(deviceValue.icon.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_VALUE, 
									BCON_UTF8(deviceValue.value.c_str()),
								  "}");
			}

			if (deviceValue.type == LIB_DEFINE_VALUE_TYPE_DATA_STR)
			{
				returnCode = RETURN_CODE_OK;

				update = BCON_NEW("$set", "{",
									MOD_MONGODB_VALUE_NAME_DEVICE_ID, 
									BCON_UTF8(deviceValue.deviceId.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_TYPE,
									BCON_UTF8(deviceValue.type.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_NAME, 
									BCON_UTF8(deviceValue.name.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_DATE, 
									BCON_UTF8(deviceValue.date.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_ICON, 
									BCON_UTF8(deviceValue.icon.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_VALUE, 
									BCON_UTF8(deviceValue.value.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_UNIT, 
									BCON_UTF8(deviceValue.unit.c_str()),
									MOD_MONGODB_VALUE_NAME_DEVICE_VALUE_NOITATION, 
									BCON_UTF8(deviceValue.notation.c_str()),
								  "}");
			}

			if (returnCode == RETURN_CODE_OK)
			{
				if (!mongoc_collection_update_one(mongodbWorkers.dbConnection.collection, 
											  selector, update, opts, NULL, &error)) 
				{
					returnCode = RETURN_CODE_FAIL;

					size_t bufferLength = sizeof("MongoDB error=\"\"") + sizeof(error.message);
					char buffer[bufferLength];
					snprintf(buffer, bufferLength, "MongoDB error=\"%s\"", error.message);
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
				sprintf(buffer, "DBDevice \"%s\" updated", deviceValue.deviceId.c_str());
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
			}

			else
			{
				char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
				sprintf(buffer, "DBDevice \"%s\" not updated", deviceValue.deviceId.c_str());
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
			}

			bson_destroy(selector);
			bson_destroy(update);

			mod_mongodb_CollectionDisconnect(mongodbWorkers.dbConnection);

		}

		mod_mongodb_DisonnectClient(mongodbWorkers.dbConnection);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mdb_device_core_AddNotificationById(const String &date,
									const String &level,
									const String &message,
									const String &deviceId)
{
	int returnCode = RETURN_CODE_OK;

	if (deviceId.length() == 0)
	{
		returnCode = RETURN_CODE_FAIL;
	}

	returnCode = mod_mongodb_ConnectClient(mongodbWorkers.dbConnection, 
										   mongodbSettings.mongoDBUri);

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = mod_mongodb_CollectionConnect(mongodbWorkers.dbConnection,
												   MOD_MONGODB_DATABASE_NAME_DEVICES,
												   MOD_MONGODB_COLLECTION_NAME_DEVICE_DATA);

		if (returnCode == RETURN_CODE_OK)
		{							
			bson_t *selector = BCON_NEW(BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_ID), 
										BCON_UTF8(deviceId.c_str()));
			bson_t *update = BCON_NEW("$push", "{", MOD_MONGODB_VALUE_NAME_DEVICE_DATA_NOTIFICATIONS,
											   "{", MOD_MONGODB_VALUE_NAME_NOTIFICATIONS_DATE,
											   		BCON_UTF8(date.c_str()),
													   MOD_MONGODB_VALUE_NAME_NOTIFICATIONS_LEVEL,
											   		BCON_UTF8(level.c_str()),
													   MOD_MONGODB_VALUE_NAME_NOTIFICATIONS_MESSAGE,
											   		BCON_UTF8(message.c_str()),
													"}",
												"}");
			bson_error_t error;

			if (!mongoc_collection_update_one(mongodbWorkers.dbConnection.collection, 
												selector, update, NULL, NULL, &error)) 
			{
				size_t bufferLength = sizeof("MongoDB error=\"\"") + sizeof(error.message);
				char buffer[bufferLength];
				snprintf(buffer, bufferLength, "MongoDB error=\"%s\"", error.message);
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
			}

			else
			{
				char buffer[MOD_DEBUG_MESSAGE_LENGTH_MAX];
				sprintf(buffer, "DBDevice \"%s\" updated", deviceId.c_str());
				debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);
			}

			bson_destroy(selector);
			bson_destroy(update);

			mod_mongodb_CollectionDisconnect(mongodbWorkers.dbConnection);

		}

		mod_mongodb_DisonnectClient(mongodbWorkers.dbConnection);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/