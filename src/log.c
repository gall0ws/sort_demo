/* $Id: log.c 18 2009-08-23 18:26:06Z gallows $
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

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "log.h"

#define DO_LOG(TYPE)							\
	va_list ap;							\
	va_start(ap, fmt);						\
	do_log(TYPE, fmt, ap);						\
	va_end(ap)

enum log_types {
	LOG_ERROR,
	LOG_WARNING,
	LOG_FIXME,
	LOG_DEBUG,
	LOG_INFO,
};

static void do_log(int type, const char *fmt, va_list ap)
{
	const char *prefix;

	switch (type) {
	case LOG_WARNING:
		prefix = " [WARNING] ";
		break;

	case LOG_ERROR:
		prefix = " [ERROR] ";
		break;
		
	case LOG_FIXME:
		prefix = " [FIXME] ";
		break;

	case LOG_DEBUG:
		prefix = " [DEBUG] ";
		break;

	case LOG_INFO:
		prefix = " [**] ";
		break;

	default:
		prefix = NULL;
	}

	fprintf(stderr, prefix);
	vfprintf(stderr, fmt, ap);
	fputc('\n', stderr);
}

void log_err(const char *fmt, ...)
{
	DO_LOG(LOG_ERROR);
}

void log_warn(const char *fmt, ...)
{
	DO_LOG(LOG_WARNING);
}

void log_fixme(const char *fmt, ...)
{
	DO_LOG(LOG_FIXME);
}

void log_debug(const char *fmt, ...)
{
	DO_LOG(LOG_DEBUG);
}

void log_info(const char *fmt, ...)
{
	DO_LOG(LOG_INFO);
}
