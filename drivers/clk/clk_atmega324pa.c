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

#include "kernel/errno.h"
#include "kernel/def.h"

#include "devices/clk.h"


/* CLOCK 0 */

static clk_m
get_clk0_mode()
{
    switch (((TCCR0B & _BV(WGM02)) >> 1) |
            (TCCR0A & ( _BV(WGM01) | _BV(WGM00)))) {

    case 0b000:
        return CLK_NORMAL;

    case 0b001:
        return CLK_PWMP;

    case 0b010:
        return CLK_CTC;

    case 0b011:
        return CLK_PWM;

        /* 0b100 is reserved */

    case 0b101:
        return CLK_PWMPA;

        /* 0b110 is reserved */

    case 0b111:
        return CLK_PWMA;
    }

    return (clk_m)-ENOTSUP;
}

static clk_s
get_clk0_src()
{
    switch (TCCR0B & (_BV(CS02) | _BV(CS01) | _BV(CS00))) {

    case 0b000:
        return CLK_STOP;

    case 0b001:
        return CLK_P1;

    case 0b010:
        return CLK_P8;

    case 0b011:
        return CLK_P64;

    case 0b100:
        return CLK_P256;

    case 0b101:
        return CLK_P1024;

    case 0b110:
        return CLK_EXTF;

    case 0b111:
        return CLK_EXTR;
    }

    return (clk_s)-ENOTSUP;
}


static clk_o __attribute__((warning("NOT SUPPORTED")))
get_clk0_out()
{
    clk_o out = -ENOTSUP;

    /* COM0A */
    switch (get_clk0_mode()) {

        /* Phase Correct PWM mode */
    case CLK_PWMP:
    case CLK_PWMPA:
    case CLK_PWM:
    case CLK_PWMA:
    default:
        break;
    }

    return out;
}

static error_t
set_clk0_mode(clk_m mode)
{
    switch (mode) {

    case CLK_NORMAL:            /* 000 */
        TCCR0B &= ~_BV(WGM02);
        TCCR0A &= ~_BV(WGM01);
        TCCR0A &= ~_BV(WGM00);
        break;

    case CLK_PWMP:              /* 001 */
        TCCR0B &= ~_BV(WGM02);
        TCCR0A &= ~_BV(WGM01);
        TCCR0A |= _BV(WGM00);
        break;

    case CLK_CTC:               /* 010 */
        TCCR0B &= ~_BV(WGM02);
        TCCR0A |= _BV(WGM01);
        TCCR0A &= ~_BV(WGM00);
        break;

    case CLK_PWM:               /* 011 */
        TCCR0B &= ~_BV(WGM02);
        TCCR0A |= _BV(WGM01);
        TCCR0A |= _BV(WGM00);
        break;

        /* 100 is reserved */

    case CLK_PWMPA:             /* 101 */
        TCCR0B |= _BV(WGM02);
        TCCR0A &= ~_BV(WGM01);
        TCCR0A |= _BV(WGM00);
        break;

        /* 110 is reserved */

    case CLK_PWMA:              /* 111 */
        TCCR0B |= _BV(WGM02);
        TCCR0A |= _BV(WGM01);
        TCCR0A |= _BV(WGM00);
        break;

    default:
        return -ENOTSUP;
    }

    return OK;
}


static error_t
set_clk0_src(clk_s src)
{
    switch (src) {

    case CLK_STOP:              /* 000 */
        TCCR0B &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
        break;

    case CLK_P1:                /* 001 */
        TCCR0B &= ~(_BV(CS02) | _BV(CS01));
        TCCR0B |= _BV(CS00);
        break;

    case CLK_P8:                /* 010 */
        TCCR0B &= ~_BV(CS02);
        TCCR0B |= _BV(CS01);
        TCCR0B &= ~_BV(CS00);
        break;

    case CLK_P64:               /* 011 */
        TCCR0B &= ~_BV(CS02);
        TCCR0B |= _BV(CS01) | _BV(CS00);
        break;

    case CLK_P256:              /* 100 */
        TCCR0B |= _BV(CS02);
        TCCR0B &= ~(_BV(CS01) | _BV(CS00));
        break;

    case CLK_P1024:             /* 101 */
        TCCR0B |= _BV(CS02);
        TCCR0B &= ~_BV(CS01);
        TCCR0B |= _BV(CS00);
        break;

    case CLK_EXTF:              /* 110 */
        TCCR0B |= _BV(CS02) | _BV(CS01);
        TCCR0B &= ~_BV(CS00);
        break;

    case CLK_EXTR:              /* 111 */
        TCCR0B |= _BV(CS02) | _BV(CS01) | _BV(CS00);
        break;

    default:
        return -ENOTSUP;
    }

    return OK;
}


