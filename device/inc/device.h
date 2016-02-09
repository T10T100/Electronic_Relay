#ifndef DEVICE_IMPLEMENTATION
#define DEVICE_IMPLEMENTATION

#include "init.h"
#include "lcd_driver.h"
#include "dtostr.h"
#include "relay.h"
#include "gpio_ex.h"
#include "keypad.h"
#include <stdio.h>
#include <string.h>
#include "ArrayListStatic.h"



int menuHeadPageHandler  (void *, int32_t);
int menuMainPage0Handler (void *, int32_t);
int menuMainPage1Handler (void *, int32_t);
int menuMainPage2Handler (void *, int32_t);
int menuMainPage3Handler (void *, int32_t);
int menuMainPage4Handler (void *, int32_t);

int menuSetupOnHandler (void *, int32_t);
int menuSetupOffHandler (void *, int32_t);

int menuSetupOvTopHandler (void *, int32_t);
int menuSetupOvBottHandler (void *, int32_t);

int menuExitHandler (void *p, int32_t param);
int menuSaveExitHandler (void *p, int32_t param);

int menuSetupClockHandler (void *, int32_t clock);

int menuAdjHandler (void *, int32_t clock);
int menuAdjKHandler (void *p, int32_t param);
int menuAdjOffsetHandler (void *p, int32_t param);

int menuInvertOutputHandler (void *p, int32_t param);

int menuSwitchDefaultRelayPresets (void *p, int32_t param);

Time &incrementTime (Time &time, uint8_t cursor);

template <int S, int M>
class MovingLine {
    private :
        char buf[M];
        int cursor;
    public :
        MovingLine() 
        {
            cursor = 0;
            memset(buf, ' ', M);
        }
        void moveRight ()
        {
            cursor++;
            if (cursor + S >= M) {
                cursor = 0;
            }
        }
        void reset ()
        {
            cursor = 0;
        }
        template <typename C>
        void print (C *input, int sector )
        {
            int sect = sector * S;
            if (sect >= M) {
                return;
            }
            for (int i = 0; i < S; i++) {
                buf[i + sect] = input[i];
            }
        }
    
        char *get ()
        {
            return buf + cursor;
        }
};

class MenuLeaf : public DefaultArrayListBaseNode<MenuLeaf> {
    private :
        int (*handler) (void *p, int32_t param);
        ArrayListBase<MenuLeaf> leafs;
        int8_t currentLeaf;
    public :
        MenuLeaf (int (*handler) (void *, int32_t )) : DefaultArrayListBaseNode<MenuLeaf>()
        {
            this->handler = handler;
            currentLeaf = -1;
        }
        
        MenuLeaf *getNext (MenuLeaf *head)
        {
            if (this->nextLink == nullptr) {
                return head->getFirstChild();
            }
            return this->nextLink;
        }
        MenuLeaf *getLeaf (MenuLeaf *parent)
        {
            if (this->leafs.isEmpty() == true) {
                return this;
            }
            return this->leafs.getFirst();
        }
        
        void *connectFirst (MenuLeaf *leaf)
        {
            currentLeaf = 0;
            this->leafs.addFirst(leaf);
        }
        void *connectLast (MenuLeaf *leaf)
        {
            currentLeaf = 0;
            this->leafs.addLast(leaf);
        }
        void disconnect (MenuLeaf *leaf)
        {
            this->leafs.remove(leaf);
            if (this->leafs.isEmpty() == true) {
                currentLeaf = -1;
            }
        }
        
        bool isLeaf ()
        {
            if (currentLeaf < 0) {
                return false;
            }
            return true;
        }
        
        bool ifLeafThenRun (void *p, int32_t param)
        {
           if (currentLeaf >= 0) {
               if (handler != nullptr) {
                   (*handler)(p, param); 
               }
               return true;
           } 
           return false;
        }
        void run (void *p, int32_t param)
        {
            if (handler != nullptr) {
                (*handler)(p, param); 
            }
        }
        
