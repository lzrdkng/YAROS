/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 */



#include "kernel/def.h"
#include "kernel/global.h"
#include "kernel/panic.h"
#include "kernel/task.h"
#include "kernel/sched.h"
#include "devices/irq.h"

#include "init/init.h"

#include "util/dlist.h"

extern void init_sysclk(void);

void init_kernel()
{
	init_sysclk();
}

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
        panic_kernel(0);
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
