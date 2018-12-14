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

/*
 * twi driver for the atmega324pa
 */

#include "kernel/errno.h"

#include "devices/twi.h"


#if 0
void
init_twi(void)
{
    /* TWI Status Register */
    TWSR = 0x00;

    /*
     * TWI Bit Rate Register
     * This will give us a bit rate of 10E6 Hz
     */
    TWBR = (8000000UL / 100000UL - 16) / 2;
}
#endif


/**
 * @brief Master Receiver mode protocol for the ATmega324PA
 */
error_t
read_twi(sla_t sla_r, void *buff, size_t count)
{
    ssize_t i;

    /* START Condition */
    start_twi();

    /* If START condition not received */
    if ((TWSR & STATUS_MASK) != MR_START_COND)
        return -ECOMM;

    /* Master Receiver mode (SLA+R) */
    SLAR_twi(sla_r);

    /* NACK Address */
    if ((TWSR & STATUS_MASK) == MR_SLA_NACK) {
        stop_twi();
        return -ECOMM;
    }

    /* Lost Address */
    if ((TWSR & STATUS_MASK) == MR_LOST) {
        TWCR = _BV(TWINT) | _BV(TWEN);
        return -ECOMM;
    }


    /* SLA+R has been transmitted; ACK has been received */


    /* Data byte will be received and ACK will be returned */
    for (i = 0; i < count - 1; ++i)
        recv_twi((U8*)buff + i);

    /* Data byte will be received and NACK will be returned */
    recv_nack_twi((U8*)buff + i++);

    /* STOP Condition */
    stop_twi();

    return (error_t)i;
}


/**
 * @brief Master Transmitter mode protocol for the ATmega324PA.
 */
error_t
write_twi(sla_t sla_w,
          const void *buff,
          size_t count)
{
    ssize_t i;

    /* START Condition */
    start_twi();

    /* If START Condition not received */
    if ((TWSR & STATUS_MASK) != MT_START_COND)
        return -ECOMM;

    /* Master Transmitter mode (SLA+W) */
    SLAW_twi(sla_w);

    /* NACK Address */
    if ((TWSR & STATUS_MASK) == MT_SLA_NACK) {
        stop_twi();
        return -ECOMM;
    }

    /* Lost Address */
    if ((TWSR & STATUS_MASK) == MT_LOST) {
        TWCR = _BV(TWINT) | _BV(TWEN);
        return -ECOMM;
    }


    /* SLA+W has been transmitted; ACK has been received */

    for (i = 0; i < count; ++i) {

        /* Data byte will be transmitted */
        send_twi(((char*)buff)[i]);

        /* Data has been transmitted; NOT ACK has been received */
        if ((TWSR & STATUS_MASK) != MT_DATA_ACK)
            break;
    }

    /* STOP condition */
    stop_twi();

    /* Return number of byte transmitted */
    return (error_t)i;
}
