#ifndef LCD_DRIVER
#define LCD_DRIVER

#ifdef __hd066__
#include "hd066.h"
#else

#endif


class LcdInterface {
    private :
        LCD *controller;
    public :
        LcdInterface () {}
        LcdInterface (LCD *controller, uint8_t mode)
        {
            this->controller = controller;
            controller->deinit();
            controller->init("", mode);
            this->controller = controller;
        }
        void operator () (LCD *controller, uint8_t mode)
        {
            this->controller = controller;
            controller->init("", mode);
            this->controller = controller;
        }
        
        void setCursor (uint8_t x, uint8_t y)
        {
            controller->setCursor(x, y);
        }
        void clear ()
        {
            controller->clear();
        }
        
        void fillVisible ()
        {
            prints("        ", 0, 0);
            prints("        ", 0, 1);
        }
        
        void fillLine (uint8_t line)
        {
            prints("        ", 0, line);
        }
        
        void putChar (char c)
        {
            controller->putChar(c);
        }
        
        template <typename S>
            void prints (S *string)
            {
                int i = 0;
                while (string[i] != 0) {
                    putChar(string[i++]);
                }
            }
        template <typename S>
            void prints (S *string, int x, int y = 0)
            {
                int i = 0;
                controller->setCursor(x, y);
                while (string[i] != 0) {
                    putChar(string[i++]);
                }
            }
    
};


#endif