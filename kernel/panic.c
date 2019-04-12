

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


#include <string.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "kernel/panic.h"
#include "kernel/global.h"
#include "kernel/task.h"
#include "kernel/sysclk.h"
#include "devices/usart.h"

static const char kernel_panic[] PROGMEM  = "\nKERNEL PANIC\n";

static void
__core_dumb(S16 infos)
{
     char buff[32];

     struct task *T;

     U8 n_running = 0;
     U8 n_sleeping = 0;

     list_for_each_entry(T, &running_queue, self) {
          ++n_running;
     }

     list_for_each_entry(T, &sleeping_queue, self) {
          ++n_sleeping;
     }


     /* KERNEL PANIC */
     strcpy_P(buff, kernel_panic);
     write_usart(0, buff, strlen(buff));

     /* Infos */
     sprintf(buff, "\nInfos:%d\n", infos);
     write_usart(0, buff, strlen(buff));


     sprintf(buff, "\nRunning:%d\n", n_running);
     write_usart(0, buff, strlen(buff));

     /* Running Tasks */
     list_for_each_entry(T, &running_queue, self) {

          if (&(T->self) == current_task)
               sprintf(buff, "\n\tTask 0x%x (*)\n", (U16)T);
          else
               sprintf(buff, "\n\tTask 0x%x\n", (U16)T);

          write_usart(0, buff, strlen(buff));

          for (int i=33; i>1; --i) {
               sprintf(buff, "\t\tr%d\t0x%x\t%d\n",
                       33-i,
                       T->stack_pointer[i],
                       T->stack_pointer[i]);
               write_usart(0, buff, strlen(buff));
          }

          /* SREG */
          sprintf(buff, "\t\tSREG\t0x%x\t%d\n",
                  T->stack_pointer[1],
                  T->stack_pointer[1]) ;
          write_usart(0, buff, strlen(buff));
     }

     sprintf(buff, "\nSleeping:%d\n", n_sleeping);
     write_usart(0, buff, strlen(buff));

     /* Sleeping Tasks */
     list_for_each_entry(T, &sleeping_queue, self) {

          sprintf(buff, "\n\tTask 0x%x\n", (U16)T);
          write_usart(0, buff, strlen(buff));
          for (int i=33; i>1; --i) {
               sprintf(buff, "\t\tr%d\t0x%x\t%d\n",
                       33-i,
                       T->stack_pointer[i],
                       T->stack_pointer[i]);
               write_usart(0, buff, strlen(buff));
          }

          /* SREG */
          sprintf(buff, "\t\tSREG\t0x%x\t%d\n",
                  T->stack_pointer[1],
                  T->stack_pointer[1]) ;
          write_usart(0, buff, strlen(buff));
     }
}

void __attribute__((weak, alias("__core_dumb")))
recover_kernel(S16 infos);

void
_panic_kernel(S16 infos)
{
     asm volatile(
          "pop r0\n\t"          /* return address lo8 */
          "pop r0\n\t"          /* return address hi8 */
          "pop r0 \n\t"         /* r24 */
          "pop r1 \n\t"         /* r25 */
          "push r2 \n\t"
          "push r3 \n\t"
          "push r4 \n\t"
          "push r5 \n\t"
          "push r6 \n\t"
          "push r7 \n\t"
          "push r8 \n\t"
          "push r9 \n\t"
          "push r10 \n\t"
          "push r11 \n\t"
          "push r12 \n\t"
          "push r13 \n\t"
          "push r14 \n\t"
          "push r15 \n\t"
          "push r16 \n\t"
          "push r17 \n\t"
          "push r18 \n\t"
          "push r19 \n\t"
          "push r20 \n\t"
          "push r21 \n\t"
          "push r22 \n\t"
          "push r23 \n\t"
          "push r0 \n\t"        /* r24 */
          "push r1 \n\t"        /* r25 */
          "push r26 \n\t"
          "push r27 \n\t"
          "push r28 \n\t"
          "push r29 \n\t"
          "push r30 \n\t"
          "push r31 \n\t"
          "in r0, __SREG__\n\t"
          "push r0\n\t"
          );

     /* Change stack pointer to end of RAM */
     asm volatile(
          "in r26, __SP_L__\n\t"
          "in r27, __SP_H__\n\t"
          "sts stack_pointer, r26\n\t"
          "sts stack_pointer+1, r27\n\t"
          "ldi r26, lo8(" xstr(RAMEND) ")\n\t"
          "ldi r27, hi8(" xstr(RAMEND) ")\n\t"
          "out __SP_L__, r26\n\t"
          "out __SP_H__, r27\n\t"
          );

     ((struct task *)current_task)->stack_pointer = stack_pointer;

     asm volatile (
          "clr r0 \n\t"
          "clr r1 \n\t"
          "clr r2 \n\t"
          "clr r3 \n\t"
          "clr r4 \n\t"
          "clr r5 \n\t"
          "clr r6 \n\t"
          "clr r7 \n\t"
          "clr r8 \n\t"
          "clr r9 \n\t"
          "clr r10 \n\t"
          "clr r11 \n\t"
          "clr r12 \n\t"
          "clr r13 \n\t"
          "clr r14 \n\t"
          "clr r15 \n\t"
          "clr r16 \n\t"
          "clr r17 \n\t"
          "clr r18 \n\t"
          "clr r19 \n\t"
          "clr r20 \n\t"
          "clr r21 \n\t"
          "clr r22 \n\t"
          "clr r23 \n\t"
          /* r24 */
          /* r25 */
          "clr r26 \n\t"
          "clr r27 \n\t"
          "clr r28 \n\t"
          "clr r29 \n\t"
          "clr r30 \n\t"
          "clr r31 \n\t"
          "out __SREG__, r0\n\t"
          );

     recover_kernel(infos);

     /* Wait for Watchdog */
     while (1)
          ;
}
