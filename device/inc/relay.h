#ifndef RELAY_DEVICE
#define RELAY_DEVICE

#include <stdint.h>
#include "clock.h"
#include "defs.h"






extern const char *modeMessages[];
extern const char *statusMessages[];
extern const char *relay_stat_messages[];
extern const char *relay_onoff_messages[];

typedef struct {
    float AnalogTop;
    float AnalogBotom;
    Time timeOn;
    Time timeOff;
    int8_t mode;
    int8_t state;
    bool ovh;
    bool ovl;
    int8_t overlay;
    Time timeStamp;
} RelayPresetDataPack;


enum RelayModesEnum {
    RelayTimeWindowMode   = 0,
    RelayAnalogWindowMode = 1,
    RelayManualMode       = 2,
    RelayMute             = 3,
    RelayOverlayMode      = 4, /*not used*/
    
};

enum RelayStates {
    RelayOn = 0,
    RelayOff = 1,  
};

enum RelayFuseMode {
    RelayOnWhenFuse = 0,
    RelayOffWhenFuse = 1,
};

enum OverlayCause {
    OverlayDummy = -1,
    OverlayHigh = 1,
    OverlayLow = 2,
};


#define DEFAULT_ANALOG_OFF_VALUE 500.0F
#define DEFAULT_ANALOG_ON_VALUE  600.0F
#define DEFAULT_RELAY_STATE      RelayOn
#define DEFAULT_RELAY_MODE       RelayManualMode
#define DEFAULT_RELAY_OVH        false
#define DEFAULT_RELAY_OVL        false
#define DEFAULT_RELAY_ADJK       1.02f
#define DEFAULT_RELAY_ADJOFF    -800.0f

typedef struct {
    Time onTime;
    Time offTime;  
} TimeControl;

typedef struct {
    float onValue;
    float offValue;
} AnalogControl;


extern RelayPresetDataPack RelayDefaultPresets[];
extern const char *presetNames[];

class Relay {
    private :
        int8_t currentMode;
        int8_t currentState;
    
        TimeControl timeControl;
        AnalogControl analogControl;
    
        Time timeAction;
    
        bool ovHighIsSet;
        bool ovLowIsSet;
        bool overlayIgnore;
        bool inverted;
        int8_t overlay;
    
        float adjustK;
        float adjustOffset;
    
        virtual void RelayWriteControl (bool state);
        virtual float RelayReadAnalog ();
        virtual int32_t RelayReadManual ();
    
        int8_t jitterCancer;
        
        
        
        
        bool fuse ()
        {
            
            return false;
        }
        
        float analogIntegralValue;
        RelayPresetDataPack stashedPreset;
        bool presetStashed;
        int8_t currentPreset;
    public :
        Relay ()
        {
            resetTime ( timeAction );
            resetTime ( timeControl.onTime );
            resetTime ( timeControl.offTime );
            
            timeControl.onTime.seconds = 5;
            timeControl.offTime.seconds = 2;
            
            analogControl.onValue   = DEFAULT_ANALOG_ON_VALUE;
            analogControl.offValue  = DEFAULT_ANALOG_OFF_VALUE;
            currentState            = DEFAULT_RELAY_STATE;
            currentMode             = DEFAULT_RELAY_MODE;
            ovHighIsSet             = DEFAULT_RELAY_OVH;
            ovHighIsSet             = DEFAULT_RELAY_OVL;
            overlayIgnore           = false;
            adjustK                 = DEFAULT_RELAY_ADJK;
            adjustOffset            = DEFAULT_RELAY_ADJOFF;
            overlay                 = OverlayDummy;
            inverted                = false;
            presetStashed           = false;
            currentPreset = 0;
        }
        
        void loadPreset (RelayPresetDataPack &preset, bool loadStamp)
        {
            this->analogControl.onValue         = preset.AnalogTop;
            this->analogControl.offValue        = preset.AnalogBotom;
            this->timeControl.onTime            = preset.timeOn;
            this->timeControl.offTime           = preset.timeOff;
            if (loadStamp == true) {
                this->timeAction = preset.timeStamp;
            } else {
                resetTime ( timeAction );
            }
            this->ovHighIsSet       = preset.ovh;
            this->ovLowIsSet        = preset.ovl;
            this->overlay           = preset.overlay;
            this->currentMode       = preset.mode;
            this->currentState      = preset.state;
            this->updateControl();
        }
        