static error_t
set_clk0_out(clk_o out)
{
    /* Out of range */
    if (out & 0b00001111)
        return -ENOTSUP;

    /* Reset the output */
    TCCR0A &= ~(_BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(COM0B0));

    /* Set the ouput */
    TCCR0A |= out;

    return OK;
}

/* CLOCK 1 */

static clk_m
get_clk1_mode()
{
    switch (((TCCR2B & _BV(WGM22)) >> 1) |
            (TCCR2A & ( _BV(WGM21) | _BV(WGM20)))) {

    case 0b0000:
        return CLK_NORMAL;

    case 0b0001:
        return CLK_PWMP8;

    case 0b0010:
        return CLK_PWMP9;

    case 0b0011:
        return CLK_PWMP10;

    case 0b0100:
        return CLK_CTC;

    case 0b0101:
        return CLK_PWM8;

    case 0b0110:
        return CLK_PWM9;

    case 0b0111:
        return CLK_PWM10;

    case 0b1000:
        return CLK_PWMPFI;

    case 0b1001:
        return CLK_PWMPFA;

    case 0b1010:
        return CLK_PWMPI;

    case 0b1011:
        return CLK_PWMPA;

    case 0b1100:
        return CLK_CTCI;

        /* 0b1101 is reserved */

    case 0b1110:
        return CLK_PWMI;

    case 0b1111:
        return CLK_PWMA;

    default:
        return -ENOTSUP;
    }
}

static clk_s
get_clk1_src()
{
    switch (TCCR1B & (_BV(CS12) | _BV(CS11) | _BV(CS10))) {

    case 0b000:
        return CLK_STOP;

    case 0b001:
        return CLK_P1;

    case 0b010:
        return CLK_P8;

    case 0b011:
        return CLK_P64;

    case 0b100:
        return CLK_P256;

    case 0b101:
        return CLK_P1024;

    case 0b110:
        return CLK_EXTF;

    case 0b111:
        return CLK_EXTR;

    default:
        return -ENOTSUP;
    }
}


static clk_o __attribute__((warning("NOT SUPPORTED")))
get_clk1_out()
{
    clk_o out = 0;

    switch (get_clk1_mode()) {

    case CLK_PWMP8:
    case CLK_PWMP9:
    case CLK_PWMP10:
    case CLK_PWMPFA:
    case CLK_PWMPFI:
    case CLK_PWMPA:
    case CLK_PWMPI:

        return -ENOTSUP;

    case CLK_PWM8:
    case CLK_PWM9:
    case CLK_PWM10:
    case CLK_PWMA:
    case CLK_PWMI:

        switch (TCCR1A & (_BV(COM1A1) | _BV(COM1A0))) {

        case _BV(COM1A1) | _BV(COM1A0): /* 0b11 */
            out |= CLK_SA;
            break;

        case _BV(COM1A1):       /* 0b10 */

            switch (((TCCR1B & (_BV(WGM13) | _BV(WGM12))) >> 1) |
                    (TCCR1A & (_BV(WGM11) | _BV(WGM10)))) {

            case 14:
            case 15:
                out |= CLK_TA;
                break;

            default:
                out |= CLK_DA;
                break;
            }

            break;

        case _BV(COM1A0):       /* 0b01*/
            out |= CLK_TA;
            break;

        default:
            out |= CLK_DA;
            break;
        }

        switch (TCCR1A & (_BV(COM1B1) | _BV(COM1B0))) {

        case _BV(COM1B1) | _BV(COM1B0): /* 0b11 */
            out |= CLK_SB;
            break;

        case _BV(COM1B1):       /* 0b10 */
            out |= CLK_DB;
            break;

        case _BV(COM1B0):       /* 0b01 */
            out |= CLK_TB;
            break;

        default:                /* 0b00 */
            out |= CLK_DB;
            break;
        }

        break;

        /* NORMAL AND CTC MODE  */
    default:

        switch (TCCR1A & (_BV(COM1A1) | _BV(COM1A0))) {

        case _BV(COM1A1) | _BV(COM1A0):
            out |= CLK_SA;
            break;

        case _BV(COM1A1):
            out |= CLK_CA;
            break;

        case _BV(COM1A0):
            out |= CLK_TA;
            break;

        default:
            out |= CLK_DA;
            break;
        }

        switch (TCCR1A & (_BV(COM1B1) | _BV(COM1B0))) {

        case _BV(COM1B1) | _BV(COM1B0):
            out |= CLK_SB;
            break;

        case _BV(COM1B1):
            out |= CLK_CB;
            break;

        case _BV(COM1B0):
            out |= CLK_TB;
            break;

        default:
            out |= CLK_DB;
            break;
        }
    }

    return out;
}

