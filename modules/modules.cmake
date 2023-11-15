### SET common directories
SET (MODULE_DIR ${CMAKE_CURRENT_LIST_DIR})

### INCLUDE mod application
SET (MOD_APPLICATION_DIR ${MODULE_DIR}/mod_application)
SET (MOD_APPLICATION ${MOD_APPLICATION_DIR}/mod_application.cpp)

### INCLUDE mod debug
SET (MOD_DEBUG_DIR ${MODULE_DIR}/mod_debug)
SET (MOD_DEBUG ${MOD_DEBUG_DIR}/mod_debug.cpp)

### INCLUDE mod gprs
SET (MOD_GPRS_DIR ${MODULE_DIR}/mod_gprs)
SET (MOD_GPRS ${MOD_GPRS_DIR}/mod_gprs.cpp)

### INCLUDE mod io
SET (MOD_IO_DIR ${MODULE_DIR}/mod_io)
SET (MOD_IO ${MOD_IO_DIR}/mod_io.cpp)

### INCLUDE mod mongodb
SET (MOD_MONGODB_DIR ${MODULE_DIR}/mod_mongodb)
SET (MOD_MONGODB ${MOD_MONGODB_DIR}/mod_mongodb.cpp)

### INCLUDE mod mqtt client
SET (MOD_MQTT_CLIENT_DIR ${MODULE_DIR}/mod_mqtt_client)
SET (MOD_MQTT_CLIENT ${MOD_MQTT_CLIENT_DIR}/mod_mqtt_client.cpp)

### INCLUDE mod mqtt server
SET (MOD_MQTT_SERVER_DIR ${MODULE_DIR}/mod_mqtt_server)
SET (MOD_MQTT_SERVER ${MOD_MQTT_SERVER_DIR}/mod_mqtt_server.cpp)

### INCLUDE mod skeleton
SET (MOD_NOTIFICATIONS_DIR ${MODULE_DIR}/mod_notifications)
SET (MOD_NOTIFICATIONS ${MOD_NOTIFICATIONS_DIR}/mod_notifications.cpp)

### INCLUDE mod skeleton
SET (MOD_SKELETON_DIR ${MODULE_DIR}/mod_skeleton)
SET (MOD_SKELETON ${MOD_SKELETON_DIR}/mod_skeleton.cpp)

### INCLUDE mod gprs
SET (MOD_GPS_DIR ${MODULE_DIR}/mod_gps)
SET (MOD_GPS ${MOD_GPS_DIR}/mod_gps.cpp)

### INCLUDE mod synthesizer
SET (MOD_SYNTHESIZER_DIR ${MODULE_DIR}/mod_synthesizer)
SET (MOD_SYNTHESIZER ${MOD_SYNTHESIZER_DIR}/mod_synthesizer.cpp)

### INCLUDE mod puppet device
SET (MOD_PUPPET_DEVICE_DIR ${MODULE_DIR}/mod_puppet_device)
SET (MOD_PUPPET_DEVICE ${MOD_PUPPET_DEVICE_DIR}/mod_puppet_device.cpp)