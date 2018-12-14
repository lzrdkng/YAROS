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

#ifndef IPC_H
#define IPC_H

#include "kernel/sched.h"

#include <util/atomic.h>

struct mutex {
	U8 bits;
};

/*
 * @brief Try to acquire a mutex.
 *
 * @param mutex The mutex to acquire
 *
 * @param bit The bit in the mutex to acquire
 *
 * @return OK on success, -EBUSY if mutex is already acquired
 *
 * @warning THIS SHOULD NEVER BE CALL IN ISR!!!
 */
error_t
try_lock(volatile struct mutex *mutex, U8 bit);


/*
 * @brief Loop until mutex is acquire.
 *
 * @param mutex The mutex to acquire
 *
 * @param bit The bit in the mutex to acquire
 *
 * @note Once return, it's guaranteed that the caller has acquired the
 * mutex.
 *
 * @warning THIS SHOULD NEVER BE CALL IN ISR!!!
 */
static inline void
spinlock(volatile struct mutex *mutex, U8 bit)
{
	while (try_lock(mutex, bit) != OK)
        reschedule();
}


/*
 * @brief Release a mutex.
 */
static inline void
unlock(volatile struct mutex *mutex, U8 bit)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		mutex->bits &= ~_BV(bit);
	}
}

#endif /* IPC_H */