static error_t
set_clk1_mode(clk_m mode)
{
    switch (mode) {

    case CLK_NORMAL:
        TCCR1B &= ~(_BV(WGM13) | _BV(WGM12)); /* 00 */
        TCCR1A &= ~(_BV(WGM11) | _BV(WGM10)); /* 00 */
        break;

    case CLK_PWMP8:
        TCCR1B &= ~(_BV(WGM13) | _BV(WGM12)); /* 00 */
        TCCR1A &= ~_BV(WGM11);                /* 01 */
        TCCR1A |= _BV(WGM10);
        break;

    case CLK_PWMP9:
        TCCR1B &= ~(_BV(WGM13) | _BV(WGM12)); /* 00 */
        TCCR1A |= _BV(WGM11);                 /* 10 */
        TCCR1A &= ~_BV(WGM10);
        break;

    case CLK_PWMP10:
        TCCR1B &= ~(_BV(WGM13) | _BV(WGM12)); /* 00 */
        TCCR1A |= _BV(WGM11) | _BV(WGM10);    /* 11 */
        break;

    case CLK_CTC:
        TCCR1B &= ~_BV(WGM13);  /* 01 */
        TCCR1B |= _BV(WGM12);
        TCCR1A &= ~(_BV(WGM11) | _BV(WGM10)); /* 00 */
        break;

    case CLK_PWM8:
        TCCR1B &= ~_BV(WGM13);  /* 01 */
        TCCR1B |= _BV(WGM12);
        TCCR1A &= ~_BV(WGM11);  /* 01 */
        TCCR1A |= _BV(WGM10);
        break;

    case CLK_PWM9:
        TCCR1B &= ~_BV(WGM13);  /* 01 */
        TCCR1B |= _BV(WGM12);
        TCCR1A |= _BV(WGM11);   /* 10 */
        TCCR1A &= ~_BV(WGM10);
        break;

    case CLK_PWM10:
        TCCR1B &= ~_BV(WGM13);  /* 01 */
        TCCR1B |= _BV(WGM12);
        TCCR1A |= _BV(WGM11);   /* 11 */
        TCCR1A |= _BV(WGM10);
        break;

    case CLK_PWMPFI:
        TCCR1B |= _BV(WGM13);   /* 10 */
        TCCR1B &= ~_BV(WGM12);
        TCCR1A &= ~(_BV(WGM11) | _BV(WGM10)); /* 00 */
        break;

    case CLK_PWMPFA:
        TCCR1B |= _BV(WGM13);   /* 10 */
        TCCR1B &= ~_BV(WGM12);
        TCCR1A &= ~_BV(WGM11);  /* 01 */
        TCCR1A |=  _BV(WGM10);
        break;

    case CLK_PWMPI:
        TCCR1B |= _BV(WGM13);   /* 10 */
        TCCR1B &= ~_BV(WGM12);
        TCCR1A |= _BV(WGM11);   /* 10 */
        TCCR1A &= ~_BV(WGM10);
        break;

    case CLK_PWMPA:
        TCCR1B |= _BV(WGM13);   /* 10 */
        TCCR1B &= ~_BV(WGM12);
        TCCR1A |= _BV(WGM11);   /* 11 */
        TCCR1A |= _BV(WGM10);
        break;

    case CLK_CTCI:
        TCCR1B |= _BV(WGM13) | _BV(WGM12);    /* 11 */
        TCCR1A &= ~(_BV(WGM11) | _BV(WGM10)); /* 00 */
        break;

        /* 1101 is reserved */

    case CLK_PWMI:
        TCCR1B |= _BV(WGM13) | _BV(WGM12);  /* 11 */
        TCCR1A |= _BV(WGM11);               /* 10 */
        TCCR1A &= ~_BV(WGM10);
        break;


    case CLK_PWMA:
        TCCR1B |= _BV(WGM13) | _BV(WGM12); /* 11 */
        TCCR1A |= _BV(WGM11) | _BV(WGM10); /* 11 */
        break;

    default:
        return -ENOTSUP;
    }

    return OK;
}


