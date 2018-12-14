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


/**
 * @brief YAS' tasks scheduler.
 */

#include "kernel/sched.h"
#include "kernel/global.h"
#include "kernel/task.h"
#include "kernel/panic.h"

#include "util/dlist.h"

void
__do_schedule(void)
{
    __reset_time_slice((struct task*)current_task);

    ((struct task*)current_task)->stack_pointer = stack_pointer;

    dlist_rotate_left(&running_queue);

    current_task = running_queue.next;

    stack_pointer = ((struct task*)current_task)->stack_pointer;
}

void
reschedule()
{
    asm volatile ("cli" ::: "memory");

    if (running_queue.next != running_queue.prev)
        do_schedule();

    asm volatile ("reti" ::: "memory");
}
