/* SPDX-License-Identifier: GPL-2.0 OR MIT */
/*
 * Copyright (C) 2015-2018 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.
 */

#ifndef _ZINC_CURVE25519_H
#define _ZINC_CURVE25519_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifndef __BYTE_ORDER__
#include <sys/param.h>
#if !defined(BYTE_ORDER) || !defined(BIG_ENDIAN) || !defined(LITTLE_ENDIAN)
#error "Unable to determine endianness."
#endif
#define __BYTE_ORDER__ BYTE_ORDER
#define __ORDER_BIG_ENDIAN__ BIG_ENDIAN
#define __ORDER_LITTLE_ENDIAN__ LITTLE_ENDIAN
#endif

typedef uint64_t u64;
typedef uint32_t u32, __le32;
typedef unsigned char u8;
typedef int64_t s64;

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define le64_to_cpup(a) __builtin_bswap64(*(a))
#define le32_to_cpup(a) __builtin_bswap32(*(a))
#define cpu_to_le64(a) __builtin_bswap64(a)
#else
#define le64_to_cpup(a) (*(a))
#define le32_to_cpup(a) (*(a))
#define cpu_to_le64(a) (a)
#endif
#define get_unaligned_le32(a) le32_to_cpup((u32 *)(a))
#define get_unaligned_le64(a) le64_to_cpup((u64 *)(a))
#define put_unaligned_le64(s, d) *(u64 *)(d) = cpu_to_le64(s)
#ifndef __always_inline
#define __always_inline __inline __attribute__((__always_inline__))
#endif
#ifndef noinline
#define noinline __attribute__((noinline))
#endif
#ifndef __aligned
#define __aligned(x) __attribute__((aligned(x)))
#endif
#ifndef __force
#define __force
#endif
#define normalize_secret(a) curve25519_normalize_secret(a)

static noinline void memzero_explicit(void *s, size_t count)
{
	memset(s, 0, count);
	asm volatile("": :"r"(s) :"memory");
}
//#include <linux/types.h>

enum curve25519_lengths {
	CURVE25519_KEY_SIZE = 32
};

bool curve25519(unsigned char mypublic[CURVE25519_KEY_SIZE],
			     const unsigned char secret[CURVE25519_KEY_SIZE],
			     const unsigned char basepoint[CURVE25519_KEY_SIZE]);
void curve25519_generate_secret(
	unsigned char secret[CURVE25519_KEY_SIZE]);
bool curve25519_generate_public(
	unsigned char pub[CURVE25519_KEY_SIZE], const unsigned char secret[CURVE25519_KEY_SIZE]);

#endif /* _ZINC_CURVE25519_H */
