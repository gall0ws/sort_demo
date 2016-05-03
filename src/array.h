/* $Id: array.h 20 2009-08-25 12:31:12Z gallows $
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

/*
 * Object Hierarchy:
 *
 *  Object
 *   +----Array (final)
 */

#ifndef ARRAY_H
#define ARRAY_H

#include "object.h"

typedef enum {
	BUBBLE_SORT,
	SELECTION_SORT,
	INSERTION_SORT,
	QUICK_SORT,
	HEAP_SORT,
} SortType;

typedef enum {
	CASE_RANDOM,
	CASE_REVERSED,
	CASE_NEARLY_SORTED,
	CASE_FEW_UNIQUE,
} SortCase;

typedef struct _Array Array;
typedef void (*Callback)(Array *);

/*
 * Array ctor.
 */
Array *array_new (void);

/*
 * Perform array sort using `algo' algorithm for the given case `kase'.
 */
void array_sort (Array *self, SortType algo, SortCase kase);

/*
 * Stop sorting.
 * (You can safely call it even if no sort-session is running).
 */
void array_stop_sorting (Array *self);

/*
 * Set the callback function. 
 * When array will be sorted, the object will emit a signal, here you can
 * set the function to catch it.
 * The function will be invoked in a separate thread.
 */
void array_set_callback (Array *self, Callback f);

#endif /* !ARRAY_H */
