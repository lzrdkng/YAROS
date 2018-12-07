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

#include "YAROS/def.h"
#include "YAROS/errno.h"
#include "YAROS/msg.h"
#include "YAROS/panic.h"
#include "YAROS/sched.h"
#include "YAROS/suspend.h"
#include "YAROS/sysclk.h"
#include "YAROS/task.h"
#include "YAROS/type.h"

#include "devices/adc.h"
#include "devices/clk.h"
#include "devices/eeprom.h"
#include "devices/irq.h"
#include "devices/twi.h"
#include "devices/usart.h"

#include "init/init.h"

#include "ipc/mutex.h"

#endif /* YAROS_H */
