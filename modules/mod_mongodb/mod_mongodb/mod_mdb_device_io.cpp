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
 * mod MongoDB device io include times
 *
 *---------------------------------------------------------------------------*/
#include "mod_mdb_device_io.h" 
#include "mod_db_device.h" 

/*-----------------------------------------------------------------------------
 * MongoDB settings and workers
 *
 *---------------------------------------------------------------------------*/
extern MongoDBSettings mongodbSettings;
extern MongoDBWorkers  mongodbWorkers;

/*-----------------------------------------------------------------------------
 * mod MongoDB device io read functions
 *
 *---------------------------------------------------------------------------*/
int mod_mdb_device_io_ReadDeviceDataById(DBDeviceData &dbDeviceData, const String &deviceId)
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
				
					mod_mongodb_DeviceDataReadFromJson(dbDeviceData, json);

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

/*---------------------------------------------------------------------------*/
int mod_mdb_device_io_ReadDeviceActionsById(DBDeviceActions &dbDeviceActions, const String &deviceId)
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
												   MOD_MONGODB_COLLECTION_NAME_ACTIONS);

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
				
					mod_mongodb_DeviceActionsReadFromJson(dbDeviceActions, json);

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
 * mod MongoDB device io write functions
 *
 *---------------------------------------------------------------------------*/
int mod_mdb_device_io_UpdateDeviceInputById(const bool &digitalInput, const String &deviceId)
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
			bson_t *update = BCON_NEW("$set", "{", MOD_MONGODB_VALUE_NAME_DEVICE_DATA_DIGITAL_INPUT, 
									BCON_BOOL(digitalInput), "}");
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
int mod_mdb_device_io_UpdateDeviceLocationById(const float &latitude,
										 		const float &longitude,
										 		const int &accuracy,
										 		const String &dateTime,
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
			bson_t *update = BCON_NEW("$set", "{", MOD_MONGODB_VALUE_NAME_DEVICE_DATA_LATITUDE, 
													BCON_DOUBLE(latitude), 
													MOD_MONGODB_VALUE_NAME_DEVICE_DATA_LONGITUDE,
													BCON_DOUBLE(longitude), 
													MOD_MONGODB_VALUE_NAME_DEVICE_DATA_ACCURACY,
													BCON_INT32(accuracy), 
													MOD_MONGODB_VALUE_NAME_DEVICE_DATA_LOCATION_MODIFIED,
													BCON_UTF8(dateTime.c_str()),
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
int mod_mdb_device_io_UpdateDeviceAlarmById(const String &AlarmState, const String &deviceId)
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
												   MOD_MONGODB_COLLECTION_NAME_ACTIONS);

		if (returnCode == RETURN_CODE_OK)
		{							
			bson_t *selector = BCON_NEW(BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_ID), 
										BCON_UTF8(deviceId.c_str()));
			bson_t *update = BCON_NEW("$set", "{", MOD_MONGODB_VALUE_NAME_DEVICE_ACTION_ALARM,
													BCON_UTF8(AlarmState.c_str()),
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