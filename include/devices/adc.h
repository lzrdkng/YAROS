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



#ifndef ADC_H
#define ADC_H

#include "kernel/def.h"

#if defined(__AVR_ATmega324PA__)
#  include "drivers/adc/adc_atmega324pa.h"
#else
# error "Architecture not supported!"
#endif

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
error_t
init_adc(adc_mode mode,
		 adc_scale scale,
		 adc_vref vref);

/**
 * @brief Restore default state of the ADC.
 */
void
fini_adc(void);


/**
 * @brief Read the conversion of the ADC.
 *
 * @param mux The selection of analog inputs to read from.
 *
 * @param buff The buffer to write the result to.
 *
 * @return OK on success, a negative errno on error.
 **/
error_t
read_adc(adc_mux mux, void *buff);
#endif /* ADC_H */
