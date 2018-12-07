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

#include "devices/twi.h"
#include "drivers/twi/twi_24XX512.h"

#define CTRL_HNYBBLE 0b1010     /*< Control High Nybble */
#define PAGE_SIZE 128

static inline void __attribute__((always_inline))
wait_for_ack(U8 select)
{
    while (1) {

        start_twi();
        SLAW_twi((CTRL_HNYBBLE << 3) | (select & 0b111));

        if (twi_ack())
            break;
    }
}


static ssize_t
write_byte_24XX512(U8 select,
                   U16 addr,
                   const char *buff)
{
    /* ACK Poll */
    wait_for_ack(select);

    /* START */
    start_twi();

    /* Control Byte*/
    SLAW_twi((CTRL_HNYBBLE << 3) | (select & 0b00000111));

    /* Address High Byte */
    send_twi(addr >> 8);

    /* Address Low Byte */
    send_twi(addr);

    /* Data */
    send_twi(*buff);

    /* STOP */
    stop_twi();

    return 1;
}

static inline U8 __attribute__((always_inline))
write_page_24XX512(U8 select,
                   U16 addr,
                   const char *buff,
                   U8 count)
{
    size_t n;

    U16 TOP = addr + count < (addr | (PAGE_SIZE - 1)) ? addr + count : (addr | (PAGE_SIZE - 1)) +1;

    /* Poll ACK */
    wait_for_ack(select);

    /* START */
    start_twi();

    /* Control Byte */
    SLAW_twi((CTRL_HNYBBLE << 3) | (select & 0b111));

    /* Address High Byte */
    send_twi(addr >> 8);

    /* Address Low Byte */
    send_twi(addr);

    /* Data */
    for (n = 0; n < TOP; ++n) {
        send_twi(((U8*)buff)[n]);
    }

    /* STOP */
    stop_twi();

    return n;
}

#include "devices/usart.h"

ssize_t
write_24XX512(U8 select,
              U16 addr,
              const void *buff,
              size_t count)
{
    size_t written = 0;

    if (count == 1)
        return write_byte_24XX512(select,
                                  addr,
                                  buff);
    do {
        written += write_page_24XX512(select,
                                      addr + written,
                                      buff + written,
                                      (count - written) & 0xFF);
    } while(written < count);

    return written;
}

ssize_t
read_24XX512(U8 select,
             U16 addr,
             void *buff,
             size_t count)
{
    ssize_t n;

    /* ACK Poll */
    wait_for_ack(select);

    /* START */
    start_twi();

    /* Control Byte */
    SLAW_twi((CTRL_HNYBBLE << 3) | (select & 0b111));

    /* Address High Byte */
    send_twi(addr >> 8);

    /* Address Low Byte */
    send_twi(addr);

    /* START */
    start_twi();

    /* Control Byte */
    SLAR_twi((CTRL_HNYBBLE << 3) | (select & 0b111));

    /* Data */
    for (n = 0; n < count - 1; ++n)
        recv_twi((U8*)buff + n);

    /* Last byte NACK */
    recv_nack_twi((U8*)buff + n++);

    /* STOP */
    stop_twi();

    return n;
}
