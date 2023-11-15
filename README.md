# JM-Platform

### Jorijn Meijer Platform version 2.0

**Made by: Jorijn Meijer**

The JM-Platform, short for Jorijn Meijer Platform, is a modular Linux application framework written in C++. It consist of multiple libraries and modules that are reusable for different projects.

Please contact Jorijn Meijer on the email address jor_meijer@hotmail.com if you want to use the platform.

## Platform fundamentals

The platform consists out of three types of components:

- **Libraries:** Libraries are a set of function and definitions. They are called from the modules and application and do not contain an own main loop function.
- **Modules:** Modules are the components were most of the data processing occurs. The each contain an own main loop function and initialization. Every module serves a specific task.
- **Applications:** The applications are the setups that combine all the modules. They also contain functionality that is very specific for the project. Because not everything is reusable for other projects.

#### Module Interfaces

To combine all the modules into the application, an array of modules is defined in the application. Every module has its own module interface. This consist out of a set of callback functions:

- **Pre-Initializer:** In the Pre-Initializer, data and variables are initialized. Al settings are set to their initial values.
- **Settings Handler:** The settings handler is called when the ini-file is read. 
- **Post-Initializer:** The post initializer is there to finalize the initialization. startup procedures that require settings are then started.
- **Destroy Data:** This function is called when the application is stopped. It will then cleanup all necessary parts of the module.
- **Mainloop Function:** The main loop is called for general processing.
- **Test Function:** The test function is only called if a test parameter is given on startup.

The code below show the type definition of the module interface. All data types are also made out of type definitions.

```c++
typedef struct ModuleInterface
{
    ModuleName	                moduleName;
    ModuleVersion               moduleVersion;
    ModuleBuild	                moduleBuild;

    ModulePreInitialization	    *modulePreInitialization;
    SettingsHandler             *settingsHandler;
    ModulePostInitialization    *modulePostInitialization;
    ModuleDestroyData		    *moduleDestroyData;
    ModuleMainloopFunction	    *moduleMainloopFunction;
    ModuleTestFunction          *moduleTestFunction;

} ModuleInterface;
```



The code below shows an example interface of the skeleton module. In here the callback functions of the module are defined. 

```c++
const ModuleInterface mod_skeleton_ModuleInterface =
{

		/* application info */
        MOD_SKELETON_NAME,
		MOD_SKELETON_VERSION,
		MOD_SKELETON_BUILD,

		/* Exposed functions of the module */
		mod_skeleton_PreInitialisation,			    /* Module PreInitialisationPtr */
		mod_skeleton_SettingsHandler,				/* Module PostInitialisationPtr */
		mod_skeleton_PostInitialisation,			/* Module PostInitialisationPtr */
		mod_skeleton_DestroyData,					/* Module DestroyDataPtr */
		mod_skeleton_MainloopFunction,				/* Module MainloopFunctionPtr */
        NULL							    		/* Module TestFunctionPtr */

};
```

The code below show an example module interface setup of the skeleton application. In here a list of modules is defined so that all the callback functions are registered.

```c++
const ModuleInterface *moduleInterfaces[] =
{
	/* Mandatory module interface pointers */
	&mod_debug_ModuleInterface,

	/* Optional module interface pointers */
	&mod_mqtt_client_ModuleInterface,
	&mod_skeleton_ModuleInterface,

	/* Application interface pointers */
	&app_skeleton_ModuleInterface,

	/* The last pointer always a NULL pointer */
	(ModuleInterface*)NULL
};
```

#### MQTT topics

The communication through MQTT is defined with a standard topic format. an example of this is shown below.

```
/* format */
/destination/source/resource-id

/* example */
/jmp-device-server/jmp-device-01/1002
```

Both the device and the server use the same library that contains all the resource id definitions, so that the data can always be processed correctly. 

# Running Applications

### Dependencies

The run a client side application, install the following dependencies:

```
sudo apt-get update
```

**libmosquittopp-dev**

```
sudo apt-get install libmosquittopp-dev
```

**mosquitto**

```
sudo apt-get install mosquitto-clients

sudo systemctl enable mosquitto.service

mosquitto -v

sudo nano /etc/mosquitto/mosquitto.conf
	listener 1883
	allow_anonymous true
```

**json c**

```
sudo apt-get install autoconf

git clone https://github.com/json-c/json-c.git

cd json-c/

mkdir build && cd build

cmake..

make install
```

**Cmake**

```
sudo apt install -y cmake
```

**NRF24**

```
wget http://tmrh20.github.io/RF24Installer/RPi/install.sh

chmod +x install.sh

./install.sh

1.BCM2835 Driver(Performance)
```



### Building

Follow the following steps to build an application, first change to the directory of the application you are trying to build, then:

```
mkdir build

cd build

cmake ..

make
```

### **Startup**

When starting an application, 2 parameters can be given. The first one is the location of the inifile. This is mandatory because the settings are necessary. The second one is the debug mode. This defines what type of data the application should show in the terminal. This is only for development purposes. 

```
./jmp-example inifile="../../jmp-example.ini" debug=4
```



## Development

To create a new module, copy the skeleton module into a new directory and change the following names:

```
skeleton 	--> name

Skeleton 	--> Name

SKELETON 	--> NAME

SkeletMod	--> Name

skeletMod	--> name
```

Afterwards update the modules.cmake file. Lastly, add your new module to the desired application.

## Version control

When adding or changing functionality in the code, please update the version number and add the changes in the changelog.

