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

#ifndef YAROS_SYSCLK_H
#define YAROS_SYSCLK_H

#include <util/atomic.h>
#include <util/delay.h>

#include "kernel/def.h"
#include "kernel/sched.h"
#include "kernel/global.h"

/* For 8Mhz. TODO: Make this good for every frequency */
#if HZ == 50
# define MS 20

#elif HZ == 100
# define MS 10

#elif HZ == 125
# define MS 8

#elif HZ == 200
# define MS 5

#elif HZ == 250
# define MS 4

#elif HZ == 500
# define MS 2

#elif HZ == 1000
# define MS 1
#else
#  error HZ value not supported.
#endif


/*
 * From Linux Kernel: include/linux/jiffies.h
 */
#define time_after(a, b) ((long)((b) - (a)) < 0)
#define time_before(a, b) time_after(b, a)
#define time_after_eq(a, b) ((long)((a) - (b)) >= 0)
#define time_before_eq(a, b) time_after_eq(b, a)

#define mdelay(_ms) _delay_ms(_ms)
#define udelay(_us) _delay_us(_us)

INLINE jiffy_t
tick()
{
  jiffy_t j;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
      j = jiffies;
    }

  return j;
}

INLINE U64
clk(void)
{
  return MS * cast(U64, tick());
}

INLINE void
wait(jiffy_t delay)
{
  jiffy_t until = tick() + delay;

  while (time_before(tick(), until))
    reschedule();
}

#endif /* YAROS_SYSCLK_H */
