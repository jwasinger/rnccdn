#include "sys/types.h"
#include "stdint.h"
#include "stdio.h"

int main()
{
  uint16_t test = (uint16_t)0x00000000ffffffff;
  printf("hello %d", test);
}