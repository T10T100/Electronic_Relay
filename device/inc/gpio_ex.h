#ifndef GPIO_LOW_IMPLEMENTATION
#define GPIO_LOW_IMPLEMENTATION
#include "main.h"

void gpio_init (GPIO_TypeDef *port, uint32_t pin, GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode, GPIOOType_TypeDef oType = GPIO_OType_PP, GPIOPuPd_TypeDef pupd = GPIO_PuPd_NOPULL);

void gpio_pin_set (GPIO_TypeDef  * port, uint32_t pin, bool value);
void gpio_pin_reset (GPIO_TypeDef *port, uint32_t pin);
void gpio_port_set (GPIO_TypeDef *port, uint32_t gPin, uint32_t value);
void gpio_port_reset (GPIO_TypeDef *port, uint32_t gPin);

int gpio_pin_read (GPIO_TypeDef *port, uint32_t gPin);



#endif

