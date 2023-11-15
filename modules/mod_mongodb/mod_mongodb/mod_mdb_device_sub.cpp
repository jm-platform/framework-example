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
 * mod MongoDB device sub include times
 *
 *---------------------------------------------------------------------------*/
#include "mod_mdb_device_sub.h" 
#include "mod_db_device.h" 

/*-----------------------------------------------------------------------------
 * MongoDB settings and workers
 *
 *---------------------------------------------------------------------------*/
extern MongoDBSettings mongodbSettings;
extern MongoDBWorkers  mongodbWorkers;

/*-----------------------------------------------------------------------------
 * mod MongoDB device sub read functions
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * mod MongoDB device sub write functions
 *
 *---------------------------------------------------------------------------*/
int mod_mdb_device_sub_UpdateSubDeviceOnline(const String deviceId,
									  			const String &dateTime,
									  			const int &subDeviceId,
									  			const bool online)
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
			bson_t *filter = BCON_NEW(BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_ID), 
									  BCON_UTF8(deviceId.c_str()));
			bson_t *opts = BCON_NEW("projection", "{", "_id", BCON_BOOL(false), "}");

			mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(mongodbWorkers.dbConnection.collection,
																	   filter,
																	   opts,
																	   NULL);
			const bson_t *item = NULL;
			char *json = NULL;
			String jsonOutput;
			bson_error_t error;

			if (mongoc_cursor_next(cursor, &item))
			{
				if (!mongoc_cursor_error(cursor, &error))
				{				
					json = bson_as_json(item, NULL);
				
					returnCode = mod_mongodb_DeviceProcessSubDeviceOnline(jsonOutput, 
																		  json,
																		  dateTime,
																		  subDeviceId,
																		  online);

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

			if (returnCode == RETURN_CODE_OK)
			{
				if (mongoc_cursor_error(cursor, &error))
				{
					size_t bufferLength = sizeof("MongoDB error=\"\"") +
										sizeof(error.message);
					char buffer[bufferLength];
					snprintf(buffer, bufferLength, "MongoDB error=\"%s\"", error.message);
					debug_masked_log(MOD_DEBUG_LOG_MODE_TYPE_GENERAL, buffer);

					returnCode = RETURN_CODE_FAIL;
				}

				bson_t *bsonData = bson_new_from_json((uint8_t*) jsonOutput.c_str(), 
													  jsonOutput.length(), 
													  &error);
				bson_t *update = BCON_NEW("$set", 
										  "{", 
										  BCON_UTF8(MOD_MONGODB_VALUE_NAME_SUB_DEVICES), 
										  BCON_ARRAY(bsonData),
										  "}");

				if (!mongoc_collection_update_one(mongodbWorkers.dbConnection.collection, 
												  filter, update, NULL, NULL, &error)) 
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

				bson_destroy(update);
			}

			bson_destroy(filter);

			mod_mongodb_CollectionDisconnect(mongodbWorkers.dbConnection);
		}

		mod_mongodb_DisonnectClient(mongodbWorkers.dbConnection);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mdb_device_sub_UpdateSubDeviceSensorData(const String deviceId,
										  			const String &dateTime,
										  			const int &subDeviceId,
										  			const int	&heartbeat,
										  			const int	&oxygen,
										  			const float &bodyTemperature,
										  			const float &latitude,
										  			const float &longitude,
										  			const int	&ambientNoise)
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
			bson_t *filter = BCON_NEW(BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_ID), 
									  BCON_UTF8(deviceId.c_str()));
			bson_t *opts = BCON_NEW("projection", "{", "_id", BCON_BOOL(false), "}");

			mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(mongodbWorkers.dbConnection.collection,
																	   filter,
																	   opts,
																	   NULL);
			const bson_t *item = NULL;
			char *json = NULL;
			String jsonOutput;
			bson_error_t error;

			if (mongoc_cursor_next(cursor, &item))
			{
				if (!mongoc_cursor_error(cursor, &error))
				{				
					json = bson_as_json(item, NULL);
				
					returnCode = mod_mongodb_DeviceProcessSubDeviceSensors(jsonOutput, 
																		   json,
																		   dateTime,
																		   subDeviceId,
																		   heartbeat,
																		   oxygen,
																		   bodyTemperature,
																		   latitude,
																		   longitude,
																		   ambientNoise);

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

			if (returnCode == RETURN_CODE_OK)
			{
				bson_t *bsonData = bson_new_from_json((uint8_t*) jsonOutput.c_str(), 
													  jsonOutput.length(), 
													  &error);
				bson_t *update = BCON_NEW("$set", 
										  "{", 
										  BCON_UTF8(MOD_MONGODB_VALUE_NAME_SUB_DEVICES), 
										  BCON_ARRAY(bsonData), 
										  MOD_MONGODB_VALUE_NAME_DEVICE_DATA_LOCATION_MODIFIED,
										  BCON_UTF8(dateTime.c_str()),
										  "}");

				if (!mongoc_collection_update_one(mongodbWorkers.dbConnection.collection, 
												  filter, update, NULL, NULL, &error)) 
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

				bson_destroy(update);
			}

			bson_destroy(filter);

			mod_mongodb_CollectionDisconnect(mongodbWorkers.dbConnection);
		}

		mod_mongodb_DisonnectClient(mongodbWorkers.dbConnection);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mdb_device_sub_UpdateSubDeviceStatus(const String deviceId,
									  			const String &dateTime,
									  			const int &subDeviceId,
									  			const String &proximityRange,
									  			const String &physicalHealth,
									  			const String &inCombat,
									  			const String &emergency,
									  			const String &support)
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
			bson_t *filter = BCON_NEW(BCON_UTF8(MOD_MONGODB_VALUE_NAME_DEVICE_ID), 
									  BCON_UTF8(deviceId.c_str()));
			bson_t *opts = BCON_NEW("projection", "{", "_id", BCON_BOOL(false), "}");

			mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(mongodbWorkers.dbConnection.collection,
																	   filter,
																	   opts,
																	   NULL);
			const bson_t *item = NULL;
			char *json = NULL;
			String jsonOutput;
			bson_error_t error;

			if (mongoc_cursor_next(cursor, &item))
			{
				if (!mongoc_cursor_error(cursor, &error))
				{				
					json = bson_as_json(item, NULL);
				
					returnCode = mod_mongodb_DeviceProcessSubDeviceStatus(jsonOutput, 
																		  json,
																		  dateTime,
																		  subDeviceId,
																		  proximityRange,
																		  physicalHealth,
																		  inCombat,
																		  emergency,
																		  support);

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

			if (returnCode == RETURN_CODE_OK)
			{
				bson_t *bsonData = bson_new_from_json((uint8_t*)jsonOutput.c_str(), 
													  jsonOutput.length(), 
													  &error);

				bson_t *update = BCON_NEW("$set", 
										  "{", 
										  BCON_UTF8(MOD_MONGODB_VALUE_NAME_SUB_DEVICES), 
										  BCON_ARRAY(bsonData), 
										  MOD_MONGODB_VALUE_NAME_DEVICE_DATA_LOCATION_MODIFIED,
										  BCON_UTF8(dateTime.c_str()),
										  "}");

				if (!mongoc_collection_update_one(mongodbWorkers.dbConnection.collection, 
												  filter, update, NULL, NULL, &error)) 
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

				bson_destroy(update);
			}

			bson_destroy(filter);

			mod_mongodb_CollectionDisconnect(mongodbWorkers.dbConnection);
		}

		mod_mongodb_DisonnectClient(mongodbWorkers.dbConnection);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/