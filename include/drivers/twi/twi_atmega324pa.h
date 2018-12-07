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

#ifndef TWI_ATMEGA324PA_H
#define TWI_ATMEGA324PA_H

#include <avr/io.h>
#include <util/twi.h>

#define STATUS_MASK 0xF8

#define MT_START_COND 0x08 /*< A START condition has been transmitted */
#define MT_START_COND_REPEATED 0x10 /*< A repeated START condition has been transmited */
#define MT_SLA_ACK 0x18	/*< SLA+W has been transmitted; ACK has been received */
#define MT_SLA_NACK 0x20 /*< SLA+W has been transmitted; NACK has been received */
#define MT_DATA_ACK 0x28 /*< Data byte has been transmitted; ACK received */
#define MT_DATA_NACK 0x30 /*< Data byte has been transmitted; NACK received */
#define MT_LOST 0x38	/*< Arbitration lost in SLA+W or data bytes */

#define MR_START_COND 0x08
#define MR_START_COND_REPEATED 0x10
#define MR_LOST 0x38
#define MR_SLA_ACK 0x40
#define MR_SLA_NACK 0x48
#define MR_DATA_ACK 0x50
#define MR_DATA_NACK 0x58


typedef unsigned char sla_t;

typedef enum __attribute__((packed)) twi_mode {
	TWI_MT, 					/*< Master Transmitter */
	TWI_MR,						/*< Master Receiver */
	TWI_ST,						/*< Slave Transmitter */
	TWI_SR						/*< Slave Receiver */
} twi_m;


static inline void __attribute__((always_inline))
__wait_for_transmission(void)
{
	while (!(TWCR & _BV(TWINT)))
		;
}



static inline void __attribute__((always_inline))
start_twi(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	__wait_for_transmission();
}

static inline void __attribute__((always_inline))
stop_twi(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}


static inline void __attribute__((always_inline))
rstart_twi(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
}


static inline void __attribute__((always_inline))
SLAW_twi(U8 addr)
{
	TWDR = addr << 1;
	TWCR = _BV(TWINT) | _BV(TWEN);
	__wait_for_transmission();
}


static inline void __attribute__((always_inline))
SLAR_twi(U8 addr)
{
	TWDR = (addr << 1) | 0x01;
	TWCR = _BV(TWINT) | _BV(TWEN);
	__wait_for_transmission();
}


static inline void __attribute__((always_inline))
send_twi(U8 data)
{
	TWDR = data;
	TWCR = _BV(TWINT) | _BV(TWEN);
	__wait_for_transmission();
}

static inline void __attribute__((always_inline))
recv_twi(U8 *dst)
{
	TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN);
	__wait_for_transmission();
	*dst = TWDR;
}


static inline void __attribute__((always_inline))
recv_nack_twi(U8 *dst)
{
	TWCR = _BV(TWINT) | _BV(TWEN);
	__wait_for_transmission();
	*dst = TWDR;
}


static inline bool __attribute__((always_inline))
twi_ack(void)
{
	return (TWSR & STATUS_MASK) == MT_SLA_ACK;
}

#endif /* TWI_ATMEGA324PA_H */