static error_t
set_clk1_src(clk_s src)
{
    switch (src) {

    case CLK_STOP:              /* 000 */
        TCCR1B &= ~(_BV(CS12) | _BV(CS11) | _BV(CS10));
        break;

    case CLK_P1:                /* 001 */
        TCCR1B &= ~(_BV(CS12) | _BV(CS11));
        TCCR1B |= _BV(CS10);
        break;

    case CLK_P8:                /* 010 */
        TCCR1B &= ~_BV(CS12);
        TCCR1B |= _BV(CS11);
        TCCR1B &= ~_BV(CS10);
        break;

    case CLK_P64:               /* 011 */
        TCCR1B &= ~_BV(CS12);
        TCCR1B |= _BV(CS11) | _BV(CS10);
        break;

    case CLK_P256:              /* 100 */
        TCCR1B |= _BV(CS12);
        TCCR1B &= ~(_BV(CS11) | _BV(CS10));
        break;

    case CLK_P1024:             /* 101 */
        TCCR1B |= _BV(CS12);
        TCCR1B &= ~_BV(CS11);
        TCCR1B |= _BV(CS10);
        break;

    case CLK_EXTF:              /* 110 */
        TCCR1B |= _BV(CS12) | _BV(CS11);
        TCCR1B &= ~_BV(CS10);
        break;

    case CLK_EXTR:              /* 111 */
        TCCR1B |= _BV(CS12) | _BV(CS11) | _BV(CS10);
        break;

    default:
        return -ENOTSUP;
    }

    return OK;
}


static error_t
set_clk1_out(clk_o out)
{
    /* Out of range */
    if (out & 0b00001111)
        return -ENOTSUP;

    /* Reset the output */
    TCCR1A &= ~(_BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0));

    /* Set the ouput */
    TCCR1A |= out;

    return OK;
}





/* CLOCK 2 */

static clk_m
get_clk2_mode()
{
    switch (((TCCR2B & _BV(WGM22)) >> 1) |
            (TCCR2A & ( _BV(WGM21) | _BV(WGM20)))) {

    case 0b000:
        return CLK_NORMAL;

    case 0b001:
        return CLK_PWMP;

    case 0b010:
        return CLK_CTC;

    case 0b011:
        return CLK_PWM;

        /* 0b100 is reserved */

    case 0b101:
        return CLK_PWMPA;

        /* 0b110 is reserved */

    case 0b111:
        return CLK_PWMA;
    }

    return (clk_m)-ENOTSUP;
}

static clk_s
get_clk2_src()
{
    return (TCCR2B & (_BV(CS22) | _BV(CS21) | _BV(CS20)));
}


static clk_o
get_clk2_out()
{
    clk_o out = 0;

    switch (TCCR2A & (_BV(COM2A1) | _BV(COM2A0))) {

    case _BV(COM2A1) | _BV(COM2A0):

        out |= CLK_SA;
        break;

    case _BV(COM2A1):

        out |= CLK_CA;
        break;

    case _BV(COM2A0):

        switch (get_clk2_mode()) {
        case CLK_PWMP:
        case CLK_PWM:

            out |= CLK_DA;
            break;

        default:

            out |= CLK_TA;
            break;
        }

        break;

    default:

        out |= CLK_DA;
        break;
    }


    switch (TCCR2A & (_BV(COM2B1) | _BV(COM2B0))) {

    case _BV(COM2B1) | _BV(COM2B0):

        out |= CLK_SB;
        break;

    case _BV(COM2B1):

        out |= CLK_CB;
        break;

    case _BV(COM2B0):

        switch (get_clk2_mode()) {
        case CLK_NORMAL:
        case CLK_CTC:
            out |= CLK_TB;
            break;
        default:
            out |= CLK_DB;
        }

    default:

        out |= CLK_DB;
        break;
    }


    return out;
}

