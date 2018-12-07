/*
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

#ifndef CONFIG_H
#define CONFIG_H

/*
 * Include kconfig ONCE.
 */
#include "kconfig.h"

#ifdef CONFIG_KCLK
#  define KCLK CONFIG_KCLK
#else
#  error "CONFIG_CLK: Kernel Clock is not defined."
#endif

/*
 * Frequency at which the TIMER0_OVF_vect interrupt should be call.
 */
#ifdef CONFIG_HZ
#  define HZ CONFIG_HZ
#else
#  define HZ 100
#endif

#if CONFIG_JIFFY == 64
#  define JIFFY_TYPE uint64_t
#else
#  define JIFFY_TYPE uint32_t
#endif /* JIFFY_TYPE */

#if CONFIG_STACK_SIZE == 16
#  define STACK_SIZE_TYPE uint16_t
/* 2^16 - MIN_STACK_SIZE (40) */
#  define MAX_STACK_SIZE 65496
#else
#  define STACK_SIZE_TYPE uint8_t
/* 2^8 - MIN_STACK_SIZE (40) */
#  define MAX_STACK_SIZE 216
#endif /* STACK_SIZE_TYPE */

#ifdef CONFIG_F_CPU
#  define F_CPU CONFIG_F_CPU
#else
#  define F_CPU 8000000UL
#endif /* F_CPU */

#ifdef CONFIG_VERBOSE_LEVEL
#  define VERBOSE_LEVEL CONFIG_VERBOSE_LEVEL
#else
#  define VERBOSE_LEVEL 5
#endif	/* VERBOSE_LEVEL */

#endif /* CONFIG_H */
