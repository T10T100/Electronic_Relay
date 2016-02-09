#ifndef KEYPAD_INTERFACE
#define KEYPAD_INTERFACE


#include "pin.h"

template <typename GPIO_Type>
struct KeyDataStruct {
    Pin<GPIO_Type> *pin;
    char key;
    bool set;
};

template <typename GPIO_Type>
    class Keypad {
        private :
            Pin<GPIO_Type> **pin_set;
            const char *key_set;
            uint32_t key_count;
            bool invert;
            bool ignoreAll;
        public :
            Keypad (Pin<GPIO_Type> **pin_set, const char *key_set, uint32_t key_count, bool invert = false)
            {
                this->pin_set = pin_set;
                this->key_set = key_set;
                this->key_count = key_count;
                this->invert = invert;
                this->ignoreAll = false;
            }
            
            KeyDataStruct<GPIO_Type> test ()
            {
                KeyDataStruct<GPIO_Type> keyData = {nullptr, '\0', false};
                if (ignoreAll == true) {
                    return keyData;
                }
                for (int i = 0; i < key_count; i ++) {
                    if (pin_set[i]->pinRead() == invert) {
                        keyData.pin = pin_set[i];
                        keyData.key = key_set[i];
                        keyData.set = true;
                        return keyData;
                    }
                }
                return keyData;
            }
            
            
            
            void setIgnoreAll (bool value)
            {
                ignoreAll = value;
            }
        
            bool testKey (char key)
            {
                for (int i = 0; i < key_count; i ++) {
                    if (key_set[i] == key) {
                        if (pin_set[i]->pinRead() == invert) {
                            return true;
                        }
                        return false;
                    }
                }
                return false;
            }
        
    };

#endif

