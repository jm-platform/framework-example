/*  (c) 2022  Jorijn Meijer   All Rights Reserved.
 *-----------------------------------------------------------------------------
 *                    JJ    MMMM          MMMM   PPPPPPPP 
 *                    JJ    MM  MM      MM  MM   PP      PP
 *                    JJ    MM    MM  MM    MM   PP      PP
 *                    JJ    MM      MM      MM   PPPPPPPP
 *             JJ     JJ    MM              MM   PP
 *               JJJJJ      MM              MM   PP
 *---------------------------------------------------------------------------*/
#ifndef MOD_PUPPET_DEVICE_IMP_H
#define MOD_PUPPET_DEVICE_IMP_H

/*-----------------------------------------------------------------------------
 * Puppet Device include files
 *
 *---------------------------------------------------------------------------*/
#include "../application_info.h"

/*-----------------------------------------------------------------------------
 * Puppet Device define values
 *
 *---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_BASE_CE_PIN  			            25
#define MOD_PUPPET_DEVICE_BASE_ADDRESS  			        0x7370643030 /* = spd00 */

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_VALUE_SENSOR_LIST  			    "sensor-list"
#define MOD_PUPPET_DEVICE_VALUE_ID  			            "id"
#define MOD_PUPPET_DEVICE_VALUE_NAME  			            "name"
#define MOD_PUPPET_DEVICE_VALUE_IDENTIFIER  			    "identifier"
#define MOD_PUPPET_DEVICE_VALUE_ONLINE  			        "online"

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_ROLE_MANAGER_STR			        "manager"
#define MOD_PUPPET_DEVICE_ROLE_PUPPET_STR			        "puppet"
#define MOD_PUPPET_DEVICE_ROLE_TEST_STR			            "test"

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_SEND_MESSAGE_MASK_DATA            0xFFFFFFFF00000000
#define MOD_PUPPET_DEVICE_SEND_MESSAGE_MASK_ADDRESS         0xFFFFFF00FFFFFFFF

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_READ_MESSAGE_MASK_DATA            0xFFFFFFFF
#define MOD_PUPPET_DEVICE_READ_MESSAGE_MASK_ADDRESS         0xFF

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_STATE_ONLINE                      true
#define MOD_PUPPET_DEVICE_STATE_OFFLINE                     false

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_NRF_MODE_JMP_STR			        "JMP"
#define MOD_PUPPET_DEVICE_NRF_MODE_DWMN_STR			        "DWMN"
#define MOD_PUPPET_DEVICE_NRF_MODE_NORMAL_STR			    "normal"

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_BITSHIFT_MESSAGE_ADDRESS          32

/*-----------------------------------------------------------------------------
 * Puppet Device message addresses
 *
 *---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_NRF_ADDRESS_GENERAL               1
#define MOD_PUPPET_DEVICE_NRF_ADDRESS_TEST                  2

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_NRF_ADDRESS_ACCELEROMETER_X_AXIS  10
#define MOD_PUPPET_DEVICE_NRF_ADDRESS_ACCELEROMETER_Y_AXIS  11
#define MOD_PUPPET_DEVICE_NRF_ADDRESS_ACCELEROMETER_Z_AXIS  12

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_NRF_ADDRESS_GYROSCOPE_X_AXIS      13
#define MOD_PUPPET_DEVICE_NRF_ADDRESS_GYROSCOPE_Y_AXIS      14
#define MOD_PUPPET_DEVICE_NRF_ADDRESS_GYROSCOPE_Z_AXIS      15

/*-----------------------------------------------------------------------------
 * Puppet Device ranges
 *
 *---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_PUPPET_DEVICE_AMOUNT_MAX          6

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_RF_CHANNEL_MAX                    125

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_RF_POWER_LEVEL_MAX                3

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_CONNECTION_TIMOUT_MIN             2
#define MOD_PUPPET_DEVICE_CONNECTION_TIMOUT_MAX             120

/*-----------------------------------------------------------------------------
 * Puppet Device initial settings
 *
 *---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_INITIAL_SENSOR_INFO_PATH_STR      "../data/puppet-devices.json"

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_INITIAL_RF_CHANNEL                80

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_INITIAL_CONNECTION_TIMOUT         10

/*---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_INITIAL_ENABLE_RF                 true

/*-----------------------------------------------------------------------------
 * Puppet Device settings commands
 *
 *---------------------------------------------------------------------------*/
