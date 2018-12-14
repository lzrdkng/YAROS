/*
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

#ifndef DEF_H
#define DEF_H

/* Standard */
#include <assert.h>

#include "kernel/config.h"
#include "kernel/type.h"
#include "kernel/errno.h"

#define xstr(s) str(s)
#define str(s) #s

#define likely likely
#define unlikely unlikely

#define cast(TYPE, ...) ( TYPE ) ( __VA_ARGS__ )
#define static_assert _Static_assert
#define assert_type(t1, t2) static_assert(__builtin_types_compatible_P(t1, t2), "Type does not match")

#define COLD __attribute__((cold))
#define DEPRECATED(MSG) __attribute__((deprecated( MSG )))
#define HOT __attribute__((hot))
#define INLINE static inline __attribute__((always_inline))
#define NAKED __attribute__((naked))
#define NON_NULL(...) __attribute__((nonnull(__VA_ARGS__)))
#define NO_INLINE __attribute__((noinline))
#define NO_OPTIMIZE __attribute__((optimize("0")))
#define NO_RETURN __attribute__((noreturn))
#define OS_MAIN __attribute__((OS_main))
#define OS_TASK __attribute__((OS_task))
#define UNUSED __attribute__((unused))
#define OPTIMIZE(O) __attribute__((optimize( O )))
#endif /* DEF_H */
