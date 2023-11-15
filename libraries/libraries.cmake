### SET common directories
SET (LIBRARIES_DIR ${CMAKE_CURRENT_LIST_DIR})

### INCLUDE file library
SET (LIB_FILE_DIR ${LIBRARIES_DIR}/lib_file)
SET (LIB_FILE ${LIB_FILE_DIR}/lib_file.cpp)

### INCLUDE MQTT library
SET (LIB_MQTT_DIR ${LIBRARIES_DIR}/lib_mqtt)
SET (LIB_MQTT ${LIB_MQTT_DIR}/lib_mqtt.cpp)

### INCLUDE time library
SET (LIB_TIME_DIR ${LIBRARIES_DIR}/lib_time)
SET (LIB_TIME ${LIB_TIME_DIR}/lib_time.cpp)

### INCLUDE nrf library
SET (LIB_NRF_DIR ${LIBRARIES_DIR}/lib_nrf)
SET (LIB_NRF ${LIB_NRF_DIR}/lib_nrf.cpp)

### INCLUDE json library
SET (LIB_JSON_DIR ${LIBRARIES_DIR}/lib_json)
SET (LIB_JSON ${LIB_JSON_DIR}/lib_json.cpp)

### INCLUDE imu library
SET (LIB_IMU_DIR ${LIBRARIES_DIR}/lib_imu)
SET (LIB_IMU ${LIB_IMU_DIR}/lib_imu.cpp)

### INCLUDE serial library
SET (LIB_SERIAL_DIR ${LIBRARIES_DIR}/lib_serial)
SET (LIB_SERIAL ${LIB_SERIAL_DIR}/lib_serial.cpp)

### INCLUDE system library
SET (LIB_SYSTEM_DIR ${LIBRARIES_DIR}/lib_system)
SET (LIB_SYSTEM ${LIB_SYSTEM_DIR}/lib_system.cpp)

### INCLUDE messaging library
SET (LIB_MESSAGING_DIR ${LIBRARIES_DIR}/lib_messaging)
SET (LIB_MESSAGING ${LIB_MESSAGING_DIR}/lib_messaging.cpp)

### INCLUDE ncurses library
SET (LIB_NCURSUS_DIR ${LIBRARIES_DIR}/lib_ncurses)
SET (LIB_NCURSUS ${LIB_NCURSUS_DIR}/lib_ncurses.cpp)