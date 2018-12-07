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
#include "YAROS/suspend.h"
#include "YAROS/task.h"

#include <util/atomic.h>

NON_NULL() void
__suspend_task(struct task *T)
{
    T->running = TASK_SLEEP;

    dlist_move(&T->this, &sleeping_queue);
}

OS_MAIN OPTIMIZE("s") void
__suspend_self()
{
    __suspend_task((struct task*)current_task);

    /* Save Context before switch */
    SAVE_STACK;
    SAVE_SP;
    ((struct task*)current_task)->stack_pointer = stack_pointer;

    /* Switching context */
    current_task = running_queue.next;
    stack_pointer = ((struct task*)current_task)->stack_pointer;

    RESTORE_SP;
    RESTORE_STACK;

    asm volatile ("reti" ::: "memory");
}

void
suspend(struct task *T)
{
    /*
     * Entering Atomic operation
     */
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

        if (&T->this == current_task || T == NULL) {
            __suspend_self();
        }
        else {
            __suspend_task(T);
        }
    }
}


void
resume(struct task *T)
{
    if (!IS_SLEEPING(T))
        return;
    /*
     * Remove task from the sleeping queue, change its status and put
     * it in the running queue.
     */
    T->running = TASK_RUN;

    /*
     * Block context switch briefly
     */
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

        struct task *pos;

        /*
         * Reverse search to place the new task
         */
        dlist_for_each_entry(pos, &running_queue, this) {
            if (pos->priority >= T->priority) {
                dlist_move_tail(&T->this, &pos->this);
                return;
            }
        }


        /*
         * If there's no previous task, i.e the task queue is empty.
         */
        dlist_move_tail(&T->this, &running_queue);

    }
}
