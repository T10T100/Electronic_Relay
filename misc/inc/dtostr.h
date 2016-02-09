#ifndef __DTOSTR
#define __DTOSTR

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char *reverseChars (char *charSequence);
char *itoa (char *buffer, int32_t value, int32_t base);
char *utoa (char *buffer, uint32_t value, int32_t base);


#endif


