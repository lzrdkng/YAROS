/*
 * Copyright (C) 2018 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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

#include "kernel/def.h"
#include "kernel/global.h"
#include "kernel/panic.h"
#include "kernel/task.h"
#include "kernel/sched.h"
#include "devices/irq.h"

#include "init/init.h"

#include "util/dlist.h"

void
run_kernel()
{
    struct task *T;

    enable_irq(IRQ_TIMER0_OVF);
    /* If no task is scheduled for running */
    if (dlist_is_empty(&running_queue)) {
#if defined(TODO_NO_PANIC) && 0
        asm volatile("sei" ::: "memory");
        while (dlist_is_empty(&running_queue))  /* Wait for event to wake up task */
            ;
        asm volatile("cli" ::: "memory");
#endif
        kpanic();
    }

    /*
     * Bootstrap the first task
     */
    T = dlist_first_entry(&running_queue, struct task, self);

    stack_pointer = T->stack_pointer;
    current_task = &T->self;

    __reset_time_slice((struct task *)current_task);

    RESTORE_SP;
    RESTORE_STACK;

    asm volatile("reti" ::: "memory");
}
