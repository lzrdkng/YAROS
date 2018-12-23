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



#ifndef YAROS_ADC_H
#define YAROS_ADC_H

#include "kernel/def.h"

/*
 * Analog-to-digital converter driver for the ATmega324PA
 */

enum adc_dev {
     ADC_DEV0
};


enum adc_out {
	ADC_OUT0 = _BV(ADC0D),
	ADC_OUT1 = _BV(ADC1D),
	ADC_OUT2 = _BV(ADC2D),
	ADC_OUT3 = _BV(ADC3D),
	ADC_OUT4 = _BV(ADC4D),
	ADC_OUT5 = _BV(ADC5D),
	ADC_OUT6 = _BV(ADC6D),
	ADC_OUT_MAX
};


/**
 * @enum adc_mode
 *
 * Select which source will trigger an ADC conversion.
 */
enum adc_mode {
	ADC_FREE,					/*< Free Running mode */
	ADC_ANALOG,					/*< Analog Comparator */
	ADC_EXT,					/* External */
	ADC_T0CA,					/* Timer0 compare A */
	ADC_T0OVF,					/* Timer 0 overflow */
	ADC_T1CB,					/* Timer 1 compare B */
	ADC_T1OVF,					/* Timer 1 overflow */
	ADC_T1CE,					/* Timer 1 capture event*/
	ADC_MODE_MAX
};


/**
 * @enum adc_scale
 *
 * These bits determine the division factor between the XTAL frequency
 * and the input clock to the ADC.
 *
 * @note Description is from the manual.
 */
 enum adc_scale {
	ADC_2 = 0b001,
	ADC_4,
	ADC_8,
	ADC_16,
	ADC_32,
	ADC_64,
	ADC_128,
	ADC_SCALE_MAX
 };


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
enum adc_vref {
	ADC_AREF = 0b00,	   /*< AREF, Internal Vref turned off */
	ADC_AVCC,		   /*< AVCC with external capacitor at AREF pin */
	ADC_I11C, /*< Internal 1.1V Voltage Reference with external capacitor at AREF pin */
	ADC_I256C, /*< Internal 2.56V Voltage Reference with external capacitor at AREF pin */
	ADC_REF_MAX
};


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
enum adc_mux {
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
};

/**
 * @brief Initialize the Analog-to-digial converter.
 *
 * @param mode The mode source for the ADC
 *
 * @param scale The division factor of the ADC
 *
 * @param vref The voltage reference for the ADC
 *
 * @param mux The combination of analog inputs connected to the ADC
 *
 * @return OK on success, a negative errno on error. If an error
 * occured, the ADC is restored to its default state.
 *
 * @note Possible value of errno are ENOTSUP.
 */
error_t init_adc(int mode, int scale, int vref);

/**
 * @brief Restore default state of the ADC.
 */
void fini_adc(void);


/**
 * @brief Read the conversion of the ADC.
 *
 * @param mux The selection of analog inputs to read from.
 *
 * @param buff The buffer to write the result to.
 *
 * @return OK on success, a negative errno on error.
 */
error_t read_adc(int mux, void *buff);
#endif /* YAROS_ADC_H */
