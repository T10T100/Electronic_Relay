#ifndef PIN_INTERFACE
#define PIN_INTERFACE

#include "stdint.h"
#include "gpio_ex.h"

template <typename GPIO_Type>
class Pin {
    private :
        GPIO_Type *port;
        uint32_t pin_bm;
    
    public :
        Pin (GPIO_Type *port, uint32_t pin_bm) 
        {
            this->port = port;
            this->pin_bm = pin_bm;        
        }
        
        
        bool pinRead ()
        {
            return ( gpio_pin_read(port, pin_bm) == 0 ? false : true);
        }
        
        void pinWrite (bool v = false)
        {
            gpio_pin_set(port, pin_bm, v);
        }
    
};


#endif