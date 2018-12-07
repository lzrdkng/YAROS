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

#ifndef USART_H
#define USART_H

#include "YAROS/def.h"

#if defined(__AVR_ATmega324PA__)
#  include "drivers/usart/usart_atmega324pa.h"
#else
# error "Architecture not supported!"
#endif

void
init_usart(void);

ssize_t
write_usart(int minor, const void *buff, size_t len);

ssize_t
read_usart(int minor, void *buff, size_t len);
#endif /* USART_H */
