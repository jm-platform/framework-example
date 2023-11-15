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
 * mod MongoDB device include times
 *
 *---------------------------------------------------------------------------*/
#include "mod_db_device.h" 

/*-----------------------------------------------------------------------------
 * mod MongoDB device functions
 *
 *---------------------------------------------------------------------------*/
void mod_mongodb_DeviceInit(DBDevice &dbDevice)
{
	dbDevice.deviceId.clear();
	dbDevice.deviceGroup.clear();
	dbDevice.deviceName.clear();

	dbDevice.online = false;

	dbDevice.lastModified.clear();
}

/*---------------------------------------------------------------------------*/
void mod_mongodb_DeviceClear(DBDevice &dbDevice)
{
	mod_mongodb_DeviceInit(dbDevice);
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceParseDeviceId(DBDevice &dbDevice, json_object *deviceObject)
{
	int returnCode = RETURN_CODE_FAIL;

	json_object *jsonObject = json_object_object_get(deviceObject, MOD_MONGODB_VALUE_NAME_DEVICE_ID);

	if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_string))
	{
		if (json_object_get_string_len(jsonObject) > 0)
		{
			dbDevice.deviceId.assign(json_object_get_string(jsonObject));

			returnCode = RETURN_CODE_OK;
		}

		else
		{
			returnCode = RETURN_CODE_FAIL;
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceParseDeviceGroup(DBDevice &dbDevice, json_object *deviceObject)
{
	int returnCode = RETURN_CODE_FAIL;

	json_object *jsonObject = json_object_object_get(deviceObject, MOD_MONGODB_VALUE_NAME_DEVICE_GROUP);

	if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_string))
	{
		if (json_object_get_string_len(jsonObject) > 0)
		{
			dbDevice.deviceGroup.assign(json_object_get_string(jsonObject));

			returnCode = RETURN_CODE_OK;
		}

		else
		{
			returnCode = RETURN_CODE_FAIL;
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceParseDeviceName(DBDevice &dbDevice, json_object *deviceObject)
{
	int returnCode = RETURN_CODE_FAIL;

	json_object *jsonObject = json_object_object_get(deviceObject, MOD_MONGODB_VALUE_NAME_DEVICE_NAME);

	if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_string))
	{
		if (json_object_get_string_len(jsonObject) > 0)
		{
			dbDevice.deviceName.assign(json_object_get_string(jsonObject));

			returnCode = RETURN_CODE_OK;
		}

		else
		{
			returnCode = RETURN_CODE_FAIL;
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceParseDeviceOnline(DBDevice &dbDevice, json_object *deviceObject)
{
	int returnCode = RETURN_CODE_FAIL;

	json_object *jsonObject = json_object_object_get(deviceObject, MOD_MONGODB_VALUE_NAME_DEVICE_ONLINE);

	if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_boolean))
	{
		dbDevice.online = json_object_get_boolean(jsonObject);

		returnCode = RETURN_CODE_OK;
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceReadFromJson(DBDevice &dbDevice, const char *jsonData)
{
	int returnCode = RETURN_CODE_OK;

	if (jsonData == NULL)
	{
		return RETURN_CODE_FAIL;
	}

	if (strlen(jsonData) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	json_object *deviceObject = json_tokener_parse(jsonData);

	if (deviceObject != NULL)
	{
		returnCode = mod_mongodb_DeviceParseDeviceId(dbDevice, deviceObject);

		if (returnCode == RETURN_CODE_OK)
		{
			mod_mongodb_DeviceParseDeviceGroup(dbDevice, deviceObject);
		}

		if (returnCode == RETURN_CODE_OK)
		{
			mod_mongodb_DeviceParseDeviceName(dbDevice, deviceObject);
		}

		if (returnCode == RETURN_CODE_OK)
		{
			mod_mongodb_DeviceParseDeviceOnline(dbDevice, deviceObject);
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * mod MongoDB device data functions
 *
 *---------------------------------------------------------------------------*/
void mod_mongodb_DeviceDataInit(DBDeviceData &dbDeviceData)
{
	dbDeviceData.digitalInput = false;
	dbDeviceData.emailsAmount = 0;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceParseDigitalInput(DBDeviceData &dbDeviceData, json_object *deviceObject)
{
	int returnCode = RETURN_CODE_FAIL;

	json_object *jsonObject = json_object_object_get(deviceObject, 
													 MOD_MONGODB_VALUE_NAME_DEVICE_DATA_DIGITAL_INPUT);

	if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_boolean))
	{
		dbDeviceData.digitalInput = json_object_get_boolean(jsonObject);

		returnCode = RETURN_CODE_OK;
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceParseEmailNotifications(DBDeviceData &dbDeviceData, json_object *deviceObject)
{
	int returnCode = RETURN_CODE_FAIL;

	json_object *jsonObject = json_object_object_get(deviceObject, 
													 MOD_MONGODB_VALUE_NAME_DEVICE_DATA_EMAILS);

	if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_array))
	{
		dbDeviceData.emailsAmount = json_object_array_length(jsonObject);

		for (int index = 0; index < dbDeviceData.emailsAmount; index++)
		{
			json_object *jsonValue = json_object_array_get_idx(jsonObject, index);

			dbDeviceData.notificationEmails[index] = String(json_object_get_string(jsonValue));
		}

		returnCode = RETURN_CODE_OK;
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceDataReadFromJson(DBDeviceData &dbDeviceData, const char *jsonData)
{
	int returnCode = RETURN_CODE_OK;

	if (jsonData == NULL)
	{
		return RETURN_CODE_FAIL;
	}

	if (strlen(jsonData) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	json_object *deviceObject = json_tokener_parse(jsonData);

	if (deviceObject != NULL)
	{
		returnCode = mod_mongodb_DeviceParseDigitalInput(dbDeviceData, deviceObject);

		if (returnCode == RETURN_CODE_OK)
		{

			returnCode = mod_mongodb_DeviceParseEmailNotifications(dbDeviceData, deviceObject);
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceParseSensorDevices(json_object *deviceObject, 
										 json_object **subDeviceObject, 
										 const int subDeviceId,
										 int &subDeviceIndex)
{
	int returnCode = RETURN_CODE_OK;

	if ((deviceObject == NULL) || json_object_object_length(deviceObject) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		json_object *subDevicesObject = json_object_object_get(deviceObject, 
													 	   	   MOD_MONGODB_VALUE_NAME_SUB_DEVICES);

		if (subDevicesObject != NULL)
		{
			if (json_object_get_type(subDevicesObject) == json_type_array)
			{
				int deviceAmount = json_object_array_length(subDevicesObject);

				bool subDeviceFound = false;

				for (int index = 0; index < deviceAmount; index++)
				{
					json_object *object = json_object_array_get_idx(subDevicesObject, index);

					int deviceId = 0; 
					returnCode = lib_json_ReadJsonInteger(deviceId,
														MOD_MONGODB_VALUE_NAME_DEVICE_ID,
														object);
					
					if ((returnCode == RETURN_CODE_OK) && (deviceId == subDeviceId))
					{
						subDeviceFound = true;
						subDeviceIndex = index;

						*subDeviceObject = object;
						break;
					}
				}

				if (!subDeviceFound)
				{
					returnCode = RETURN_CODE_FAIL;
				}
			}
		}	
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceSetSensorDeviceSensors(json_object **deviceObject,
											 const int &heartbeat,
											 const int &oxygen,
											 const float &bodyTemperature,
											 const float &latitude,
											 const float &longitude,
											 const int &ambientNoise)
{
	int returnCode = RETURN_CODE_OK;

	if ((*deviceObject == NULL) || json_object_object_length(*deviceObject) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = RETURN_CODE_FAIL;

		json_object *sensorsObject = json_object_new_object();
		
		if (sensorsObject != NULL)
		{
			json_object *latitudeObject = json_object_new_double((double)latitude);

			if (latitudeObject != NULL)
			{
				returnCode = json_object_object_add(sensorsObject, 
													MOD_MONGODB_VALUE_NAME_LATITUDE,
													latitudeObject);
			}

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *longitudeObject = json_object_new_double((double)longitude);

				if (longitudeObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_LONGITUDE,
														longitudeObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *heartbeatObject = json_object_new_int(heartbeat);

				if (heartbeatObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_HEARTBEAT,
														heartbeatObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				char temperatureString[5];
				sprintf(temperatureString, "%.1f", bodyTemperature);
				json_object *temperatureObject = json_object_new_string(temperatureString);

				if (temperatureObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_BODY_TEMPERATURE,
														temperatureObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *oxygenObject = json_object_new_int(oxygen);

				if (oxygenObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_BLOOD_SATURATION,
														oxygenObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *noiseObject = json_object_new_int(ambientNoise);

				if (noiseObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_AMBIENT_NOISE,
														noiseObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				returnCode = json_object_object_add(*deviceObject, 
													MOD_MONGODB_VALUE_NAME_SENSORS,
													sensorsObject);
			}

			returnCode = RETURN_CODE_OK;
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceSetSensorDeviceStatus(json_object **deviceObject,
											const String &proximityRange,
										 	const String &physicalHealth,
										 	const String &inCombat,
										 	const String &emergency,
										 	const String &support)
{
	int returnCode = RETURN_CODE_OK;

	if ((*deviceObject == NULL) || json_object_object_length(*deviceObject) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		returnCode = RETURN_CODE_FAIL;

		json_object *sensorsObject = json_object_new_object();
		
		if (sensorsObject != NULL)
		{
			json_object *proximityRangeObject = json_object_new_string(proximityRange.c_str());

			if (proximityRangeObject != NULL)
			{
				returnCode = json_object_object_add(sensorsObject, 
													MOD_MONGODB_VALUE_NAME_PROXIMITY_RANGE,
													proximityRangeObject);
			}

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *physicalHealthObject = json_object_new_string(physicalHealth.c_str());

				if (physicalHealthObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_CONDITION,
														physicalHealthObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *inCombatObject = json_object_new_string(inCombat.c_str());

				if (inCombatObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_IN_COMBAT,
														inCombatObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *emergencyObject = json_object_new_string(emergency.c_str());

				if (emergencyObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_EMERGENCY,
														emergencyObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *supportObject = json_object_new_string(support.c_str());

				if (supportObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_SUPPORT,
														supportObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *overalObject = json_object_new_string(support.c_str());

				if (overalObject != NULL)
				{
					returnCode = json_object_object_add(sensorsObject, 
														MOD_MONGODB_VALUE_NAME_OVERAL_STATUS,
														overalObject);
				}
			}

			if (returnCode == RETURN_CODE_OK)
			{
				returnCode = json_object_object_add(*deviceObject, 
													MOD_MONGODB_VALUE_NAME_STATUS,
													sensorsObject);
			}

			returnCode = RETURN_CODE_OK;
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceSetSensorDeviceOnline(json_object **deviceObject,
									  		const String &dateTime,
									  		const bool online)
{
	int returnCode = RETURN_CODE_OK;

	if ((*deviceObject == NULL) || json_object_object_length(*deviceObject) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		json_object *modifiedObject = json_object_new_string(dateTime.c_str());
		
		if (modifiedObject != NULL)
		{
			returnCode = json_object_object_add(*deviceObject, 
												MOD_MONGODB_VALUE_NAME_DEVICE_LAST_MODIFIED,
												modifiedObject);

			if (returnCode == RETURN_CODE_OK)
			{
				json_object *onlineObject = json_object_new_boolean(online);

				if (onlineObject != NULL)
				{
					returnCode = json_object_object_add(*deviceObject, 
														MOD_MONGODB_VALUE_NAME_DEVICE_ONLINE,
														onlineObject);
				}
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceSetSubDeviceSensors(json_object **deviceObject,
										  const String &dateTime,
										  const int &heartbeat,
										  const int &oxygen,
										  const float &bodyTemperature,
										  const float &latitude,
										  const float &longitude,
										  const int &ambientNoise)
{
	int returnCode = RETURN_CODE_OK;

	if ((*deviceObject == NULL) || json_object_object_length(*deviceObject) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		json_object *modifiedObject = json_object_new_string(dateTime.c_str());
		
		if (modifiedObject != NULL)
		{
			json_object_object_add(*deviceObject, 
									MOD_MONGODB_VALUE_NAME_DEVICE_LAST_MODIFIED,
									modifiedObject);

			returnCode = mod_mongodb_DeviceSetSensorDeviceSensors(deviceObject,
																  heartbeat,
																  oxygen,
																  bodyTemperature,
																  latitude,
																  longitude,
																  ambientNoise);
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceSetSubDeviceStatus(json_object **deviceObject,
										 const String &dateTime,
										 const String &proximityRange,
										 const String &physicalHealth,
										 const String &inCombat,
										 const String &emergency,
										 const String &support)
{
	int returnCode = RETURN_CODE_OK;

	if ((*deviceObject == NULL) || json_object_object_length(*deviceObject) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		json_object *modifiedObject = json_object_new_string(dateTime.c_str());
		
		if (modifiedObject != NULL)
		{
			json_object_object_add(*deviceObject, 
									MOD_MONGODB_VALUE_NAME_DEVICE_LAST_MODIFIED,
									modifiedObject);

			returnCode = mod_mongodb_DeviceSetSensorDeviceStatus(deviceObject,
																 proximityRange,
															  	 physicalHealth,
															  	 inCombat,
															   	 emergency,
															  	 support);
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceSetSensorDevices(json_object **deviceObject, 
									   json_object *subDeviceObject,
									   int &subDeviceIndex)
{
	int returnCode = RETURN_CODE_OK;

	if ((*deviceObject == NULL) || json_object_object_length(*deviceObject) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	if (returnCode == RETURN_CODE_OK)
	{
		json_object *subDevicesObject = json_object_object_get(*deviceObject, 
													 	   	   MOD_MONGODB_VALUE_NAME_SUB_DEVICES);

		if (subDevicesObject != NULL)
		{
			if (json_object_get_type(subDevicesObject) == json_type_array)
			{
				json_object_array_put_idx(subDevicesObject, subDeviceIndex, subDeviceObject);

				returnCode = json_object_object_add(*deviceObject, 
													MOD_MONGODB_VALUE_NAME_SENSORS,
													subDevicesObject);
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceProcessSubDeviceOnline(String &outputData,
											 const char *jsonData,							  
											 const String &dateTime,
											 const int &subDeviceId,
											 const bool online)
{
	int returnCode = RETURN_CODE_OK;

	if (jsonData == NULL)
	{
		return RETURN_CODE_FAIL;
	}

	if (strlen(jsonData) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	json_object *deviceObject = json_tokener_parse(jsonData);

	if (deviceObject != NULL)
	{
		int subDeviceIndex = 0;

		json_object *subDeviceObject = json_object_new_object();

		returnCode = mod_mongodb_DeviceParseSensorDevices(deviceObject, 
														  &subDeviceObject, 
														  subDeviceId,
														  subDeviceIndex);

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_mongodb_DeviceSetSensorDeviceOnline(&subDeviceObject,
																 dateTime,
																 online);
		}

		if (returnCode == RETURN_CODE_OK)
		{
			json_object *subDevicesObject = json_object_new_object();
			
			String deviceObjectString = json_object_to_json_string(deviceObject);
			json_object *tempDevicesObject = json_tokener_parse(deviceObjectString.c_str());
			
			json_object_object_get_ex(tempDevicesObject,
									  MOD_MONGODB_VALUE_NAME_SUB_DEVICES,
									  &subDevicesObject);
			
			if (subDevicesObject != NULL)
			{
				if (json_object_get_type(subDevicesObject) == json_type_array)
				{
					json_object_array_put_idx(subDevicesObject, subDeviceIndex, subDeviceObject);					

					outputData = json_object_to_json_string(subDevicesObject);
				}
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceProcessSubDeviceSensors(String &outputData,
											  const char *jsonData,							  
											  const String &dateTime,
											  const int &subDeviceId,
											  const int &heartbeat,
											  const int &oxygen,
											  const float &bodyTemperature,
											  const float &latitude,
											  const float &longitude,
											  const int &ambientNoise)
{
	int returnCode = RETURN_CODE_OK;

	if (jsonData == NULL)
	{
		return RETURN_CODE_FAIL;
	}

	if (strlen(jsonData) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	json_object *deviceObject = json_tokener_parse(jsonData);

	if (deviceObject != NULL)
	{
		int subDeviceIndex = 0;

		json_object *subDeviceObject = json_object_new_object();

		returnCode = mod_mongodb_DeviceParseSensorDevices(deviceObject, 
														  &subDeviceObject, 
														  subDeviceId,
														  subDeviceIndex);

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_mongodb_DeviceSetSubDeviceSensors(&subDeviceObject,
															   dateTime,
															   heartbeat,
															   oxygen,
															   bodyTemperature,
															   latitude,
															   longitude,
															   ambientNoise);
		}

		if (returnCode == RETURN_CODE_OK)
		{
			json_object *subDevicesObject = json_object_new_object();
			
			String deviceObjectString = json_object_to_json_string(deviceObject);
			json_object *tempDevicesObject = json_tokener_parse(deviceObjectString.c_str());
			
			json_object_object_get_ex(tempDevicesObject,
									  MOD_MONGODB_VALUE_NAME_SUB_DEVICES,
									  &subDevicesObject);
			
			if (subDevicesObject != NULL)
			{
				if (json_object_get_type(subDevicesObject) == json_type_array)
				{
					json_object_array_put_idx(subDevicesObject, subDeviceIndex, subDeviceObject);					

					outputData = json_object_to_json_string(subDevicesObject);
				}
			}
		}
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceProcessSubDeviceStatus(String &outputData,
											 const char *jsonData,							  
											 const String &dateTime,
											 const int &subDeviceId,
											 const String &proximityRange,
											 const String &physicalHealth,
											 const String &inCombat,
											 const String &emergency,
											 const String &support)
{
	int returnCode = RETURN_CODE_OK;

	if (jsonData == NULL)
	{
		return RETURN_CODE_FAIL;
	}

	if (strlen(jsonData) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	json_object *deviceObject = json_tokener_parse(jsonData);

	if (deviceObject != NULL)
	{
		int subDeviceIndex = 0;

		json_object *subDeviceObject = json_object_new_object();

		returnCode = mod_mongodb_DeviceParseSensorDevices(deviceObject, 
														  &subDeviceObject, 
														  subDeviceId,
														  subDeviceIndex);

		if (returnCode == RETURN_CODE_OK)
		{
			returnCode = mod_mongodb_DeviceSetSubDeviceStatus(&subDeviceObject,
															  dateTime,
															  proximityRange,
															  physicalHealth,
															  inCombat,
															  emergency,
															  support);
		}

		if (returnCode == RETURN_CODE_OK)
		{
			json_object *subDevicesObject = json_object_new_object();

			String deviceObjectString = json_object_to_json_string(deviceObject);
			json_object *tempDevicesObject = json_tokener_parse(deviceObjectString.c_str());
			
			json_object_object_get_ex(tempDevicesObject,
									  MOD_MONGODB_VALUE_NAME_SUB_DEVICES,
									  &subDevicesObject);
			
			if (subDevicesObject != NULL)
			{
				if (json_object_get_type(subDevicesObject) == json_type_array)
				{
					json_object_array_put_idx(subDevicesObject, subDeviceIndex, subDeviceObject);					

					outputData = json_object_to_json_string(subDevicesObject);
				}
			}
		}
	}

	return returnCode;
}

/*-----------------------------------------------------------------------------
 * mod MongoDB device action functions
 *
 *---------------------------------------------------------------------------*/
void mod_mongodb_DeviceActionsInit(DBDeviceActions &dbDeviceActions)
{
	dbDeviceActions.alarm = LIB_MQTT_ACTION_ALARM_STATE_UNKNOWN_STR;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceParseAlarm(DBDeviceActions &dbDeviceActions, json_object *deviceObject)
{
	int returnCode = RETURN_CODE_FAIL;

	json_object *jsonObject = json_object_object_get(deviceObject, 
													 MOD_MONGODB_VALUE_NAME_DEVICE_ACTION_ALARM);

	if ((jsonObject != NULL) && (json_object_get_type(jsonObject) == json_type_string))
	{
		dbDeviceActions.alarm = json_object_get_string(jsonObject);

		returnCode = RETURN_CODE_OK;
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/
int mod_mongodb_DeviceActionsReadFromJson(DBDeviceActions &dbDeviceActions, const char *jsonData)
{
	int returnCode = RETURN_CODE_OK;

	if (jsonData == NULL)
	{
		return RETURN_CODE_FAIL;
	}

	if (strlen(jsonData) == 0)
	{
		return RETURN_CODE_FAIL;
	}

	json_object *deviceObject = json_tokener_parse(jsonData);

	if (deviceObject != NULL)
	{
		returnCode = mod_mongodb_DeviceParseAlarm(dbDeviceActions, deviceObject);
	}

	return returnCode;
}

/*---------------------------------------------------------------------------*/