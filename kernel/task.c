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


#include "kernel/def.h"
#include "kernel/global.h"
#include "kernel/panic.h"
#include "kernel/sched.h"
#include "kernel/task.h"


#include "util/dlist.h"


#include <string.h>             /* For memset */
#include <util/atomic.h>        /* For Atomic Block */


/**
 * This need to be not optimize. I don't know why, but at other levels
 * of optimization, GCC just fuck up things and corrupt the memory
 * of the task's stack when using multiple tasks somehow.
 */
static NO_INLINE NO_OPTIMIZE void
prepare_stack(struct task *T,
              taskfunc func,
              void *data)
{
    /*
     * The stack of the task is now:
     *
     * The *data* parameter is put in the stack at position
     * R25-R24. This is because of the calling convention of
     * avr-gcc.
     *
     * See: https://gcc.gnu.org/wiki/avr-gcc#Calling_Convention
     *
     * Top: Low Address
     * |       0        | : offset = 0
     * |       .        |
     * |       .        |
     * |       .        |
     * |       .        | <-- Stack Pointer of the Task : offset = size - 38
     * |       .        | <-- Saved SREG
     * |       .        | <-- [R31-R26]
     * |    hi8(data)   | <-- R25 : offset = size - 30
     * |    lo8(data)   | <-- R24
     * |       .        | <-- [R23-R0]
     * |    hi8(func)   | <-- Entry Point : offset = size - 4
     * |    lo8(func)   |
     * | hi8(kill_self) | <-- Return To Caller : offset =  size - 2
     * | lo8(kill_self) |
     * Bottom: High Address
     */

    /* lo8(kill_self) */
    T->stack[T->size - 1] = (U8)(((U16)kill_self) & 0xFF);

    /* hi8(kill_self) */
    T->stack[T->size - 2] = (U8)(((U16)kill_self) >> 8);

    /* lo8(func) */
    T->stack[T->size - 3] = (U8)(((U16)func) & 0xFF);

    /* hi8(func) */
    T->stack[T->size - 4] = (U8)(((U16)func) >> 8);

    /* lo8(data) */
    T->stack[T->size - 29] = (U8)(((U16)data) & 0xFF);

    /* hi8(data) */
    T->stack[T->size - 30] = (U8)(((U16)data) >> 8);
}


/**
 * @brief Free the resource of a task and remove it from any queue.
 *
 * This is an helper for the kill_self function.
 *
 * @param T The task to free.
 */
static inline NON_NULL()  void
__free_task(struct task *T)
{
    T->running = TASK_SLEEP;
    dlist_del(&(T->self));
}


struct task*
init_task(struct task *T,
          taskfunc func,
          void *data)
{
    /*
     * Initialize the linked list.
     */
    INIT_DLIST(&T->self);

    /*
     * Clear the stack
     */
    memset(T->stack,
           0x00,
           T->size);

    /*
     * Push kill_self and func on the task's stack.
     */
    prepare_stack(T, func, data);

    /*
     * Move the stack pointer on top of what was pushed + registers
     * and SREG.
     */
    T->stack_pointer = (U16*)(T->stack + T->size - 38);

    /*
     * Put the task to sleep
     */
    suspend(T);

    return T;
}

void
kill_self(void)
{
    /*
     * Entering Atomic operation.
     */
    asm volatile("cli" ::: "memory");

    /*
     * We remove ourself from any list.
     */
    __free_task((struct task*)current_task);

    current_task = running_queue.next;

    if (current_task == &running_queue) {
        /* No more task; End of time */
        kpanic();
    }

    stack_pointer = ((struct task*)current_task)->stack_pointer;

    RESTORE_SP;

    /*
     * Restore the context
     */
    RESTORE_STACK;

    /*
     * End of Atomic operation. Return to the scheduled task.
     */
    asm volatile("reti" ::: "memory");
}

void
kill_task(struct task *T)
{
    /*
     * Entering Atomic operation.
     */
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

        if (&T->self == current_task || T == NULL)
            kill_self();

        __free_task(T);
    }
}
