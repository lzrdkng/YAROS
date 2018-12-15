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

#ifndef YAROS_ERRNO_H
#define YAROS_ERRNO_H

typedef enum {
  OK = 0,
  ENOTSUP,						/*< Opperation Not Supported */
  EBUSY,						/*< Device is Busy */
  EBDEV,						/*< Bad Device */
  ECOMM,						/*< Error of Communication on Send */
  EINV,							/*< Invalid Value */
  E2BIG,						/*< Value too big */
  ESLEEP,                       /*< Task is sleeping */
} error_t;

#endif /* YAROS_ERRNO_H */
