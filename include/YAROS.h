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

#ifndef YAROS_H
#define YAROS_H

#include "kernel/def.h"
#include "kernel/errno.h"
#include "kernel/msg.h"
#include "kernel/panic.h"
#include "kernel/sched.h"
#include "kernel/sysclk.h"
#include "kernel/task.h"
#include "kernel/type.h"

#include "devices/adc.h"
#include "devices/clk.h"
#include "devices/eeprom.h"
#include "devices/irq.h"
#include "devices/twi.h"
#include "devices/usart.h"

#include "init/init.h"

#include "ipc/mutex.h"

#endif /* YAROS_H */
