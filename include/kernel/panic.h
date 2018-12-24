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

#ifndef YAROS_PANIC_H
#define YAROS_PANIC_H

#include <avr/interrupt.h>
#include "kernel/def.h"

#define panic_kernel(INFOS)                             \
     do {                                               \
          asm volatile("cli\n\t"                        \
                       "push r0\n\t"                    \
                       "push r1\n\t"                    \
                       "push r25\n\t"                   \
                       "push r24\n\t");                 \
          _panic_kernel( INFOS );                       \
     } while (0)


NO_RETURN void _panic_kernel(S16 infos);

void recover_kernel(S16 infos);

#endif /* YAROS_PANIC_H */
