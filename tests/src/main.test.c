#include "devices/usart.h"

int
main(int argc, char *argv[])
{
  static const char str[] = "Hello World!";

  init_usart(0);

  write_usart(0, str, sizeof(str));

  while (1)
    ;

  return 0;
}
