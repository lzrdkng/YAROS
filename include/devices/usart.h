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

#ifndef YAROS_USART_H
#define YAROS_USART_H

#include "kernel/def.h"

/**
 * Initialize
 */
ssize_t
init_usart(int device, ssize_t baud_rate);

ssize_t
write_usart(int device, const void *buff, size_t len);

ssize_t
read_usart(int device, void *buff, size_t len);

error_t
fini_usart(int device);
#endif /* YAROS_USART_H */
