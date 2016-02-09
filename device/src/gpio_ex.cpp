#include "gpio_ex.h"


void gpio_init (GPIO_TypeDef *port, uint32_t pin, GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode, GPIOOType_TypeDef oType, GPIOPuPd_TypeDef pupd)
{
  GPIO_InitTypeDef GPIO_InitStructure;    
    
  GPIO_InitStructure.GPIO_Pin = pin;
  GPIO_InitStructure.GPIO_Mode = mode;
  GPIO_InitStructure.GPIO_OType = oType;
  GPIO_InitStructure.GPIO_Speed = speed;
  GPIO_InitStructure.GPIO_PuPd = pupd;
  GPIO_Init(port, &GPIO_InitStructure);    

}

void gpio_pin_set (GPIO_TypeDef *port, uint32_t pin, bool value)
{
    if (value == true) {
        port->BSRR = pin;
    } else {
        port->BRR = pin;
    }
}
void gpio_pin_reset (GPIO_TypeDef *port, uint32_t pin)
{
    port->BRR = pin;
}
void gpio_port_set (GPIO_TypeDef *port, uint32_t gPin)
{
    port->BSRR = gPin;
}
void gpio_port_reset (GPIO_TypeDef *port, uint32_t gPin)
{
    port->BRR = gPin;
}

void gpio_port_out (GPIO_TypeDef *port, uint32_t gPin)
{
    port->ODR = gPin;
}

int gpio_pin_read (GPIO_TypeDef *port, uint32_t pin)
{
    return (port->IDR & pin ? 1  : 0);
}


