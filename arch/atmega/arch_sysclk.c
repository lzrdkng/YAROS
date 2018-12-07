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

#include "YAROS/def.h"
#include "YAROS/global.h"
#include "YAROS/sched.h"
#include "YAROS/sysclk.h"
#include "YAROS/msg.h"

#include "devices/clk.h"
#include "devices/irq.h"

#include <stddef.h>

/*
 * Save on Stack r24, r25, SREG
 */
#define save_minimal_context() \
    asm volatile ("push r24\n\t" \
                  "push r25\n\t" \
                  "in r24, __SREG__\n\t"       \
                  "push r24\n\t" \
                  )

#define restore_minimal_context() \
    asm volatile ("pop r24\n\t" \
                  "out __SREG__, r24\n\t"      \
                  "pop r25\n\t" \
                  "pop r24\n\t" \
                  )

INLINE void
increment_jiffies()
{
    asm volatile (
        /*
         * We need to to adjust the initial value of TCNT0 with the
         * 7 previous cycles and the next 2.
         */
        "ldi r24, " xstr(KCLK_COUNT + (9 / KCLK_SCALE)) "\n\t"
        "out 0x26, r24\n\t"

        /* Load jiffies lower 16 bits */
        "lds r24, jiffies\n\t"
        "lds r25, jiffies+1\n\t"

        /* ++jiffies */
        "adiw r24, 1\n\t"
        "sts jiffies, r24\n\t"
        "sts jiffies+1, r25\n\t"
        "lds r24, jiffies+2\n\t"
        "adc r24, r1\n\t"
        "sts jiffies+2, r24\n\t"
        "lds r24, jiffies+3\n\t"
        "adc r24, r1\n\t"
        "sts jiffies+3, r24\n\t"

#  if TIMER0_LATENCY != 0

        /*
         * If the timer as a latency because of HZ, we have to
         * increment TCNT0 every TIMER0_LATENCY interrupts. See the
         * table in clock.h
         */

        /* Load jiffies in r24 */
        "lds r24, jiffies\n\t"


        /* if (jiffies % TIMER0_LATENCY == 0) */
        /* BRANCH .restore if != 0 */
        "andi r24, " xstr(TIMER0_LATENCY - 1) "\n\t"
        "brne .restore\n\t"

        /* ++TCNT0 */
        "in r24, 0x26\n\t"
#  if TIMER0_SCALE <= 29        /* Same reason has before */
        "subi r24, lo8(-(2))\n\t"
#  else
        "subi r24, lo8(-(1))\n\t"
#  endif
        "out 0x26, r24\n"


#  endif    /* TIMER0_LATENCY */
        ".restore:\n\t"
        //  "ret"
        );
}

/**
 * @file sysclkc.c
 *
 * @brief System Clock for the ATmega324PA.
 *
 * An optimization could be made. Instead of using the timer overflow
 * vector, it would be possible to use one of the compare units
 * (TIMER0_COMPA_vect or TIMER0_COMPB_vect) and put the timer in CTC
 * mode. This would save the 2 instructions of reseting the TCNT0 to
 * the appropriate value, because the hardware will do it for
 * us. Therefore, it would be possible to save 2 cycles and 4 bytes of
 * memory.
 *
 * However, I'll rather use the timer overflow instead, leaving the
 * two compare units free for something else.
 */
ISR(TIMER0_OVF_vect, ISR_NAKED)
{
    save_minimal_context();

    increment_jiffies();

    asm volatile ("lds r24, time_slice\n\t"
                  "cpi r24, 0\n\t"
                  "breq .schedule\n\t"
                  "jmp .end\n"); /* Skip scheduling */

    asm volatile(".schedule:\n");

    restore_minimal_context();

    do_schedule();

    reti();

    /* If not scheduling, decrement time_slice */
    asm volatile (".end:\n\t"
                  "subi r24, 1\n\t"
                  "sts time_slice, r24\n\t");

    restore_minimal_context();

    reti();
}

/**
 * @brief Initialize the System Clock.
 *
 * This routine initialize the Timer 0 for the System Clock.
 */
void
kinit_sysclk(void)
{
    if (init_clk(KCLK,
                 CLK_NORMAL,
                 KCLK_SRC,
                 0) != OK) {
        FATAL("kinit_sysclk");
    }
}
