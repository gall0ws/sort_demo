/* $Id: stdinc.h 20 2009-08-25 12:31:12Z gallows $
 *
 * Copyright (c) 2009 Sergio Perticone <g4ll0ws@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef STDINC_H
#define STDINC_H

#if HAVE_CONFIG_H
# include <config.h>
#endif

#if HAVE_STDBOOL_H
# include <stdbool.h>
#else
# define NEED_BOOL
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;

typedef int8_t		s8;
typedef int16_t		s16;
typedef int32_t		s32;

#ifdef NEED_BOOL
typedef unsigned char   bool;
#endif

#ifndef __GNUC__
# define __attribute__(X)	/* nothing */
#endif

#define INLINE		__attribute__((always_inline))
#define INLINE_METHOD	INLINE
#define SENTINEL	__attribute__((sentinel))
#define CHECK_FMT	__attribute__((format(printf, 1, 2)))
#define CHECK_FMT2	__attribute__((format(printf, 2, 3)))

#endif /* !STDINC_H */
