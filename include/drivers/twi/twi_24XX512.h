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

#ifndef TWI_24XX512_H
#define TWI_24XX512_H

#include "YAROS/type.h"

#include "devices/twi.h"

#define PAGE_SIZE_24XX512 128

ssize_t
write_24XX512(U8 select,
			  U16 addr,
			  const void *buff,
			  size_t count);

ssize_t
read_24XX512(U8 select,
			 U16 addr,
			 void *buff,
			 size_t count);
#endif /* TWI_24XX512_H */
