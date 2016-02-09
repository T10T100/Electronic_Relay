#include "relay.h"

const char  message_0[] = "<Time>";
const char  message_1[] = "<Analog>";
const char  message_2[] = "<Manual>";
const char  message_3[] = "<Muted>";
const char  message_4[] = "Overlay";

const char *modeMessages[] = {
    message_0,
    message_1,
    message_2,
    message_3,
    message_4
};

const char  message_5[] = "<On>";
const char  message_6[] = "<Off>";

const char *statusMessages[] = {
    message_5,
    message_6,
};

const char  relay_stat_mess_0[] = "OVR :off";
const char  relay_stat_mess_1[] = "OVR : on";

const char *relay_stat_messages[] = {
    relay_stat_mess_0,
    relay_stat_mess_1,
};


const char  relay_onoff_mess_0[] = "Disabled";
const char  relay_onoff_mess_1[] = "Enabled";

const char *relay_onoff_messages[] = {
    relay_onoff_mess_0,
    relay_onoff_mess_1,
};

RelayPresetDataPack DefaultRelayPreset_0  = { 
    /*.AnalogTop      = */90.0f, \
    /*.AnalogBotom    = */50.0f, \
    /*.timeOn         = */{0 , 10, 0, 0}, \
    /*.timeOff        = */{0 , 10, 0, 0}, \
    /*.mode           = */RelayManualMode, \
    /*.state          = */RelayOn, \
    /*.ovh            = */false, \
    /*.ovl            = */false, \
    /*.overlay        = */OverlayDummy, \
    /*.timeStamp      = */{0 , 10, 0, 0} \
};
RelayPresetDataPack DefaultRelayPreset_1  = { 
    /*.AnalogTop      = */90.0f, \
    /*.AnalogBotom    = */50.0f, \
    /*.timeOn         = */{0 , 0, 30, 0}, \
    /*.timeOff        = */{0 , 1, 0, 0}, \
    /*.mode           = */RelayTimeWindowMode, \
    /*.state          = */RelayOff, \
    /*.ovh            = */false, \
    /*.ovl            = */false, \
    /*.overlay        = */OverlayDummy, \
    /*.timeStamp      = */{0 , 10, 0, 0} \
};
RelayPresetDataPack DefaultRelayPreset_2  = { 
    /*.AnalogTop      = */90.0f, \
    /*.AnalogBotom    = */50.0f, \
    /*.timeOn         = */{0 , 0, 20, 0}, \
    /*.timeOff        = */{0 , 0, 40, 0}, \
    /*.mode           = */RelayTimeWindowMode, \
    /*.state          = */RelayOff, \
    /*.ovh            = */false, \
    /*.ovl            = */false, \
    /*.overlay        = */OverlayDummy, \
    /*.timeStamp      = */{0 , 0, 0, 0} \
};
RelayPresetDataPack DefaultRelayPreset_3  = { 
    /*.AnalogTop      = */90.0f, \
    /*.AnalogBotom    = */50.0f, \
    /*.timeOn         = */{0 , 0, 10, 0}, \
    /*.timeOff        = */{0 , 0, 30, 0}, \
    /*.mode           = */RelayTimeWindowMode, \
    /*.state          = */RelayOff, \
    /*.ovh            = */false, \
    /*.ovl            = */false, \
    /*.overlay        = */OverlayDummy, \
    /*.timeStamp      = */{0 , 0, 0, 0} \
};
RelayPresetDataPack DefaultRelayPreset_4  = { 
    /*.AnalogTop      = */70.0f, \
    /*.AnalogBotom    = */50.0f, \
    /*.timeOn         = */{0 , 1, 0, 0}, \
    /*.timeOff        = */{0 , 2, 0, 0}, \
    /*.mode           = */RelayTimeWindowMode, \
    /*.state          = */RelayOff, \
    /*.ovh            = */false, \
    /*.ovl            = */false, \
    /*.overlay        = */OverlayDummy, \
    /*.timeStamp      = */{0 , 0, 0, 0} \
};
RelayPresetDataPack DefaultRelayPreset_5  = { 
    /*.AnalogTop      = */90.0f, \
    /*.AnalogBotom    = */50.0f, \
    /*.timeOn         = */{0 , 10, 0, 0}, \
    /*.timeOff        = */{0 , 10, 0, 0}, \
    /*.mode           = */RelayTimeWindowMode, \
    /*.state          = */RelayOff, \
    /*.ovh            = */false, \
    /*.ovl            = */false, \
    /*.overlay        = */OverlayDummy, \
    /*.timeStamp      = */{0 , 10, 0, 0} \
};
RelayPresetDataPack DefaultRelayPreset_6  = { 
    /*.AnalogTop      = */90.0f, \
    /*.AnalogBotom    = */50.0f, \
    /*.timeOn         = */{0 , 10, 0, 0}, \
    /*.timeOff        = */{0 , 10, 0, 0}, \
    /*.mode           = */RelayTimeWindowMode, \
    /*.state          = */RelayOff, \
    /*.ovh            = */false, \
    /*.ovl            = */false, \
    /*.overlay        = */OverlayDummy, \
    /*.timeStamp      = */{0 , 10, 0, 0} \
};
    
RelayPresetDataPack RelayDefaultPresets[] = {
    DefaultRelayPreset_0,
    DefaultRelayPreset_1,
    DefaultRelayPreset_2,
    DefaultRelayPreset_3,
};

const char preset0NameMsg[] = "PRESET0";
const char preset1NameMsg[] = "PRESET1";
const char preset2NameMsg[] = "PRESET2";
const char preset3NameMsg[] = "PRESET3";
const char preset4NameMsg[] = "PRESET4";
const char preset5NameMsg[] = "PRESET5";
const char preset6NameMsg[] = "PRESET6";

const char *presetNames[] = {
    preset0NameMsg,
    preset1NameMsg,
    preset2NameMsg,
    preset3NameMsg,
    preset4NameMsg,
    preset5NameMsg,
    preset6NameMsg,
};




