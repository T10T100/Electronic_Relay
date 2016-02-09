#ifndef HD066_LCD_CONTROLLER
#define HD066_LCD_CONTROLLER

#include "stdint.h"

enum BusModes {
    __4BitBus = 4,
    __8bitBus = 8,
    
};



#define __lcd_enter_main_mode 0x20

#define __lcd_line_numbers_2 0x08
#define __lcd_line_numbers_1 0x00

#define __lcd_format_510 0x04
#define __lcd_format_58 0x00



#define __lcd_move_properties 0x04

#define __lcd_move_screen_decrement 0x00
#define __lcd_move_screen_increment 0x02

#define __lcd_move screen 0x08
#define __lcd_move_cursor 0x00

#define __lcd_go_home 0x02

#define __lcd_clear 0x01

#define __lcd_enter_display_mode 0x08

#define __lcd_enable_display 0x04
#define __lcd_show_cursor 0x02
#define __lcd_cursor_blinking 0x01


#define __lcd_ddram_select 0x80







static const char *lcd_controller_name = "HD066__";

class LCD {
    private :
        
        virtual void command_next ();
        virtual void data_next ();
        virtual void data_on_bus (uint8_t data);
        virtual void strobe ();
        virtual void delay_ms (uint32_t ms);
        virtual void reset () /*stub*/
        {

        }           
        uint8_t bus_width;
        bool initSucceed;
        void delay_ns (uint32_t ns)
        {
            while (ns-- != 0) {
                
            }
        }
        void deinit ()
        {
            command_next();
            data_on_bus(0x00);
            strobe();
            delay_ms(10);
        }
        
        void init (const char *args, uint8_t mode)
        {
            delay_ms(150);
            bus_width = mode;
            if (mode == __4BitBus) {
                command_next();
                data_on_bus(0x00);
                strobe();
                delay_ms(1);
                data_on_bus(0x02);
                strobe();
                delay_ms(1);
                sendCommand(__lcd_enter_main_mode | \
                            __lcd_line_numbers_2 | \
                             __lcd_format_510);
                sendCommand(__lcd_move_properties); delay_ms(1);
                sendCommand(__lcd_go_home); delay_ms(1);
                sendCommand(__lcd_clear); delay_ms(2);
                sendCommand(__lcd_enter_display_mode | \
                            __lcd_enable_display | \
                            __lcd_show_cursor | \
                            __lcd_cursor_blinking); delay_ms(1);
                initSucceed = true;
            } else {
                
                
                initSucceed = false;
            }
        }
        
        
        void sendData (uint8_t data)
        {
            data_next();
            if (bus_width == __4BitBus) {
                data_on_bus(data >> 4); /*most signed nibble*/
                strobe ();
                delay_ns(200);
                data_on_bus(data); /*least signed nibble*/
                strobe ();
            } else {
                data_on_bus(data);
                strobe ();
            }
            delay_ns(200);
        }
        void sendCommand (uint8_t com)
        {
            command_next();
            if (bus_width == __4BitBus) {
                data_on_bus(com >> 4); /*most signed nibble*/
                strobe ();
                delay_ns(200);
                data_on_bus(com); /*least signed nibble*/
                strobe ();
            } else {
                data_on_bus(com);
                strobe ();
            }
            delay_ns(200);
        }
        
        friend class LcdInterface;
    public :
        LCD () {}
        LCD (uint8_t bus)
        {
            //bus_width = bus;
            //init("");
        }
        
        void setMode (uint8_t mode)
        {
            
        }
        void setCursor (int8_t x, int8_t y)
        {
            y = y > 0 ? 0x40 : 0;
            sendCommand(__lcd_ddram_select | x | y );
            delay_ms(1);
        }
        void putChar (char c, int8_t x, int8_t y )
        {
            y = y > 0 ? 0x40 : 0;
            sendCommand(__lcd_ddram_select | x | y );
            sendData(c);
        }
        
        void putChar (char c)
        {
            sendData(c);
        }
        
        void home ()
        {
            sendCommand(__lcd_go_home);
            delay_ms(1);
        }
        
        void clear ()
        {
            sendCommand(__lcd_clear);
            delay_ms(2);
        }
        
        void sleep ()
        {
            
        }
        
        void wakeUp ()
        {
            
        }
        
        
        
        
        uint8_t getBusWidth ()
        {
            return this->bus_width;
        }
        
    
    
};


#endif 

