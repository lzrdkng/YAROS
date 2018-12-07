
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

#include "YAROS/def.h"
#include "YAROS/errno.h"

#include "devices/adc.h"
#include "devices/irq.h"

#include <avr/interrupt.h>

error_t
init_adc(adc_mode mode,
         adc_scale scale,
         adc_vref vref)
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
read_adc(adc_mux mux, void *buff)
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
