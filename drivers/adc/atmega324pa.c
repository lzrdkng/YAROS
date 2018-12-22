
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


#include "devices/adc.h"
#include "devices/irq.h"

/* ADMUX */


/* :REFS1:0 */
static inline error_t __attribute__((always_inline))
vref_adc(int vref)
{
	if (vref >= ADC_REF_MAX)
		return -ENOTSUP;

	ADMUX = (ADMUX & 0b00111111) | (vref << 6);

	return OK;
}


/* :ADLAR */
/* Select left adjust presentation of conversion */
static inline void  __attribute__((always_inline))
left_adjust_adc()
{
	ADMUX |= _BV(ADLAR);
}

/* IDEM but right adjust */
static inline void  __attribute__((always_inline))
right_adjust_adc()
{
	ADMUX &= ~_BV(ADLAR);
}


/* :MUX4:0 */
static inline error_t  __attribute__((always_inline))
mux_adc(int selection)
{
	if (selection >= ADC_MUX_MAX)
		return -ENOTSUP;

	ADMUX = (ADMUX & 0b11100000) | (selection);

	return OK;
}


/* ADCSRA */

/* :ADEN */
static inline void  __attribute__((always_inline))
enable_adc()
{
	ADCSRA |= _BV(ADEN);
}

static inline void  __attribute__((always_inline))
disable_adc()
{
	ADCSRA &= ~_BV(ADEN);
}

/* :ADSC */
static inline void  __attribute__((always_inline))
start_adc(void)
{
	ADCSRA |= _BV(ADSC);
}

static inline bool  __attribute__((always_inline))
adc_started(void)
{
	return ADCSRA & _BV(ADSC);
}

/* :ADATE */
static inline void  __attribute__((always_inline))
auto_adc(void)
{
	ADCSRA |= _BV(ADATE);
}

static inline void  __attribute__((always_inline))
manual_adc(void)
{
	ADCSRA &= ~_BV(ADATE);
}


/****************
 *    :ADIF     *
 *Don't touch it*
 ****************/

/* :ADIE */
/* see enable/disable irq IRQ_ADC */

/* :ADPS2:0 */
static inline error_t  __attribute__((always_inline))
scale_adc(int scale)
{
	if (scale >= ADC_SCALE_MAX)
		return -ENOTSUP;

	ADCSRA = (ADCSRA & 0b11111000) | scale;

	return 0;
}

/* ADCSRB */

/* :ADTS2:0 */
static inline error_t  __attribute__((always_inline))
mode_adc(int mode)
{
	if (mode >= ADC_MODE_MAX)
		return -ENOTSUP;

	ADCSRB = (ADCSRB & 0b11111000) | mode;

	return OK;
}

/* DIDR0 */
static inline error_t  __attribute__((always_inline))
enable_adc_out(int out)
{
	if (out >= ADC_OUT_MAX)
		return -ENOTSUP;

	DIDR0 &= ~out;

	return OK;
}

static inline error_t  __attribute__((always_inline))
disable_adc_out(int out)
{
	if (out >= ADC_OUT_MAX)
		return -ENOTSUP;

	DIDR0 |= out;

	return OK;
}

error_t
init_adc(int mode,
         int scale,
         int vref)
{
    error_t err;

    err = mode_adc(mode);

    if (err)
        goto error;

    err = scale_adc(scale);

    if (err)
        goto error;

    err = vref_adc(vref);

    if (err)
        goto error;

    enable_adc();

    return OK;

error:
    fini_adc();
    return err;
}

void
fini_adc(void)
{
    /* Turnoff any conversion */
    disable_irq_unsafe(IRQ_ADC);
    disable_adc();
    manual_adc();

    /* Reset all registers to their default state */
    ADMUX = 0x00;
    ADCSRA = 0x00;
    ADCH = 0x00;
    ADCL = 0x00;
    ADCSRB = 0x00;
    DIDR0 = 0x00;
}


error_t
read_adc(int mux, void *buff)
{
    error_t err;

    err = mux_adc(mux);

    if (err)
        return err;

    start_adc();

	while (!(ADCSRA & _BV(ADIF)))
		;

	/* Left adjust; 8 bits ADC */
    if (ADMUX & _BV(ADLAR)) {
        *((U8*)buff) = ADCH;
    }
    /* Right adjust; 10 bits ADC */
    else {
        *((U16*)buff) = ADCL;
        *((U16*)buff) |= (ADCH << 8);
    }

	ADCSRA |= _BV(ADIF);

    return OK;
}
