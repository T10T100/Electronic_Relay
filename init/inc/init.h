#ifndef STARTUP_INIT
#define STARTUP_INIT

#include "main.h"

enum DevicePINS {
    LCD_RS_PIN      = GPIO_Pin_10,
    LCD_E_PIN       = GPIO_Pin_9,
    LCD_D4_PIN      = GPIO_Pin_1,
    LCD_D5_PIN      = GPIO_Pin_7,
    LCD_D6_PIN      = GPIO_Pin_6,
    LCD_D7_PIN      = GPIO_Pin_5,
    
    BUTTON_L_PIN    = GPIO_Pin_2,
    BUTTON_C_PIN    = GPIO_Pin_3,
    BUTTON_R_PIN    = GPIO_Pin_4,
    
    ADC_IN0_PIN = GPIO_Pin_0,
    RELAY_PIN  = GPIO_Pin_1,
};


#define LCD_RS_PORT     GPIOA
#define LCD_E_PORT      GPIOA
#define LCD_D4_PORT     GPIOB
#define LCD_D5_PORT     GPIOA
#define LCD_D6_PORT     GPIOA
#define LCD_D7_PORT     GPIOA
#define ADC_IN0_PORT    GPIOA

#define BUTTON_L_PORT GPIOA
#define BUTTON_C_PORT GPIOA
#define BUTTON_R_PORT GPIOA

#define RELAY_PORT GPIOA


void device_gpio_init ();
void device_enable_clock();
void device_adc_init ();
void device_timer_init ();


#endif

