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

#ifndef MSG_H
#define MSG_H

#include <avr/pgmspace.h>
#include <string.h>

#include "YAROS/def.h"
#include "YAROS/errno.h"

#include "devices/usart.h"

#define COLOR_NC "\e[0m"
#define COLOR_WHITE "\e[1;37m"
#define COLOR_BLACK "\e[0;30m"
#define COLOR_BLUE "\e[0;34m"
#define COLOR_LIGHT_BLUE "\e[1;34m"
#define COLOR_GREEN "\e[0;32m"
#define COLOR_LIGHT_GREEN "\e[1;32m"
#define COLOR_CYAN "\e[0;36m"
#define COLOR_LIGHT_CYAN "\e[1;36m"
#define COLOR_RED "\e[0;31m"
#define COLOR_LIGHT_RED "\e[1;31m"
#define COLOR_PURPLE "\e[0;35m"
#define COLOR_LIGHT_PURPLE "\e[1;35m"
#define COLOR_BROWN "\e[0;33m"
#define COLOR_YELLOW "\e[1;33m"
#define COLOR_GRAY "\e[0;30m"
#define COLOR_LIGHT_GRAY "\e[0;37m"

#define GREEN(STR) COLOR_GREEN STR COLOR_NC
#define PURPLE(STR) COLOR_PURPLE STR COLOR_NC
#define YELLOW(STR) COLOR_YELLOW STR COLOR_NC
#define LIGHT_RED(STR) COLOR_LIGHT_RED STR COLOR_NC
#define RED(STR) COLOR_RED STR COLOR_NC

/*
 * Empty expansion by default
 */
#define DEBUG(STR) ({do {} while (0);})
#define INFO(STR) ({do {} while (0);})
#define WARNING(STR) do {} while (0); ({do {} while (0);})
#define ERROR(STR) ({do {} while (0);})
#define FATAL(STR) ({do {} while (0);})

#define MSG_FMT(STR) __FILE__ ":" xstr(__LINE__) ":" STR

extern const char NL[] PROGMEM;
extern void __send_debug_header();
extern void __send_info_header();
extern void __send_warning_header();
extern void __send_error_header();
extern void __send_fatal_header();
extern void __send_nl();

#if VERBOSE_LEVEL >= 5

#undef DEBUG
#define DEBUG(STR) \
	({			   \
          static const char __DEBUG_STR__[] PROGMEM = MSG_FMT(STR);     \
	char __debug_buff__[1];									\
	__send_debug_header();												\
	for (U16 __debug_i__=0; __debug_i__<sizeof(__DEBUG_STR__); ++__debug_i__) { \
	memcpy_P(__debug_buff__, &__DEBUG_STR__[__debug_i__], 1);				\
	write_usart(0, __debug_buff__, 1); \
	} \
	__send_nl();								\
	})
#endif

#if VERBOSE_LEVEL >= 4
#undef INFO
#define INFO(STR) \
	({			   \
	static const char __INFO_STR__[] PROGMEM = MSG_FMT(STR); \
	char __info_buff__[1];									\
	__send_info_header();												\
	for (U16 __info_i__=0; __info_i__<sizeof(__INFO_STR__); ++__info_i__) { \
	memcpy_P(__info_buff__, &__INFO_STR__[__info_i__], 1);				\
	write_usart(0, __info_buff__, 1); \
	} \
	__send_nl();								\
	})
#endif


#if VERBOSE_LEVEL >= 3
#undef WARNING
#define WARNING(STR) \
	({			   \
	static const char __WARNING_STR__[] PROGMEM =  MSG_FMT(STR);     \
	char __warning_buff__[1];									\
	__send_warning_header();											\
	for (U16 __warning_i__=0; __warning_i__<sizeof(__WARNING_STR__); ++__warning_i__) { \
	memcpy_P(__warning_buff__, &__WARNING_STR__[__warning_i__], 1);				\
	write_usart(0, __warning_buff__, 1); \
	} \
	__send_nl();								\
	})
#endif

#if VERBOSE_LEVEL >= 2
#undef ERROR
#define ERROR(STR) \
	({			   \
	static const char __ERROR_STR__[] PROGMEM =  MSG_FMT(STR);     \
	char __error_buff__[1];									\
	__send_error_header();												\
	for (U16 __error_i__=0; __error_i__<sizeof(__ERROR_STR__); ++__error_i__) { \
	memcpy_P(__error_buff__, &__ERROR_STR__[__error_i__], 1);				\
	write_usart(0, __error_buff__, 1); \
	} \
	__send_nl(); \
	})
#endif


#if VERBOSE_LEVEL >= 1
#undef FATAL
#define FATAL(STR) \
	({			   \
	static const char __FATAL_STR__[] PROGMEM = MSG_FMT(STR);     \
	char __fatal_buff__[1];									\
	__send_fatal_header();												\
	for (U16 __fatal_i__=0; __fatal_i__<sizeof(__FATAL_STR__); ++__fatal_i__) { \
	memcpy_P(__fatal_buff__, &__FATAL_STR__[__fatal_i__], 1);				\
	write_usart(0, __fatal_buff__, 1); \
	} \
	__send_nl(); \
	})
#endif

#endif /* MSG_H */
