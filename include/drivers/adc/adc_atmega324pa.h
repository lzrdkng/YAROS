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

#ifndef ADC_M324PA_H
#define ADC_M324PA_H

#include "kernel/errno.h"

#include <avr/io.h>

/*
 * Analog-to-digital converter driver for the ATmega324PA
 */

typedef enum __attribute__((packed)) adc_dev {
	ADC_DEV0
} adc_dev;


typedef enum __attribute__((packed)) adc_out {
	ADC_OUT0 = _BV(ADC0D),
	ADC_OUT1 = _BV(ADC1D),
	ADC_OUT2 = _BV(ADC2D),
	ADC_OUT3 = _BV(ADC3D),
	ADC_OUT4 = _BV(ADC4D),
	ADC_OUT5 = _BV(ADC5D),
	ADC_OUT6 = _BV(ADC6D),
	ADC_OUT_MAX
} adc_out;


/**
 * @enum adc_mode
 *
 * Select which source will trigger an ADC conversion.
 */
typedef enum __attribute__((packed)) adc_mode {
	ADC_FREE,					/*< Free Running mode */
	ADC_ANALOG,					/*< Analog Comparator */
	ADC_EXT,					/* External */
	ADC_T0CA,					/* Timer0 compare A */
	ADC_T0OVF,					/* Timer 0 overflow */
	ADC_T1CB,					/* Timer 1 compare B */
	ADC_T1OVF,					/* Timer 1 overflow */
	ADC_T1CE,					/* Timer 1 capture event*/
	ADC_MODE_MAX
} adc_mode;


/**
 * @enum adc_scale
 *
 * These bits determine the division factor between the XTAL frequency
 * and the input clock to the ADC.
 *
 * @note Description is from the manual.
 */
typedef enum __attribute__((packed)) adc_scale {
	ADC_2 = 0b001,
	ADC_4,
	ADC_8,
	ADC_16,
	ADC_32,
	ADC_64,
	ADC_128,
	ADC_SCALE_MAX
} adc_scale;


/**
 * @enum adc_vref
 *
 * These bits select the voltage reference for the ADC. If the selection
 * is changed during a conversion, the change will not go in effect
 * until this conversion is complete (ADIF in ADCSRA is set). The
 * internal voltage reference options may not be used if an external
 * reference voltage is being applied to the AREF pin.
 *
 * @note Description is from the manual.
 */
typedef enum __attribute__((packed)) adc_vref {
	ADC_AREF = 0b00,	   /*< AREF, Internal Vref turned off */
	ADC_AVCC,		   /*< AVCC with external capacitor at AREF pin */
	ADC_I11C, /*< Internal 1.1V Voltage Reference with external capacitor at AREF pin */
	ADC_I256C, /*< Internal 2.56V Voltage Reference with external capacitor at AREF pin */
	ADC_REF_MAX
} adc_vref;


/**
 * @enum adc_mux
 *
 * The value of these bits selects which combination of analog inputs
 * are connected to the ADC. These bits also select the gain for the
 * differential channels.If these bits are changed during a
 * conversion, the change will not go in effect until this conversion
 * is complete (ADIF in ADCSRA is set).
 *
 * @note Description is from the manual.
 */
typedef enum __attribute__((packed)) adc_mux {
	ADC_MUX0 = 0b00000,
	ADC_MUX1,
	ADC_MUX2,
	ADC_MUX3,
	ADC_MUX4,
	ADC_MUX5,
	ADC_MUX6,
	ADC_MUX7,
	/* Rest is not supported yet */
	ADC_MUX_MAX
} adc_mux;

/* ADMUX */


/* :REFS1:0 */
static inline error_t __attribute__((always_inline))
vref_adc(adc_vref vref)
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
mux_adc(adc_mux selection)
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
scale_adc(adc_scale scale)
{
	if (scale >= ADC_SCALE_MAX)
		return -ENOTSUP;

	ADCSRA = (ADCSRA & 0b11111000) | scale;

	return 0;
}

/* ADCSRB */

/* :ADTS2:0 */
static inline error_t  __attribute__((always_inline))
mode_adc(adc_mode mode)
{
	if (mode >= ADC_MODE_MAX)
		return -ENOTSUP;

	ADCSRB = (ADCSRB & 0b11111000) | mode;

	return OK;
}

/* DIDR0 */
static inline error_t  __attribute__((always_inline))
enable_adc_out(adc_out out)
{
	if (out >= ADC_OUT_MAX)
		return -ENOTSUP;

	DIDR0 &= ~out;

	return OK;
}

static inline error_t  __attribute__((always_inline))
disable_adc_out(adc_out out)
{
	if (out >= ADC_OUT_MAX)
		return -ENOTSUP;

	DIDR0 |= out;

	return OK;
}
#endif /* ADC_M324PA_H */
