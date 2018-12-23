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

#ifndef YAROS_EEPROM_H
#define YAROS_EEPROM_H

#include "kernel/def.h"

error_t init_eeprom(int dev);

error_t fini_eeprom(int dev);

ssize_t write_eeprom(int dev, const void *buff, size_t len);

ssize_t read_eeprom(int dev, void *buff, size_t len);

#endif /* YAROS_EEPROM_H */
