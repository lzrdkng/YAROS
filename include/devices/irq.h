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

#ifndef IRQ_H
#define IRQ_H

#include "YAROS/def.h"
#include "YAROS/errno.h"

#if defined(__AVR_ATmega324PA__)
#  include "drivers/irq/irq_atmega324pa.h"
#else
#  error "Architecture not supported!"
#endif

error_t
disable_irq(irq_t irq);

error_t
disable_irq_unsafe(irq_t irq);

error_t
enable_irq(irq_t irq);

error_t
enable_irq_unsafe(irq_t irq);
#endif /* IRQ_H */
