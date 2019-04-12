/*
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * kernel/sched.c - Round robin scheduler.
 */

#include "kernel/sched.h"
#include "kernel/global.h"
#include "kernel/task.h"
#include "kernel/panic.h"
#include "util/list.h"

/**
 * __do_schedule() - This is where the magic happen.
 *
 * This procedure should never be call directly.  Only through the
 * do_schedule() macro.  This is to ensure that the stack pointer and
 * registers were save.
 *
 */
void __do_schedule(void)
{
	((struct task*)current_task)->stack_pointer = stack_pointer;

	list_rotate_left(&running_queue);

	current_task = running_queue.next;

	stack_pointer = ((struct task*)current_task)->stack_pointer;

	__reset_time_slice((struct task*)current_task);
}

/**
 * __reschedule() - Internal of reschedule()
 *
 * Force rescheduling of current task, if there's other task to run.
 * If the calling task is the only task running, nothing happen and
 * return imedialty to the caller.
 *
 * This should NEVER be call in non premptive context.
 */
void reschedule()
{
	cli();

	if (running_queue.next != running_queue.prev)
		do_schedule();

	reti();
}
