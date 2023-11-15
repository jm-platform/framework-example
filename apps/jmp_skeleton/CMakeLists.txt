#####################################################################
### PROJECT INFORMATION
#####################################################################
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (jmp-skeleton)
MESSAGE ("-- CMake of \"${PROJECT_NAME}\" starting")

#####################################################################
### INCLUDE FILES FROM THIS PROJECT
#####################################################################

### INCLUDE CMAKE FILES (THEIR CONTENT WILL BE ADDED HERE)
INCLUDE (${CMAKE_CURRENT_LIST_DIR}/app_skeleton/app_skeleton.cmake)

### INCLUDE application interface
SET (APPLICATION_INTERFACE ${CMAKE_CURRENT_LIST_DIR}/application_interface.cpp)

#####################################################################
### INCLUDE FILES FROM EXTERNAL SOURCES
#####################################################################

### INCLUDE libraries
INCLUDE (${CMAKE_CURRENT_LIST_DIR}/../../libraries/libraries.cmake)

### INCLUDE modules
INCLUDE (${CMAKE_CURRENT_LIST_DIR}/../../modules/modules.cmake)


#####################################################################
### COMPILER INFORMATION AND SETTINGS
#####################################################################

### COMPILER INFORMATION
SET (CMAKE_C_STANDARD 17)
SET (CMAKE_C_STANDARD_REQUIRED True)
SET (CMAKE_C_EXTENSIONS False)
SET (CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_STANDARD_REQUIRED True)
SET (CMAKE_CXX_EXTENSIONS False)

### COMPILER FLAGS
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -Wall -Wextra -std=c++17")

### COMPILER DEFINITIONS
ADD_DEFINITIONS (-DLINUX)

#####################################################################
### BUILD INFORMATION AND SETTINGS
#####################################################################


#####################################################################
### INCLUDE FILES FROM EXTERNAL LIBRARIES
#####################################################################

### INCLUDE lib pthread
FIND_PACKAGE (Threads REQUIRED)
TARGET_LINK_LIBRARIES (${PROJECT_NAME} PRIVATE "${CMAKE_THREAD_LIBS_INIT}")

### INCLUDE lib rt
FIND_LIBRARY (LIB_RT rt)
TARGET_LINK_LIBRARIES (${PROJECT_NAME} PRIVATE "${LIB_RT}")

### INCLUDE lib mosquitto
FIND_LIBRARY (LIB_MOSQUITTO mosquittopp)
TARGET_LINK_LIBRARIES (${PROJECT_NAME} PRIVATE "${LIB_MOSQUITTO}")