        const char *loadNextDefaultPreset ()
        {
            currentPreset++;
            if (currentPreset > 4) {
                currentPreset= 0;
            }
            this->loadPreset( RelayDefaultPresets[currentPreset] , false);
            return presetNames[currentPreset];
        }
        
        const char *getPresetInfo ()
        {
            return presetNames[currentPreset];
        }
        
        void stashCurrentPreset ()
        {
            if (presetStashed == true) {
                return;
            }
            stashedPreset.AnalogBotom   = this->analogControl.offValue;
            stashedPreset.AnalogTop     = this->analogControl.onValue;
            stashedPreset.timeOn        = this->timeControl.onTime;
            stashedPreset.timeOff       = this->timeControl.offTime;
            stashedPreset.timeStamp     = this->timeAction;
            stashedPreset.mode          = this->currentMode;
            stashedPreset.state         = this->currentState;
            stashedPreset.ovh           = this->ovHighIsSet;
            stashedPreset.ovl           = this->ovLowIsSet;
            stashedPreset.overlay       = this->overlay;
            presetStashed               = true;
        }
        
        
        void unstashCurrentPreset ()
        {
            if (presetStashed == false) {
                return;
            }
            this->analogControl.onValue     = stashedPreset.AnalogTop;
            this->analogControl.offValue    = stashedPreset.AnalogBotom;
            this->timeControl.onTime        = stashedPreset.timeOn;
            this->timeControl.offTime       = stashedPreset.timeOff;
            this->timeAction                = stashedPreset.timeStamp;
            this->currentMode               = stashedPreset.mode;
            this->currentState              = stashedPreset.state;
            this->ovHighIsSet               = stashedPreset.ovh;
            this->ovLowIsSet                = stashedPreset.ovl;
            this->overlay                   = stashedPreset.overlay;
            this->updateControl();
            presetStashed = false;
        }
        
        void switchInversion ()
        {
            if (inverted == false) {
                inverted = true;
            } else {
                inverted = false;
            }
        }
        
        const char *getInvertedStatus ()
        {
            if (inverted == false) {
                return "Normal";
            } else {
                return "Inverted";
            }
        }
        
        void updateControl ()
        {
            if (currentMode == RelayMute) {
                return;
            }
            RelayWriteControl(inverted == false ? currentState : !currentState);
        }
        
        void setAdjustmentK (float k)
        {
            adjustK = k;
        }
        void setAdjustmentOffset (float offset)
        {
            adjustOffset = offset;
        }
        float getAdjK ()
        {
            return adjustK;

        }
        float getAdjOffset ()
        {
            return adjustOffset;
        }
        
        void overlayTest ()
        {
                    if (ovHighIsSet == true) {
                        if (analogIntegralValue > analogControl.onValue) {
                            overlay = OverlayHigh;
                            currentState = RelayOn;
                        } else {
                            overlay = OverlayDummy;
                        }
                    } else {
                        overlay = OverlayDummy;
                    }
                    if (ovLowIsSet == true) {
                        if (analogIntegralValue < analogControl.offValue) {
                            overlay = OverlayLow;
                            currentState = RelayOff;
                        } else {
                            overlay = OverlayDummy;
                        }
                    } else {
                        overlay = OverlayDummy;
                    }
        }
        
