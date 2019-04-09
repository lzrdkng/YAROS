#ifndef YR_ARCH_ATOMIC_H
#define YR_ARCH_ATOMIC_H

#include "kernel/def.h"

typedef struct {
	u8 __atom;
} atomic8_t;
typedef atomic8_t a8;

typedef struct {
	u16 __atom;
} atomic16_t;
typedef atomic16_t a16;

typedef struct {
	u32 __atom;
} atomic32_t;
typedef atomic32_t a32;

typedef struct {
	u64 __atom;
} atomic64_t;
typedef atomic64_t a64;


extern u8 atomic8_read(const atomic8_t *atom);
extern u8 atomic8_read_C(const atomic8_t *atom);

extern u16 atomic16_read(const atomic16_t *atom);
extern u16 atomic16_read_C(const atomic16_t *atom);

extern u32 atomic32_read(const atomic32_t *atom);
extern u32 atomic32_read_C(const atomic32_t *atom);

extern u64 atomic64_read(const atomic64_t *atom);
extern u64 atomic64_read_C(const atomic64_t *atom);


extern u8 atomic8_write(atomic8_t *atomtom, u8 value);
extern u8 atomic8_write_C(atomic8_t *atomtom, u8 value);

extern u16 atomic16_write(atomic16_t *atom, u16 value);
extern u16 atomic16_write_C(atomic16_t *atom, u16 value);

extern u32 atomic32_write(atomic32_t *atom, u32 value);
extern u32 atomic32_write_C(atomic32_t *atom, u32 value);

extern u64 atomic64_write(atomic64_t *atom, u64 value);
extern u64 atomic64_write_C(atomic64_t *atom, u64 value);


extern u8 atomic8_inc(atomic8_t *atom);
extern u8 atomic8_inc_C(atomic8_t *atom);

extern u16 atomic16_inc(atomic16_t *atom);
extern u16 atomic16_inc_C(atomic16_t *atom);

extern u32 atomic32_inc(atomic32_t *atom);
extern u32 atomic32_inc_C(atomic32_t *atom);

extern u64 atomic64_inc(atomic64_t *atom);
extern u64 atomic64_inc_C(atomic64_t *atom);


#endif
