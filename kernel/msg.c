/*
 * Copyright (C) Olivier Dion <olivier.dion@polymtl.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <stdio.h>

#include "kernel/msg.h"
#include "devices/usart.h"

#if KERNEL_VERBOSE_LEVEL >= 5
const char DEBUG_HEADER[] PROGMEM = GREEN("[DEBUG] ");

void
__send_debug_header()
{
     char buff[ARRAY_SIZE(DEBUG_HEADER)];

     memcpy_P(buff, &DEBUG_HEADER[0], ARRAY_SIZE(DEBUG_HEADER));

     print_kernel(buff);
}
#endif

#if KERNEL_VERBOSE_LEVEL >= 4
const char INFO_HEADER[] PROGMEM = PURPLE("[INFO] ");

void
__send_info_header()
{
     char buff[ARRAY_SIZE(INFO_HEADER)];

     memcpy_P(buff, &INFO_HEADER[0], ARRAY_SIZE(INFO_HEADER));

     print_kernel(buff);
}
#endif


#if KERNEL_VERBOSE_LEVEL >= 3
const char WARNING_HEADER[] PROGMEM = YELLOW("[WARNING] ");

void
__send_warning_header()
{
     char buff[ARRAY_SIZE(WARNING_HEADER)];

     memcpy_P(buff, &WARNING_HEADER[0], ARRAY_SIZE(WARNING_HEADER));

     print_kernel(buff);
}
#endif


#if KERNEL_VERBOSE_LEVEL >= 2
const char ERROR_HEADER[] PROGMEM = RED("[ERROR] ");

void
__send_error_header()
{
     char buff[ARRAY_SIZE(ERROR_HEADER)];

     memcpy_P(buff, &ERROR_HEADER[0], ARRAY_SIZE(ERROR_HEADER));

     print_kernel(buff);

}
#endif

#if KERNEL_VERBOSE_LEVEL >= 1
const char FATAL_HEADER[] PROGMEM = LIGHT_RED("[FATAL] ");
const char NL[] PROGMEM = {'\n'};

void
__send_fatal_header()
{
     char buff[ARRAY_SIZE(FATAL_HEADER)];

     memcpy_P(buff, &FATAL_HEADER[0], ARRAY_SIZE(FATAL_HEADER));

     print_kernel(buff);
}

void
__send_nl()
{
     char buff[ARRAY_SIZE(NL)];

     memcpy_P(buff, &NL[0], ARRAY_SIZE(NL));

     print_kernel(buff);
}

static int
printk(const char *fmt, ...)
{
     char buff[16];

     va_list ap;

     va_start(ap, fmt);

     snprintf(buff, ARRAY_SIZE(buff), fmt, ap);

     write_usart(0, buff, strlen(buff));

     return 16;
}

static int
putck(char c)
{
     write_usart(0, &c, 1);

     return 1;
}

int __attribute__((weak, alias("printk"), format(printf, 1, 2)))
print_kernel(const char *fmt, ...);

int __attribute__((weak, alias("putck")))
putc_kernel(char c);

#endif
