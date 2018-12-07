

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

#include "YAROS/panic.h"
#include "YAROS/sched.h"
#include "devices/usart.h"

#include <avr/pgmspace.h>

#if 0
const char reg_fmt[] PROGMEM =
"  0  1  2  3\n"
" +--+--+--+--+\n"
"0|%-2X|%-2X|%-2X|%-2X|\n"
" +--+--+--+--+\n"
"1|%-2X|%-2X|%-2X|%-2X|\n"
" +--+--+--+--+\n"
"2|%-2X|%-2X|%-2X| SREG: %-2X\n"
" +--+--+--+\n"
"3|%-2X|%-2X|%-2X|\n"
" +--+--+--+\n"
"4|%-2X|%-2X|%-2X|\n"
" +--+--+--+\n"
"5|%-2X|%-2X|%-2X|\n"
" +--+--+--+\n"
"6|%-2X|%-2X|%-2X|\n"
" +--+--+--+\n"
"7|%-2X|%-2X|%-2X|\n"
" +--+--+--+\n"
"8|%-2X|%-2X|%-2X|\n"
" +--+--+--+\n"
"9|%-2X|%-2X|%-2X|\n"
" +--+--+--+\n";
#endif

const char kernel_panic[] PROGMEM  = "KERNEL PANIC\n";


#include <string.h>
#include <avr/interrupt.h>

static inline void __attribute__((always_inline))
__kpanic()
{
    char buff[80];

    strcpy_P(buff, kernel_panic);

    write_usart(0, buff, strlen(buff));
}

void
kpanic()
{
    cli();

    /* r0 and r1 are */

    /* Change stack pointer to bottom */
    asm volatile (
        "ldi r26, hi8(" xstr(RAMEND) ")\n\t"
        "out __SP_H__, r26\n\t"
        "ldi r26, lo8(" xstr(RAMEND) ")\n\t"
        "out __SP_L__, r26\n\t"
        );

    SAVE_STACK;

    __kpanic();

    while (1)
        ;
}


