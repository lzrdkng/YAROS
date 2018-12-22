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

#include "devices/irq.h"

typedef int irq_t;

#if 0
void
disable_irq(irq_t irq);
#endif

error_t
disable_irq_unsafe(irq_t irq)
{
    switch (irq) {

    case IRQ_INT0:
        EIMSK &= ~_BV(INT0);
        break;

    case IRQ_INT1:
        EIMSK &= ~_BV(INT1);
        break;

    case IRQ_INT2:
        EIMSK &= ~_BV(INT2);
        break;

    case IRQ_PCINT0:
        PCICR &= ~_BV(PCIE0);
        break;

    case IRQ_PCINT1:
        PCICR &= ~_BV(PCIE1);
        break;

    case IRQ_PCINT2:
        PCICR &= ~_BV(PCIE2);
        break;

    case IRQ_PCINT3:
        PCICR &= ~_BV(PCIE3);
        break;

    case IRQ_WDT:
        WDTCSR &= ~_BV(WDIE);
        break;

    case IRQ_TIMER2_COMPA:
        TIMSK2 &= ~_BV(OCIE2A);
        break;

    case IRQ_TIMER2_COMPB:
        TIMSK2 &= ~_BV(OCIE2B);
        break;

    case IRQ_TIMER2_OVF:
        TIMSK2 &= ~_BV(TOIE2);
        break;

    case IRQ_TIMER1_CAPT:
        TIMSK1 &= ~_BV(ICIE1);
        break;

    case IRQ_TIMER1_COMPA:
        TIMSK1 &= ~_BV(OCIE1A);
        break;

    case IRQ_TIMER1_COMPB:
        TIMSK1 &= ~_BV(OCIE1B);
        break;

    case IRQ_TIMER1_OVF:
        TIMSK1 &= ~_BV(TOIE1);
        break;

    case IRQ_TIMER0_COMPA:
        TIMSK0 &= ~_BV(OCIE0A);
        break;

    case IRQ_TIMER0_COMPB:
        TIMSK0 &= ~_BV(OCIE0B);
        break;

    case IRQ_TIMER0_OVF:
        TIMSK0 &= ~_BV(TOIE0);
        break;

    case IRQ_SPI_STC:
        SPCR0 &= ~_BV(SPIE0);
        break;

    case IRQ_USART0_RX:
        UCSR0B &= ~_BV(RXCIE0);
        break;

    case IRQ_USART0_UDRE:
        UCSR0B &= ~_BV(UDRIE0);
        break;

    case IRQ_USART0_TX:
        UCSR0B &= ~_BV(TXCIE0);
        break;

    case IRQ_ANALOG_COMP:
        ACSR &= ~_BV(ACIE);
        break;

    case IRQ_ADC:
        ADCSRA &= ~_BV(ADIE);
        break;

    case IRQ_EE_READY:
        EECR &= ~_BV(EERIE);
        break;

    case IRQ_TWI:
        TWCR &= ~_BV(TWIE);
        break;

    case IRQ_SPM_READY:
        SPMCSR &= ~_BV(SPMIE);
        break;

    case IRQ_USART1_RX:
        UCSR1B &= ~_BV(RXCIE1);
        break;

    case IRQ_USART1_UDRE:
        UCSR1B &= ~_BV(UDRIE1);
        break;

    case IRQ_USART1_TX:
        UCSR1B &= ~_BV(TXCIE1);
        break;

    default:
        return -ENOTSUP;
    }

    return OK;
}

error_t
enable_irq(irq_t irq)
{
    switch (irq) {

    case IRQ_INT0:
        EIMSK |= _BV(INT0);
        break;

    case IRQ_INT1:
        EIMSK |= _BV(INT1);
        break;

    case IRQ_INT2:
        EIMSK |= _BV(INT2);
        break;

    case IRQ_PCINT0:
        PCICR |= _BV(PCIE0);
        break;

    case IRQ_PCINT1:
        PCICR |= _BV(PCIE1);
        break;

    case IRQ_PCINT2:
        PCICR |= _BV(PCIE2);
        break;

    case IRQ_PCINT3:
        PCICR |= _BV(PCIE3);
        break;

    case IRQ_WDT:
        WDTCSR |= _BV(WDIE);
        break;

    case IRQ_TIMER2_COMPA:
        TIMSK2 |= _BV(OCIE2A);
        break;

    case IRQ_TIMER2_COMPB:
        TIMSK2 |= _BV(OCIE2B);
        break;

    case IRQ_TIMER2_OVF:
        TIMSK2 |= _BV(TOIE2);
        break;

    case IRQ_TIMER1_CAPT:
        TIMSK1 |= _BV(ICIE1);
        break;

    case IRQ_TIMER1_COMPA:
        TIMSK1 |= _BV(OCIE1A);
        break;

    case IRQ_TIMER1_COMPB:
        TIMSK1 |= _BV(OCIE1B);
        break;

    case IRQ_TIMER1_OVF:
        TIMSK1 |= _BV(TOIE1);
        break;

    case IRQ_TIMER0_COMPA:
        TIMSK0 |= _BV(OCIE0A);
        break;

    case IRQ_TIMER0_COMPB:
        TIMSK0 |= _BV(OCIE0B);
        break;

    case IRQ_TIMER0_OVF:
        TIMSK0 |= _BV(TOIE0);
        break;

    case IRQ_SPI_STC:
        SPCR0 |= _BV(SPIE0);
        break;

    case IRQ_USART0_RX:
        UCSR0B |= _BV(RXCIE0);
        break;

    case IRQ_USART0_UDRE:
        UCSR0B |= _BV(UDRIE0);
        break;

    case IRQ_USART0_TX:
        UCSR0B |= _BV(TXCIE0);
        break;

    case IRQ_ANALOG_COMP:
        ACSR |= _BV(ACIE);
        break;

    case IRQ_ADC:
        ADCSRA |= _BV(ADIE);
        break;

    case IRQ_EE_READY:
        EECR |= _BV(EERIE);
        break;

    case IRQ_TWI:
        TWCR |= _BV(TWIE);
        break;

    case IRQ_SPM_READY:
        SPMCSR |= _BV(SPMIE);
        break;

    case IRQ_USART1_RX:
        UCSR1B |= _BV(RXCIE1);
        break;

    case IRQ_USART1_UDRE:
        UCSR1B |= _BV(UDRIE1);
        break;

    case IRQ_USART1_TX:
        UCSR1B |= _BV(TXCIE1);
        break;

    default:
        return -ENOTSUP;

    }

    return OK;
}

#if 0
void
enable_irq_unsafe(irq_t irq);
#endif
