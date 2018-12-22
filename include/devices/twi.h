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


#ifndef YAROS_TWI_H
#define YAROS_TWI_H

#include "kernel/def.h"
#include "kernel/errno.h"

enum twi_mode {
	TWI_MT, 					/*< Master Transmitter */
	TWI_MR,						/*< Master Receiver */
	TWI_ST,						/*< Slave Transmitter */
	TWI_SR						/*< Slave Receiver */
};

error_t
init_twi(int device);

error_t
fini_twi(int device);

error_t
read_twi(int sla_r, void *buff, size_t count);

error_t
write_twi(int sla_w, const void *buff, size_t count);


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


#endif /* YAROS_TWI_H */