static error_t
set_clk2_mode(clk_m mode)
{
    switch (mode) {

    case CLK_NORMAL:            /* 000 */
        TCCR2B &= ~_BV(WGM22);
        TCCR2A &= ~_BV(WGM21);
        TCCR2A &= ~_BV(WGM20);
        break;

    case CLK_PWMP:              /* 001 */
        TCCR2B &= ~_BV(WGM22);
        TCCR2A &= ~_BV(WGM21);
        TCCR2A |= _BV(WGM20);
        break;

    case CLK_CTC:               /* 010 */
        TCCR2B &= ~_BV(WGM22);
        TCCR2A |= _BV(WGM21);
        TCCR2A &= ~_BV(WGM20);
        break;

    case CLK_PWM:               /* 011 */
        TCCR2B &= ~_BV(WGM22);
        TCCR2A |= _BV(WGM21);
        TCCR2A |= _BV(WGM20);
        break;

        /* 100 is reserved */

    case CLK_PWMPA:             /* 101 */
        TCCR2B |= _BV(WGM22);
        TCCR2A &= ~_BV(WGM21);
        TCCR2A |= _BV(WGM20);
        break;

        /* 110 is reserved */

    case CLK_PWMA:              /* 111 */
        TCCR2B |= _BV(WGM22);
        TCCR2A |= _BV(WGM21);
        TCCR2A |= _BV(WGM20);
        break;

    default:
        return -ENOTSUP;
    }

    return OK;
}

static error_t
set_clk2_src(clk_s src)
{
    switch (src) {

    case CLK_STOP:              /* 000 */
        TCCR2B &= ~_BV(CS22);
        TCCR2B &= ~_BV(CS21);
        TCCR2B &= ~_BV(CS20);
        break;

    case CLK_P1:                /* 001 */
        TCCR2B &= ~_BV(CS22);
        TCCR2B &= ~_BV(CS21);
        TCCR2B |= _BV(CS20);
        break;

    case CLK_P8:                /* 010 */
        TCCR2B &= ~_BV(CS22);
        TCCR2B |= _BV(CS21);
        TCCR2B &= ~_BV(CS20);
        break;

    case CLK_P32:               /* 011 */
        TCCR2B &= ~_BV(CS22);
        TCCR2B |= _BV(CS21);
        TCCR2B |= _BV(CS20);
        break;

    case CLK_P64:               /* 100 */
        TCCR2B |= _BV(CS22);
        TCCR2B &= ~_BV(CS21);
        TCCR2B &= ~_BV(CS20);
        break;

    case CLK_P128:              /* 101 */
        TCCR2B |= _BV(CS22);
        TCCR2B &= ~_BV(CS21);
        TCCR2B |= _BV(CS20);
        break;

    case CLK_P256:              /* 110 */
        TCCR2B |= _BV(CS22);
        TCCR2B |= _BV(CS21);
        TCCR2B &= ~_BV(CS20);
        break;

    case CLK_P1024:             /* 111 */
        TCCR2B |= _BV(CS22);
        TCCR2B |= _BV(CS21);
        TCCR2B |= _BV(CS20);
        break;

    default:
        return -ENOTSUP;
    }

    return OK;
}

static error_t
set_clk2_out(clk_o out)
{
    /* Out of range */
    if (out & 0b00001111)
        return -ENOTSUP;

    /* There's no toggle of output B in fast and phase correct PWM modes */
    if ((out & 0b00110000) == CLK_TB) {

        /* PWM Phase Correct; Fast PWM */
        if (TCCR2A & _BV(WGM20)) {
            return -ENOTSUP;
        }
    }

    /* Reset the output */
    TCCR2A &= ~(_BV(COM2A1) | _BV(COM2A0) | _BV(COM2B1) | _BV(COM2B0));

    /* Set the ouput */
    TCCR2A |= out;

    return OK;
}



error_t
init_clk(clk_t clk,
         clk_m mode,
         clk_s src,
         clk_o out)
{
    if (clk >= CLK_TYPE_MAX || clk < 0)
        return -EBDEV;

    clk_m saved_mode = get_clk_mode(clk);
    clk_s saved_src  = get_clk_src(clk);
    clk_o saved_out  = get_clk_out(clk);

    error_t retval = OK;

    fini_clk(clk);

    switch (clk) {

    case CLK_0:

        retval = set_clk0_mode(mode);

        if (retval)
            goto clk0_failed;

        retval = set_clk0_out(out);

        if (retval)
            goto clk0_failed;

        retval = set_clk0_src(src);

        if (retval)
            goto clk0_failed;

        break;

    clk0_failed:
        set_clk0_mode(saved_mode);
        set_clk0_out(saved_out);
        set_clk0_src(saved_src);

        break;


    case CLK_1:
        retval = set_clk1_mode(mode);

        if (retval)
            goto clk1_failed;

        retval = set_clk1_out(out);

        if (retval)
            goto clk1_failed;

        retval = set_clk1_src(src);

        if (retval)
            goto clk1_failed;

        break;

    clk1_failed:
        set_clk1_mode(saved_mode);
        set_clk1_out(saved_out);
        set_clk1_src(saved_src);

        break;


    case CLK_2:
        retval = set_clk2_mode(mode);

        if (retval)
            goto clk2_failed;

        retval = set_clk2_out(out);

        if (retval)
            goto clk2_failed;

        retval = set_clk2_src(src);

        if (retval)
            goto clk2_failed;

        break;

    clk2_failed:
        set_clk2_mode(saved_mode);
        set_clk2_out(saved_out);
        set_clk2_src(saved_src);

        break;


    default:
        return -EBDEV;
    }


    return retval;
}