#define MOD_PUPPET_DEVICE_SETTING_SENSOR_INFO_PATH_STR      "puppet-device-sensor-info-path"
#define MOD_PUPPET_DEVICE_SETTING_PRINT_NRF_DETAILS_STR     "puppet-device-print-details"
#define MOD_PUPPET_DEVICE_SETTING_PUPPET_DEVICE_ROLE_STR    "puppet-device-set-role"
#define MOD_PUPPET_DEVICE_SETTING_SET_CHANNEL_STR           "puppet-device-set-channel"
#define MOD_PUPPET_DEVICE_SETTING_SET_POWER_STR             "puppet-device-set-power-level"
#define MOD_PUPPET_DEVICE_SETTING_SET_TIMEOUT_STR           "puppet-device-connection-timeout"
#define MOD_PUPPET_DEVICE_SETTING_SET_ENABLE_RF_STR         "puppet-device-enable-rf"
#define MOD_PUPPET_DEVICE_SETTING_SET_NRF_MODE_STR          "puppet-device-nrf-mode"

/*-----------------------------------------------------------------------------
 * Puppet Device type definitions
 *
 *---------------------------------------------------------------------------*/
typedef struct PuppetDevice
{
    bool        active;
    bool        online;
    String      name;
    int         id;
    uint64_t    address;
    int         connectionTimer;

} PuppetDevice;

/*---------------------------------------------------------------------------*/
typedef void NRFMessageCallBack(int&, float&);

/*---------------------------------------------------------------------------*/
typedef struct NRFMessageAction
{
    uint8_t             address;
    NRFMessageCallBack* messageCallback;

} NRFMessageAction;

/*---------------------------------------------------------------------------*/
typedef std::list<NRFMessageAction> MessageActionList;

/*---------------------------------------------------------------------------*/
typedef enum NRFMode
{
    nrfModeNone = 0,
    nrfModeJMP,
    nrfModeDWMN,
    nrfModeNormal,
    nrfModeUnknown = -1

} NRFMode;

/*-----------------------------------------------------------------------------
 * Puppet Device settings
 *
 *---------------------------------------------------------------------------*/
typedef struct PuppetDeviceSettings
{
    String          puppetDevicesPath;
    bool            printNRFDetails;

    int             rfChannel;
    rf24_pa_dbm_e   rfPowerLevel;
    bool            rfEnabled;
    NRFMode         nrfMode;

    String          rolePuppet;
    uint64_t        puppetAddress;
    int             connectionTimeout;

} PuppetDeviceSettings;

/*-----------------------------------------------------------------------------
 * Puppet Device workers
 *
 *---------------------------------------------------------------------------*/
typedef struct PuppetDeviceWorkers
{
    timer_t             timer1HzId;
    bool                timer1HzTrigger;

    PuppetDevice        puppetDeviceList[MOD_PUPPET_DEVICE_PUPPET_DEVICE_AMOUNT_MAX];
    int                 puppetDeviceAmount;

    float               dataOut;

    MessageActionList   messageActionList;

} PuppetDeviceWorkers;

/*-----------------------------------------------------------------------------
 * Puppet Device exposed functions of the application interface
 *
 *---------------------------------------------------------------------------*/
void mod_puppet_device_PreInitialisation();
int  mod_puppet_device_SettingsHandler(SettingsObjectDataList &settingsObjectDataList);
int  mod_puppet_device_PostInitialisation();
int  mod_puppet_device_DestroyData();
int  mod_puppet_device_MainloopFunction();
int  mod_puppet_device_TestFunction();

/*---------------------------------------------------------------------------*/
int mod_puppet_device_SendMessage(const uint8_t &address, const float &data);
void mod_puppet_device_RegisterMessageAction(const uint8_t &address, 
											 NRFMessageCallBack* messageCallback);
int mod_puppet_device_GetPuppetDeviceId();

/*---------------------------------------------------------------------------*/
#endif /* MOD_PUPPET_DEVICE_IMP_H											 */
/*---------------------------------------------------------------------------*/