        void tick ()
        {
            int8_t state = currentState;
            
            analogIntegralValue += (RelayReadAnalog() * adjustK + adjustOffset - analogIntegralValue) * 0.03;
            switch (currentMode) {
                case RelayTimeWindowMode :
                    if ( decreaseTime(timeAction) == true) {
                        if (currentState == RelayOn) {
                            currentState = RelayOff;
                            timeAction = timeControl.offTime;
                        } else {
                            currentState = RelayOn;
                            timeAction = timeControl.onTime;
                        }
                    }
                    overlayTest();
                  break;
                case RelayAnalogWindowMode :
                    overlay = OverlayDummy;
                    if (analogIntegralValue > analogControl.onValue) {
                        currentState = RelayOn;
                    } else if (analogIntegralValue < analogControl.offValue) {
                        currentState = RelayOff;
                    }
                  break;
                case RelayManualMode :
                    overlayIgnore = true;
                  break;
                case RelayMute :
                    overlay = OverlayDummy;
                  break;
                default :
                    overlay = OverlayDummy;
                  break;
            };
        }
        
        void programManual (RelayStates state)
        {
            if (currentMode == RelayManualMode) {
                if (currentState != state) {
                    currentState = (state == RelayOff ? RelayOff : RelayOn);
                    updateControl();
                }
                
            }
        }
        
        void switchManual ()
        {
            if (currentMode == RelayManualMode) {
                currentState = (currentState == RelayOff ? RelayOn : RelayOff);
                RelayWriteControl( currentState ); 
            }
        }
        
        void programOnTime (Time &time)
        {
            timeControl.onTime = time;
            timeAction = time;
        }
        
        void programOffTime (Time &time)
        {
            timeControl.offTime = time;
        }
        
        void programOnValue (float value)
        {
            analogControl.onValue = value;
        }
        void programOffValue (float value)
        {
            analogControl.offValue = value;
        }
        
        void programMode (int32_t mode)
        {
            currentMode = mode;
        }
        
        
        void setOverlayHigh (bool v)
        {
            ovHighIsSet = v;
        }
        void setOverlayLow (bool v)
        {
            ovLowIsSet = v;
        }
        
        void switchOverlayHigh ()
        {
            ovHighIsSet = ovHighIsSet == true ? false : true;
        }
        void switchOverlayLow ()
        {
            ovLowIsSet = ovLowIsSet == true ? false : true; 
        }
        
        void setNextMode ()
        {
            currentMode++;
            if (currentMode > RelayMute) {
                currentMode = RelayTimeWindowMode;
            }
        }
        
        float getAnalogValue ()
        {
            return analogIntegralValue;
        }
    
        const char *getStatus ()
        {
            if (overlay == OverlayLow) {
                return relay_stat_messages[0];
            } else if (overlay == OverlayHigh) {
                return relay_stat_messages[1];
            }
            return statusMessages[currentState];
        }
        
        const char *getMode ()
        {
            return modeMessages[currentMode];
        }
        int8_t getModeNum ()
        {
            return currentMode;
        }
        
        int32_t getOnValue ()
        {
            return analogControl.onValue;    
        }
        int32_t getOffValue ()
        {
            return analogControl.offValue;    
        }
        bool getOverlayHigh ()
        {
            return ovHighIsSet;
        }
        bool getOverlayLow ()
        {
            return ovLowIsSet;
        }
        char *getOverlayHighStatus ()
        {
            if (ovHighIsSet == true) {
                return (char *)relay_onoff_messages[1];
            } else {
                return (char *)relay_onoff_messages[0];
            }
        }
        char *getOverlayLowStatus ()
        {
            if (ovLowIsSet == true) {
                return (char *)relay_onoff_messages[0];
            } else {
                return (char *)relay_onoff_messages[1];
            }
        }
        
        
        const char *getActivity ()
        {
            static char modeMessage[16];
            if (overlay == OverlayLow) {
                return relay_stat_messages[0];
            } else if (overlay == OverlayHigh) {
                return relay_stat_messages[1];
            }
            switch (currentMode) {
                case RelayTimeWindowMode:
                    return clockConverter(modeMessage, "hh:mm:ss", timeAction);
                    break;
                case RelayAnalogWindowMode:
                    return "Analog";
                    break;
                case RelayManualMode:
                      return "Manual";
                    break;
                case RelayMute:
                    return "Muted";
                    break;
                
            };
            return modeMessages[currentMode];
        }
        
        Time &getOnTime ()
        {
            return timeControl.onTime;
        }
        
        Time &getOffTime ()
        {
            return timeControl.offTime;
        }
    
};


#endif

