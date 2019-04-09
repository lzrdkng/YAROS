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

#include <util/atomic.h>

#include "kernel/def.h"
#include "kernel/errno.h"

#include "ipc/mutex.h"

error_t
try_lock_mutex(volatile struct mutex *mutex)
{
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                if (mutex->owner == NULL) {
                        mutex->owner = current_task;
                        return OK;
                }

                if (mutex->owner == current_task)
                        return OK;
        }

        return -EBUSY;
}

volatile struct mutex *
lock_mutex(volatile struct mutex *mutex)
{
        while (try_lock_mutex(mutex) != OK)
                reschedule();

        return mutex;
}

error_t
unlock_mutex(volatile struct mutex *mutex)
{
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                if (mutex->owner == current_task) {
                        mutex->owner = NULL;
                        return OK;
                }
        }

        return -EBUSY;
}
