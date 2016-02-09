#include "device.h"
#include "gpio_ex.h"
#include <string.h>

uint32_t TimingDelay;

char *getTempInCelsium (int32_t temp)
{
    static char buf[12];
    memset(buf, '\0', 12);
            
    itoa(buf, temp / 10, 10);
    int i = strlen(buf);
    buf[i] = '.';
    utoa(buf + i + 1, (temp < 0 ? -temp : temp) % 10, 10);
    i = strlen(buf);
    buf[i++] = ' ';
    buf[i++] = 'C';
    buf[i++] = '*';
    return buf;
}
char *getAdjustments (int32_t temp)
{
    static char buf[12];
    memset(buf, '\0', 12);
            
    itoa(buf, temp / 1000, 10);
    int i = strlen(buf);
    buf[i] = '.';
    utoa(buf + i + 1, (temp < 0 ? -temp : temp) % 1000, 10);
    i = strlen(buf);
    buf[i++] = ' ';
    buf[i++] = '*';
    buf[i++] = '*';
    return buf;
}


void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

extern Keypad<GPIO_TypeDef> *keypad;

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
  if (TimingDelay % 100 == 0) {
      if (keypad != nullptr) {
          
          }
  }
}


float Relay::RelayReadAnalog ()
{
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    return (float) (0xFFF - (ADC_GetConversionValue(ADC1) * 3300)/0xFFF);
}

void Relay::RelayWriteControl (bool value)
{
    gpio_pin_set(RELAY_PORT, RELAY_PIN, value);
}

int32_t Relay::RelayReadManual ()
{
    return 0;
}


void LCD::command_next ()
{
    gpio_pin_reset(LCD_RS_PORT, LCD_RS_PIN);
    asm volatile ("nop \n\t nop \n\t");
}

void LCD::data_next ()
{
    gpio_pin_set(LCD_RS_PORT, LCD_RS_PIN, true);
    asm volatile ("nop \n\t nop \n\t");
}

void LCD::strobe ()
{
    asm volatile ("nop \n\t nop \n\t");
    gpio_pin_reset(LCD_E_PORT, LCD_E_PIN);
    asm volatile ("nop \n\t nop \n\t");
    gpio_pin_set(LCD_E_PORT, LCD_E_PIN, true);
    asm volatile ("nop \n\t nop \n\t");
    gpio_pin_reset(LCD_E_PORT, LCD_E_PIN);
    asm volatile ("nop \n\t nop \n\t");
}

void LCD::delay_ms (uint32_t ms)
{
   Delay(ms);
}

void LCD::data_on_bus (uint8_t data)
{
    gpio_pin_set(LCD_D4_PORT, LCD_D4_PIN, data & 0x01);
    gpio_pin_set(LCD_D5_PORT, LCD_D5_PIN, data & 0x02);
    gpio_pin_set(LCD_D6_PORT, LCD_D6_PIN, data & 0x04);
    gpio_pin_set(LCD_D7_PORT, LCD_D7_PIN, data & 0x08);
    asm volatile ("nop \n\t nop \n\t");
}

void timer3_tick(void *p)
{
    if (p == nullptr) {
        return;
    }
    Device *device = (Device *)p;
    device->tick();
}

template <int e>
void Message<e>::__print(void *printer)
{
    Device *device = (Device *)printer;
    device->lcd.fillLine(this->line);
    device->prints(this->message, 0, this->line);
}


int menuHeadPageHandler  (void *p, int32_t param)
{
    Device *device = (Device *)p;
    char clockMemo[12];
    
    device->line_0((char *)clockConverter(clockMemo, "hh:mm:ss", device->getLocalTime()));
    return 0;
}

int menuMainPage0Handler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    char clockMemo[12];
    device->line_0( (char *)clockConverter(clockMemo, "hh:mm:ss", device->getLocalTime()));
    device->line_1( (char *)device->relay.getActivity() );
    return 0;
}

int menuMainPage1Handler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    if (device->keypad->testKey('R') == true) {
        device->relay.setNextMode();
    }
    device->line_0((char *)"Mode : ");
    device->line_1((char *)device->relay.getMode());
    return 0;
}

int menuMainPage2Handler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    device->line_0((char *)getTempInCelsium (device->relay.getAnalogValue()));
    device->line_1((char *)device->relay.getStatus());
    return 0;
}

