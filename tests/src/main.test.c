#include "devices/usart.h"

int
main(int argc, char *argv[])
{
  static const char str[] = "Hello World!";

  init_usart(0);

  asm volatile ("sei" ::: "memory");

  while (1)
      write_usart(0, str, sizeof(str));

  return 0;
}
