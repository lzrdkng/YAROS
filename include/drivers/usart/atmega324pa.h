/*
 * Copyright (C) 2018 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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


#ifndef YAROS_USART_ATMEGA324PA
#define YAROS_USART_ATMEGA324PA

#include <avr/io.h>
#include "kernel/def.h"

INLINE void
__enable_reception()
{
    UCSR0B |= _BV(RXEN0);
}

INLINE void
__disable_reception()
{
    UCSR0B &= ~_BV(RXEN0);
}

INLINE void
__wait_for_send()
{
    while (!(UCSR0A & _BV(UDRE0)))
        ;
}

INLINE void
__wait_for_recv()
{
    while (!(UCSR0A & _BV(RXC0)))
        ;
}

#endif  /* YAROS_USART_ATMEGA324PA */