int menuMainPage3Handler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    if (device->keypad->testKey('C') == true) {
        device->relay.switchManual();
    }
    device->line_0((char *)device->relay.getMode());
    device->line_1((char *)device->relay.getStatus());
    return 0;
}

int menuMainPage4Handler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    device->line_0("Overlay");
    device->line_1("Settings");
    return 0;
}

Time &incrementTime (Time &time, uint8_t cursor)
{
    switch (cursor) {
        case 0:
            time.hours += 10;
            if (time.hours >= 24) {
                time.hours = 0;
            }
            break;
        case 1:
            time.hours += 1;
            if (time.hours >= 24) {
                time.hours = 0;
            }
            break;
        case 2:
            
            break;
        case 3:
            time.minutes += 10;
            if (time.minutes >= 60) {
                time.minutes = 0;
            }
            break;
        case 4:
            time.minutes += 1;
            if (time.minutes >= 60) {
                time.minutes = 0;
            }
            
            break;
        case 5:
            
            break;
        case 6:
            time.seconds += 10;
            if (time.seconds >= 60) {
                time.seconds = 0;
            }
            
            break;
        case 7:
            time.seconds += 1;
            if (time.seconds >= 60) {
                time.seconds = 0;
            }
            break;
        
        
    };
}

Time &clockAdjust (Device *device, Time &time, Cursor<'_', 8> &cursor)
{
    
    if (device->keypad->testKey('R') == true) {
        cursor.moveRight();
    } else if (device->keypad->testKey('C') == true) {
        incrementTime(time, cursor.getPosition());
        return time;
    }
    return time;
}

int menuSetupOffHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    char clockMemo[12];
    static float __analog = 1.0f;
    static int32_t pressCount = 0;
    
    static Cursor<'_', 8> cursor;
    static char mem[12];
    
    device->line_0("Bott/Off");
    switch (device->relay.getModeNum()) {
        case RelayTimeWindowMode:
            
            clockConverter(mem, "hh:mm:ss", device->relay.getOffTime());
            device->relay.programOffTime( clockAdjust( device, device->relay.getOffTime(), cursor) );
            cursor.update(mem);
            device->line_1(mem);
            break;
        case RelayAnalogWindowMode:
            device->line_1((char *)getTempInCelsium (device->relay.getOffValue()));
            if (device->keypad->testKey('R') == true) {
                device->relay.programOffValue(device->relay.getOffValue() + __analog);
                pressCount++;
                __analog += 0.1f;
            } else if (device->keypad->testKey('C') == true) {
                device->relay.programOffValue(device->relay.getOffValue() - __analog);
                pressCount++;
                __analog += 0.1f;
            } else {
                pressCount = 0;
                __analog = 1.0f;
            }
            __analog = (float)pressCount / 7 + __analog;
            break;
        case RelayManualMode :
            device->line_1((char *)device->relay.getStatus());
            break;
        case RelayOverlayMode :
            device->line_1((char *)device->relay.getOverlayLowStatus());
            break;
        case RelayMute:
            device->line_1((char *)"MUTED");
            break;
        default :
            
            break;
    }
    
    return 0;
}
int menuSetupOnHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    static Cursor<'_', 8> cursor;
    static char mem[12];
    device->line_0("Top / On");
    char clockMemo[12];
    static float __analog = 1.0f;
    static int32_t pressCount = 0;
    switch (device->relay.getModeNum()) {
        case RelayTimeWindowMode:
            clockConverter(mem, "hh:mm:ss", device->relay.getOnTime());
            device->relay.programOnTime( clockAdjust( device, device->relay.getOnTime(), cursor) );
            cursor.update(mem);
            device->line_1(mem);
            break;
        case RelayAnalogWindowMode:
            device->line_1((char *)getTempInCelsium (device->relay.getOnValue()));
            if (device->keypad->testKey('R') == true) {
                device->relay.programOnValue(device->relay.getOnValue() + __analog);
                pressCount++;
                __analog += 0.1f;
            } else if (device->keypad->testKey('C') == true) {
                device->relay.programOnValue(device->relay.getOnValue() - __analog);
                pressCount++;
                __analog += 0.1f;
            } else {
                pressCount = 0;
                __analog = 1.0f;
            }
            __analog = (float)pressCount / 7 + __analog;
            break;
        case RelayManualMode :
            device->line_1((char *)device->relay.getStatus());
            break;
        case RelayOverlayMode :
            device->line_1((char *)device->relay.getOverlayHighStatus());
            break;
        case RelayMute:
            device->line_1((char *)"MUTED");
            break;
    }
    
    return 0;
}
int menuSetupOvTopHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    device->line_0("Ov. Top:");
    if (device->keypad->testKey('C') == true) {
        device->relay.switchOverlayHigh();
    }
    device->line_1(device->relay.getOverlayHighStatus());
    
    return 0;
}
int menuSetupOvBottHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    device->line_0("Ov. Bot:");
    if (device->keypad->testKey('C') == true) {
        device->relay.switchOverlayLow();
    }
    device->line_1(device->relay.getOverlayLowStatus());
    return 0;
}

