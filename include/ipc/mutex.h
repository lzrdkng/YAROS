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

#ifndef YAROS_MUTEX_H
#define YAROS_MUTEX_H

#include "kernel/sched.h"

#include <util/atomic.h>

/**
 * struct mutex my_mutex = INIT_MUTEX;
 */
#define INIT_MUTEX {NULL}

struct mutex {
        void *owner;
};


/*
 * @brief Try to acquire a mutex.
 *
 * @param mutex The mutex to acquire
 *
 * @return OK on success, -EBUSY if mutex is already acquired
 *
 * @warning This should never be call in ISR
 */
error_t try_lock_mutex(volatile struct mutex *mutex);

/*
 * @brief Loop until mutex is acquire.
 *
 * @param mutex The mutex to acquire
 *
 * @note Once return, it's guaranteed that the caller has acquired the
 * mutex.
 *
 * @warning This should never be call in ISR
 */
volatile struct mutex * lock_mutex(volatile struct mutex *mutex);


/*
 * @brief Release a mutex.
 *
 * @mutex: The mutex to release
 *
 * @return OK on success, -EBUSY if the task trying to release the
 * mutex doesn't own it.
 */
error_t unlock_mutex(volatile struct mutex *mutex);

static inline void cleanup_mutex(volatile struct mutex **mutex)
{
        /* Cleanup is when for returning from the block.  But if we get
         * out of the block naturally, we don't want to cleanup a NULL pointer*/
        if (*mutex) {
                unlock_mutex(*mutex);
                reschedule();
        }
}

#define WITH_LOCK(MUTEX) \
        for (volatile struct mutex *__mutex__ __attribute__((cleanup(cleanup_mutex)))= lock_mutex(MUTEX); __mutex__; unlock_mutex(__mutex__), __mutex__=NULL, reschedule())

#endif /* YAROS_MUTEX_H */
