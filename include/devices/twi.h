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


#ifndef TWI_H
#define TWI_H

#include "YAROS/def.h"
#include "YAROS/errno.h"

#if defined(__AVR_ATmega324PA__)
#  include "drivers/twi/twi_atmega324pa.h"
#else
# error "Architecture not supported!"
#endif

error_t
init_twi(int device);

error_t
fini_twi(int device);

error_t
read_twi(sla_t sla_r, void *buff, size_t count);

error_t
write_twi(sla_t sla_w, const void *buff, size_t count);


/*
 * The following should be provide by the device' driver as macros or
 * inline functions.
 */

/*
  start_twi()
  stop_twi()
  rstart_twi()
  SLAW_twi()
  SLAR_twi()
  send_twi(U8 data)
  recv_twi(U8 *dst)
  recv_nack_twi(U8 *dst)
  bool twi_ack()
  bool twi_nack()
  bool twi_lost()
*/


#endif /* TWI_H */