int menuSetupClockHandler (void *p, int32_t clock)
{
    Device *device = (Device *)p;
    static char mem[12];
    static Cursor<'_', 8> cursor;
    device->setLocalTime( clockAdjust( device, device->getLocalTime(), cursor) );
    device->line_0("time :");
    clockConverter(mem, "hh:mm:ss", device->getLocalTime());
    cursor.update(mem);
    device->line_1(mem);
    return 0;
}

int menuExitHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    static uint8_t sw = 0;
    if (device->keypad->testKey('R') == true) {
        sw ^= 1;
    }
    device->line_0((char *)"Exit ? ");
    if (sw) {
        device->line_1((char *)"Yes");
        if (device->keypad->testKey('C') == true) {
            device->currentPage = device->headPage;
        }
    } else {
        device->line_1((char *)"No");
        if (device->keypad->testKey('C') == true) {
            device->currentPage = device->retPage->getFirstChild();
        }
    }
    return 0;
}

int menuSaveExitHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    static uint8_t sw = 0;
    if (device->keypad->testKey('R') == true) {
        sw ^= 1;
    }
    device->line_0((char *)"Save ? ");
    if (sw) {
        device->line_1((char *)"Yes");
        if (device->keypad->testKey('C') == true) {
            device->currentPage = device->headPage;
        }
    } else {
        device->line_1((char *)"No");
        if (device->keypad->testKey('C') == true) {
            device->currentPage = device->retPage;
        }
    }
    return 0;
}

int menuAdjHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    device->line_0((char *)"Adjust");
    device->line_1((char *)"Settings");
    
    return 0;
}
int menuAdjKHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    device->line_0((char *)"Adj K :");
    
    if (device->keypad->testKey('R') == true) {
        device->relay.setAdjustmentK( device->relay.getAdjK() + 0.01 );
    } else if (device->keypad->testKey('C') == true) {
        device->relay.setAdjustmentK( device->relay.getAdjK() - 0.01 );
    }
    device->line_1( getAdjustments( device->relay.getAdjK() * 100 ) );
    return 0;
}

int menuAdjOffsetHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    device->line_0((char *)"Adj Off:");
    if (device->keypad->testKey('R') == true) {
        device->relay.setAdjustmentOffset( device->relay.getAdjOffset() + 2 );
    } else if (device->keypad->testKey('C') == true) {
        device->relay.setAdjustmentOffset( device->relay.getAdjOffset() - 2 );
    }
    device->line_1( getAdjustments( device->relay.getAdjOffset() * 1000 ) );
    return 0;
}

int menuInvertOutputHandler (void *p, int32_t param)
{
    Device *device = (Device *)p;
    device->line_0((char *)"Output:");
    if (device->keypad->testKey('C') == true) {
        device->relay.switchInversion();
    }
    device->line_1( (char *)device->relay.getInvertedStatus() );
    return 0;
}

int menuSwitchDefaultRelayPresets (void *p, int32_t param)
{
    Device *device = (Device *)p;
    static MovingLine<8, 48> movingLine;
    device->line_0((char *)"Preset:");
    if (device->keypad->testKey('C') == true) {
        device->relay.stashCurrentPreset();
        device->relay.loadNextDefaultPreset();
        movingLine.print( (char *)device->relay.getPresetInfo(), 0 );
        movingLine.reset();
    } else if (device->keypad->testKey('R') == true) {
        device->relay.unstashCurrentPreset();
        device->line_1( "Unstash" );
    } else {
        movingLine.print( (char *)device->relay.getPresetInfo(), 0 );
    }
    movingLine.print( "Mode : ", 1);
    movingLine.print( device->relay.getMode(), 2);
    movingLine.print( device->relay.getActivity(), 3);
    movingLine.print( "****", 4 );
    movingLine.moveRight();
    device->line_1( movingLine.get() );
    return 0;
}