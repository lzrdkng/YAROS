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

#ifndef IRQ_M324PA_H
#define IRQ_M324PA_H

typedef enum __attribute__((packed)) irq_m324pa {
		IRQ_RESET = 0x00,
		IRQ_INT0,
		IRQ_INT1,
		IRQ_INT2,
		IRQ_PCINT0,
		IRQ_PCINT1,
		IRQ_PCINT2,
		IRQ_PCINT3,
		IRQ_WDT,
		IRQ_TIMER2_COMPA,
		IRQ_TIMER2_COMPB,
		IRQ_TIMER2_OVF,
		IRQ_TIMER1_CAPT,
		IRQ_TIMER1_COMPA,
		IRQ_TIMER1_COMPB,
		IRQ_TIMER1_OVF,
		IRQ_TIMER0_COMPA,
		IRQ_TIMER0_COMPB,
		IRQ_TIMER0_OVF,
		IRQ_SPI_STC,
		IRQ_USART0_RX,
		IRQ_USART0_UDRE,
		IRQ_USART0_TX,
		IRQ_ANALOG_COMP,
		IRQ_ADC,
		IRQ_EE_READY,
		IRQ_TWI,
		IRQ_SPM_READY,
		IRQ_USART1_RX,
		IRQ_USART1_UDRE,
		IRQ_USART1_TX
} irq_t ;

#endif /* IRQ_M324PA_H */