        MenuLeaf *getFirstChild ()
        {
            return this->leafs.getFirst();
        }
        
        uint32_t getChildCount ()
        {
            return this->leafs.size();
        }
        
};




template <int e>
class Message {
    private :
        bool contextChanged;
        char message[e];
        uint8_t line;
    public :
        Message (uint8_t line)
        {
            this->contextChanged = true;
            memset(message, '\0', e);
            this->line = line;
        }
        void operator () (char *message)
        {
            this->contextChanged = true;
            for (int i = 0; i < e && message[i] != '\0'; i++) {
               this->message[i] = message[i]; 
            }
            
        }
        virtual void __print (void *printer);
        
        void print (void *printer)
        {
            __print(printer);
            memset(message, '\0', e);
        }
        
        bool isChanged ()
        {
            return this->contextChanged;
        }
        uint8_t getLine ()
        {
            return this->line;
        }
        
        friend class Device;
    
};

char *getTempInCelsium (int32_t temp);
char *getPreciseValue (int32_t temp);

void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

template <char C, int8_t Max>
class Cursor {
    private :
        int8_t position;
        int8_t blink;
    public :
        Cursor()
        {
            blink = 0;
        }
        
        int8_t moveRight ()
        {
            position++;
            if (position > Max) {
                position = 0;
            }
        }
        int8_t moveLeft ()
        {
            position--;
            if (position < 0) {
                position = 0;
            }
        }
        
        int8_t getPosition ()
        {
            return position;
        }
        
        int8_t update (char *input)
        {
            blink ^= 1;
            if (blink) {
                input[position] = C;
            }
        }            
    
};

class Device {
    private :
        LCD controller;
        LcdInterface lcd; 
        
        char text[50];
        Time localTime;
        
        Pin<GPIO_TypeDef> *relayPin;
        
    
        MenuLeaf *headPage;
        MenuLeaf *currentPage;
        MenuLeaf *retPage;
    
    
    public : 

    friend void timer3_tick(void *p);    
    friend class Message<9>; 
    friend int menuExitHandler (void *p, int32_t param);
    friend int menuSaveExitHandler (void *p, int32_t param);
    
    Keypad<GPIO_TypeDef> *keypad = nullptr;
    Message<9> line_0;
    Message<9> line_1;
    Relay relay;
    Device () : relay(),
                line_0(0),
                line_1(1)
        {
            device_enable_clock();
            device_gpio_init();
            device_adc_init();
            device_timer_init();
            Delay(300);
            lcd(&controller, __4BitBus);
            resetTime(localTime);
            
            static Pin<GPIO_TypeDef> buttonL(BUTTON_L_PORT, BUTTON_L_PIN);
            static Pin<GPIO_TypeDef> buttonR(BUTTON_R_PORT, BUTTON_R_PIN);
            static Pin<GPIO_TypeDef> buttonC(BUTTON_C_PORT, BUTTON_C_PIN);
            static Pin<GPIO_TypeDef> relay(RELAY_PORT, RELAY_PIN);
            relayPin = &relay;
            static  Pin<GPIO_TypeDef> *buttons[3] = {
                &buttonL,
                &buttonR,
                &buttonC,
            };
            static Keypad<GPIO_TypeDef> kpd(buttons, "LRC", 3);
            
            static MenuLeaf head(menuHeadPageHandler);
            
            static MenuLeaf main0(menuMainPage0Handler);
            static MenuLeaf main1(menuMainPage1Handler);
            static MenuLeaf main2(menuMainPage2Handler);
            static MenuLeaf main3(menuMainPage3Handler);
            static MenuLeaf main4(menuMainPage4Handler);
            static MenuLeaf mainPresetChooser(menuSwitchDefaultRelayPresets);
            
            static MenuLeaf menuInvertOutput(menuInvertOutputHandler);
            
            
            static MenuLeaf setupOn(menuSetupOnHandler);
            static MenuLeaf setupOff(menuSetupOffHandler);
            static MenuLeaf setupOffOnRet(menuExitHandler);
            
            static MenuLeaf setupOvTop(menuSetupOvTopHandler);
            static MenuLeaf setupOvBot(menuSetupOvBottHandler);
            static MenuLeaf setupOvRet(menuExitHandler);
            
            static MenuLeaf setupClock(menuSetupClockHandler);
            
            static MenuLeaf menuAdj(menuAdjHandler);
            static MenuLeaf menuAdjK(menuAdjKHandler);
            static MenuLeaf menuAdjOffset(menuAdjOffsetHandler);
            static MenuLeaf menuAdjRet(menuSaveExitHandler);
           
            
            head.connectFirst(&menuInvertOutput);
            head.connectFirst(&main0);
            head.connectFirst(&main1);
            head.connectFirst(&main2);
            head.connectFirst(&main3);
            head.connectFirst(&main4);
            head.connectFirst(&menuAdj);
            head.connectFirst(&setupClock);
            head.connectFirst(&mainPresetChooser);
            
            
            main1.connectFirst(&setupOffOnRet);
            main1.connectFirst(&setupOn);
            main1.connectFirst(&setupOff);
            
            main4.connectFirst(&setupOvRet);
            main4.connectFirst(&setupOvTop);
            main4.connectFirst(&setupOvBot);
            
            menuAdj.connectFirst(&menuAdjRet);
            menuAdj.connectFirst(&menuAdjK);
            menuAdj.connectFirst(&menuAdjOffset);
            
            
            headPage = &head;
            currentPage = head.getFirstChild();
            retPage = headPage;
            
            keypad = &kpd;
            lcd.prints("_relay_", 0, 0);
            lcd.prints("<v_0.05>", 0, 1);
            Delay(1000);
            lcd.prints("09.01.16", 0, 0);
            Delay(500);
        }
        
