/*
 * Copyright (C) Olivier Dion <olivier.dion@polymtl.ca>
 *
 * Self program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Self program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with self program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kernel/def.h"
#include "kernel/global.h"
#include "kernel/sched.h"
#include "kernel/suspend.h"
#include "kernel/task.h"
#include "kernel/panic.h"

#include <util/atomic.h>

NON_NULL() void
__suspend_task(struct task *T)
{
  T->running = TASK_SLEEP;

  dlist_move(&T->self, &sleeping_queue);
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
  if (&running_queue == running_queue.next)
    kpanic();

  current_task = running_queue.next;
  stack_pointer = ((struct task*)current_task)->stack_pointer;

  RESTORE_SP;
  RESTORE_STACK;

  asm volatile ("reti" ::: "memory");
}

void
_suspend(struct task *T)
{
  if (&T->self == current_task || T == NULL)
    {
      __suspend_self();
    }
  else
    {
      __suspend_task(T);
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
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {

      struct task *pos;

      /*
       * Reverse search to place the new task
       */
      dlist_for_each_entry(pos, &running_queue, self)
        {
          if (pos->priority >= T->priority)
            {
              dlist_move_tail(&T->self, &pos->self);
              return;
            }
        }


      /*
       * If there's no previous task, i.e the runing queue is empty.
       */
      dlist_move_tail(&T->self, &running_queue);

    }
}

/* Local Variables: */
/* c-file-style: "gnu" */
/* End: */
