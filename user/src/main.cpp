#include "device.h"


int adc_data;
void *__device = nullptr;

int main ()
{
    
    
    
    Device device;
    __device = &device;
    Delay(500);
    while (1) {
        device.run();
    }
    
    return 0;
}