error_t
fini_clk(clk_t clk)
{
    switch (clk) {

    case CLK_0:
        TIMSK0 = 0x00;
        TCCR0A = 0x00;
        TCCR0B = 0x00;
        TCNT0 = 0x00;
        OCR0A = 0x00;
        OCR0B = 0x00;
        TIFR0 = 0x00;
        break;

    case CLK_1:
        TIMSK1 = 0x00;
        TCCR1A = 0x00;
        TCCR1B = 0x00;
        TCCR1C = 0x00;
        TCNT1 = 0x0000;
        OCR1A = 0x0000;
        OCR1B = 0x0000;
        ICR1 = 0x0000;
        TIFR1 = 0x00;
        break;

    case CLK_2:
        TIMSK2 = 0x00;
        TCCR2A = 0x00;
        TCCR2B = 0x00;
        TCNT2 = 0x00;
        OCR2A = 0x00;
        OCR2B = 0x00;
        ASSR = 0x00;
        TIFR2 = 0x00;
        GTCCR = 0x00;
        break;

    default:
        return -EBDEV;
    }

    return OK;
}

error_t
read_clk(clk_t clk, void *buff)
{
    switch (clk) {

    case CLK_0:
        *((U8*)buff) = TCNT0;
        break;

    case CLK_1:
        *((U16*)buff) = TCNT1;
        break;

    case CLK_2:
        *((U8*)buff) = TCNT2;
        break;

    default:
        return -EBDEV;
    }

    return OK;
}


error_t
clk_mode(clk_t clk,
         clk_m mode)
{
    switch (clk) {

    case CLK_0:
        return set_clk0_mode(mode);

    case CLK_1:
        return set_clk1_mode(mode);

    case CLK_2:
        return set_clk2_mode(mode);

    default:
        return -EBDEV;
    }
}

error_t
clk_src(clk_t clk,
        clk_s src)
{
    switch (clk) {

    case CLK_0:
        return set_clk0_src(src);

    case CLK_1:
        return set_clk1_src(src);

    case CLK_2:
        return set_clk2_src(src);

    default:
        return -EBDEV;
    }
}


error_t
clk_out(clk_t clk,
        clk_o out)
{
    switch (clk) {

    case CLK_0:
        return set_clk0_out(out);

    case CLK_1:
        return set_clk1_out(out);

    case CLK_2:
        return set_clk2_out(out);

    default:
        return -EBDEV;
    }
}

clk_m
get_clk_mode(clk_t clk)
{
    switch (clk) {

    case CLK_0:
        return get_clk0_mode();

    case CLK_1:
        return get_clk1_mode();

    case CLK_2:
        return get_clk2_mode();

    default:
        return (clk_m)-EBDEV;
    }
}

clk_s
get_clk_src(clk_t clk)
{
    switch (clk) {

    case CLK_0:
        return get_clk0_src();

    case CLK_1:
        return get_clk1_src();

    case CLK_2:
        return get_clk2_src();

    default:
        return (clk_s)-EBDEV;
    }
}



clk_o
get_clk_out(clk_t clk)
{
    switch (clk) {

    case CLK_0:
        return get_clk0_out();

    case CLK_1:
        return get_clk1_out();

    case CLK_2:
        return get_clk2_out();

    default:
        return (clk_o)-EBDEV;
    }
}


error_t
write_clk(clk_t clk, const void *buff)
{
    switch (clk) {

    case CLK_0:
        TCNT0 = *((U8*)buff);
        break;

    case CLK_1:
        TCNT1 = *((U16*)buff);
        break;

    case CLK_2:
        TCNT2 = *((U8*)buff);
        break;

    default:
        return -EBDEV;
    }

    return OK;
}