        void run ()
        {
            int page = 1;
            static char clockMemo[16];
            for (;;) {
                
                clear();
                if (keypad->testKey('L') == true) {
                    currentPage = currentPage->getNext(retPage);
                }
                else if (keypad->testKey('L') == true) {
                    currentPage = currentPage->getNext(retPage);
                }
                switch (keypad->test().key) {
                    case 'L' :
                        break;
                    case 'C' :
                        retPage = currentPage;
                        currentPage = currentPage->getLeaf(headPage);
                        if (retPage == currentPage) {
                            retPage = headPage;
                        }
                        break;
                    case 'R' :
                        page = 3;
                        break;
                    
                    default :
                        break;
                    
                }
                currentPage->run(this, page);
                relay.updateControl();
                
                line_0.print(this);
                line_1.print(this);
                
                Delay(300);
            }
        }
        
        void tick ()
        {
            relay.tick();
            icreaseTime(localTime);
        }
        
        void clear ()
        {
            lcd.fillVisible();
        }
        
        void home ()
        {
            
        }
        
        void setCursor (uint8_t x = 0, uint8_t y = 0)  
        {
            lcd.setCursor(x, y);
        }
        
        void prints (char *s)
        {
            lcd.prints( s );
        }
        
        void prints (const char *s)
        {
            lcd.prints( s );
        }
        
        void prints (char *s, int8_t x, int8_t y)
        {
            lcd.prints( s, x, y );
        }
        
        void prints (const char *s, int8_t x, int8_t y)
        {
            lcd.prints( s, x, y );
        }
        
        void prints (int32_t value, int8_t x, int8_t y)
        {
            static char buf[12];
            memset(buf, '\0', 12);
            
            itoa(buf, value / 10, 10);
            lcd.prints(buf, x, y);
        }
        
        void prints (int32_t value)
        {
            static char buf[12];
            lcd.prints( itoa(buf, value, 10) );
        }
        
        Time &getLocalTime ()
        {
            static Time local = localTime;
            local = localTime;
            return local;
        }
        
        void setLocalTime (Time &time)
        {
            localTime = time;
        }
};


void timer3_tick(void *);

Time &clockAdjust (Device *device, Time *time, Cursor<'_', 8> &cursor);


#endif