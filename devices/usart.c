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

#include "kernel/config.h"
#include "kernel/msg.h"
#include "devices/usart.h"


#if defined(__AVR_ATmega324PA__)

#  include "drivers/usart/atmega324pa.c"

#else

# warning "USART not supported!"

error_t
init_usart(int device)
{
  return -ENOTSUP;
}

ssize_t
write_usart(int device, const void *buff, size_t len)
{
  return -ENOTSUP;
}


ssize_t
read_usart(int device, void *buff, size_t len)
{
  return -ENOTSUP;
}


error_t
fini_usart(int device)
{
  return -ENOTSUP;
}
#endif
