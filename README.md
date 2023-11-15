# JM-Platform

### Jorijn Meijer Platform version 2.0

**Made by: Jorijn Meijer**

The JM-Platform, short for Jorijn Meijer Platform, is a versatile framework and service designed for embedded devices, with a particular focus on Internet of Things (IoT) projects. This platform expedites the development of new C++ projects in Linux environments by harnessing an established architecture of modules and libraries, suitable for both backend microservices and embedded devices. Through a user-friendly web application, devices can be effortlessly monitored and controlled, thanks to the generic functionalities that are beneficial for a wide range of projects, eliminating the need for additional web development. 

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
    ModuleName	                	moduleName;
    ModuleVersion               	moduleVersion;
    ModuleBuild	                	moduleBuild;

    ModulePreInitialization		*modulePreInitialization;
    SettingsHandler					*settingsHandler;
    ModulePostInitialization		*modulePostInitialization;
    ModuleDestroyData		    *moduleDestroyData;
    ModuleMainloopFunction	    *moduleMainloopFunction;
    ModuleTestFunction          	*moduleTestFunction;

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
		mod_skeleton_PreInitialisation,			    	/* Module PreInitialisationPtr */
		mod_skeleton_SettingsHandler,				/* Module PostInitialisationPtr */
		mod_skeleton_PostInitialisation,			/* Module PostInitialisationPtr */
		mod_skeleton_DestroyData,				/* Module DestroyDataPtr */
		mod_skeleton_MainloopFunction,				/* Module MainloopFunctionPtr */
        NULL							    	/* Module TestFunctionPtr */

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

