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

#include "kernel/msg.h"

#if VERBOSE_LEVEL >= 5
const char DEBUG_HEADER[] PROGMEM = GREEN("[DEBUG] ");

void
__send_debug_header()
{
    char buff[1];

    for (U8 i = 0; i < sizeof(DEBUG_HEADER); ++i) {
        memcpy_P(buff, &DEBUG_HEADER[i], 1);
        write_usart(0, buff, 1);
    }

}
#endif

#if VERBOSE_LEVEL >= 4
const char INFO_HEADER[] PROGMEM = PURPLE("[INFO] ");

void
__send_info_header()
{
    char buff[1];

    for (U8 i = 0; i < sizeof(INFO_HEADER); ++i) {
        memcpy_P(buff, &INFO_HEADER[i], 1);
        write_usart(0, buff, 1);
    }

}
#endif


#if VERBOSE_LEVEL >= 3
const char WARNING_HEADER[] PROGMEM = YELLOW("[WARNING] ");

void
__send_warning_header()
{
    char buff[1];

    for (U8 i = 0; i < sizeof(WARNING_HEADER); ++i) {
        memcpy_P(buff, &WARNING_HEADER[i], 1);
        write_usart(0, buff, 1);
    }

}
#endif


#if VERBOSE_LEVEL >= 2
const char ERROR_HEADER[] PROGMEM = RED("[ERROR] ");

void
__send_error_header()
{
    char buff[1];

    for (U8 i = 0; i < sizeof(ERROR_HEADER); ++i) {
        memcpy_P(buff, &ERROR_HEADER[i], 1);
        write_usart(0, buff, 1);
    }

}
#endif

#if VERBOSE_LEVEL >= 1
const char FATAL_HEADER[] PROGMEM = LIGHT_RED("[FATAL] ");
const char NL[] PROGMEM = {'\n'};

void
__send_fatal_header()
{
    char buff[1];

    for (U8 i = 0; i < sizeof(FATAL_HEADER); ++i) {
        memcpy_P(buff, &FATAL_HEADER[i], 1);
        write_usart(0, buff, 1);
    }

}

void __send_nl()
{
    char buff[1];
    memcpy_P(buff, NL, 1);
    write_usart(0, buff, 1);
}
#endif
