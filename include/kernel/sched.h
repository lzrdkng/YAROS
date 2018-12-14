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

#ifndef SCHED_H
#define SCHED_H

#include "kernel/def.h"
#include "kernel/global.h"
#include "kernel/task.h"

#include "util/dlist.h"

/*
 * Thanks to RTOS. These macros should always be call in CLI context.
 */
#define SAVE_STACK								\
	asm volatile(								\
		"push r0 \n\t"							\
		"push r1 \n\t"							\
		"push r2 \n\t"							\
		"push r3 \n\t"							\
		"push r4 \n\t"							\
		"push r5 \n\t"							\
		"push r6 \n\t"							\
		"push r7 \n\t"							\
		"push r8 \n\t"							\
		"push r9 \n\t"							\
		"push r10 \n\t"							\
		"push r11 \n\t"							\
		"push r12 \n\t"							\
		"push r13 \n\t"							\
		"push r14 \n\t"							\
		"push r15 \n\t"							\
		"push r16 \n\t"							\
		"push r17 \n\t"							\
		"push r18 \n\t"							\
		"push r19 \n\t"							\
		"push r20 \n\t"							\
		"push r21 \n\t"							\
		"push r22 \n\t"							\
		"push r23 \n\t"							\
		"push r24 \n\t"							\
		"push r25 \n\t"							\
		"push r26 \n\t"							\
		"push r27 \n\t"							\
		"push r28 \n\t"							\
		"push r29 \n\t"							\
		"push r30 \n\t"							\
		"push r31 \n\t"							\
		"in r0, __SREG__\n\t"					\
		"push r0\n\t"							\
		)


#define SAVE_SP									\
	asm volatile(								\
		"in r26, __SP_L__ \n\t"					\
		"in r27, __SP_H__ \n\t"					\
		"sts stack_pointer, r26 \n\t"			\
		"sts stack_pointer+1, r27"				\
		)

#define RESTORE_SP								\
	asm volatile(								\
		"lds r26, stack_pointer \n\t"			\
		"lds r27, stack_pointer+1 \n\t"			\
		"out __SP_L__, r26 \n\t"				\
		"out __SP_H__, r27"						\
		)

#define RESTORE_STACK							\
	asm volatile(								\
		"pop r0 \n\t"							\
		"out __SREG__, r0\n\t"					\
		"pop r31 \n\t"							\
		"pop r30 \n\t"							\
		"pop r29 \n\t"							\
		"pop r28 \n\t"							\
		"pop r27 \n\t"							\
		"pop r26 \n\t"							\
		"pop r25 \n\t"							\
		"pop r24 \n\t"							\
		"pop r23 \n\t"							\
		"pop r22 \n\t"							\
		"pop r21 \n\t"							\
		"pop r20 \n\t"							\
		"pop r19 \n\t"							\
		"pop r18 \n\t"							\
		"pop r17 \n\t"							\
		"pop r16 \n\t"							\
		"pop r15 \n\t"							\
		"pop r14 \n\t"							\
		"pop r13 \n\t"							\
		"pop r12 \n\t"							\
		"pop r11 \n\t"							\
		"pop r10 \n\t"							\
		"pop r9 \n\t"							\
		"pop r8 \n\t"							\
		"pop r7 \n\t"							\
		"pop r6 \n\t"							\
		"pop r5 \n\t"							\
		"pop r4 \n\t"							\
		"pop r3 \n\t"							\
		"pop r2 \n\t"							\
		"pop r1 \n\t"							\
		"pop r0"								\
		)


/**
 * @brief Change current task running.
 *
 * This will reset the time slice of the current ask according to its
 * nice value. It will then put it at the end of the queue and select
 * the next one.
 *
 * @note It's mandatory to save the context before calling and
 * restoring the new context after returned.
 */
OS_MAIN OPTIMIZE("s") void
__do_schedule(void);

/*
 * Helper Macro that wrap __do_schedule with saving/restoring context.
 */
#define do_schedule()							\
	({											\
		SAVE_STACK;								\
		SAVE_SP;								\
		__do_schedule();						\
		RESTORE_SP;								\
		RESTORE_STACK;							\
	})

/**
 * @brief Call do_schedule without waiting for for interrupt. This is
 * useful when waiting for a polling event.
 */
void
reschedule();

INLINE void
__reset_time_slice(struct task *T)
{
    time_slice = TASK_MAX_NICE - T->nice;
}
#endif /* SCHED_H */
