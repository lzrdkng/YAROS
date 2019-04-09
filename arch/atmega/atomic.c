/*
 * arch/atmega/atomic.c - Atomic type for atmega architecture.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 *
 * There's 4 different atomic types:
 *
 * - atomic8_t/a8
 * - atomic16_t/a16
 * - atomic32_t/a32
 * - atomic64_t/a64
 *
 * All atomic operations should be optimized for the lowest non
 * premptive time.  i.e., that the number of instruction between the
 * clear interupt and set interupt is the smallest possible.
 *
 * Every operations should have a second version with the suffix _C.
 * The C version should never be used other than for testing the
 * assembly version.
 *
 * Don't use the GCC extension to write assembly with inputs/ouputs
 * lists and clobbered registers.  It has a terrible syntax and nobody
 * understand how to use it really.  Write pure assembly in your
 * function body with the __naked attribute for the function and
 * __unused for the parameter(s).
 */

#include <util/atomic.h>

#include "arch/atomic.h"

#include "kernel/def.h"
#include "kernel/sched.h"


#define do_atomic_read_C(T)						\
	u##T atomic##T##_read_C(const atomic##T##_t *a)			\
	{								\
		u##T retval;						\
		ATOMIC_BLOCK(ATOMIC_FORCEON) {				\
			retval = a->__atom;				\
		}							\
		return retval;						\
	}

#define do_atomic_inc_C(T)					\
	u##T atomic##T##_inc_C(atomic##T##_t *a)	\
	{							\
		u##T retval;					\
		ATOMIC_BLOCK(ATOMIC_FORCEON)			\
			retval = ++(a->__atom);			\
		return retval;					\
	}



do_atomic_read_C(8);
do_atomic_read_C(16);
do_atomic_read_C(32);
do_atomic_read_C(64);

do_atomic_inc_C(8);
do_atomic_inc_C(16);
do_atomic_inc_C(32);
do_atomic_inc_C(64);


__naked
u8 atomic8_read(const atomic8_t __unused *a)
{
	asm (
		"movw r30, r24\n\t"
		"ld r24, Z\n\t"
		"ret"
		);
}

__naked
u16 atomic16_read(const atomic16_t __unused *a)
{
	asm volatile(
		"movw r30, r24\n\t"
		"cli\n\t"
		"ld r24, Z\n\t"
		"ldd r25, Z+1\n\t"
		"reti\n\t"
		);
}

__naked
u32 atomic32_read(const atomic32_t __unused *a)
{
	asm volatile(
		"movw r30, r24\n\t"
		"cli\n\t"
		"ld r22, Z\n\t"
		"ldd r23, Z+1\n\t"
		"ldd r24, Z+2\n\t"
		"ldd r25, Z+3\n\t"
		"reti"
		);
}

__naked
u64 atomic64_read(const atomic64_t __unused *a)
{
	asm volatile(
		"movw r30, r24\n\t"
		"cli\n\t"
		"ld r18, Z\n\t"
		"ldd r19, Z+1\n\t"
		"ldd r20, Z+2\n\t"
		"ldd r21, Z+3\n\t"
		"ldd r22, Z+4\n\t"
		"ldd r23, Z+5\n\t"
		"ldd r24, Z+6\n\t"
		"ldd r25, Z+7\n\t"
		"reti"
		);
}


__naked
u8 atomic8_inc(atomic8_t __unused *a)
{
	asm volatile(
		"movw r30, r24\n\t"
		"cli\n\t"
		"ld r24, Z\n\t"
		"inc r24\n\t"
		"reti"
		);
}
