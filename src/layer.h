/* $Id: layer.h 21 2009-08-28 23:44:56Z gallows $
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
 *   +----Layer (abstract)
 */

#ifndef LAYER_H
#define LAYER_H

#include "object.h"

#define LAYER(OBJ)	((LayerVT *)(OBJ))

typedef void Layer;

typedef bool (* pfOwn)  (const Layer *, s16, s16);
typedef int  (* pfGet)  (const Layer *);
typedef void (* pfSet)  (Layer *, int);
typedef void (* pfSet2) (Layer *, int, int);

struct _LayerVT {
	ObjectVT parent;

	/*< protected >*/
	struct {
		pfOwn  own;
		pfGet  get_width;
		pfGet  get_height;
		pfGet  get_x;
		pfGet  get_y;
		pfSet  set_width;
		pfSet  set_height;
		pfSet  set_x;
		pfSet  set_y;
		pfSet2 set_xy;
	} vtable;
};

typedef struct _LayerVT LayerVT;

bool layer_own	      (const Layer *self, s16 x, s16 y);/* pure virtual */

u16  layer_get_width  (const Layer *self);		/* pure virtual */
u16  layer_get_height (const Layer *self);		/* pure virtual */
s16  layer_get_x      (const Layer *self);		/* pure virtual */
s16  layer_get_y      (const Layer *self);		/* pure virtual */

void layer_set_width  (Layer *self, u16 w);		/* pure virtual */
void layer_set_height (Layer *self, u16 h);		/* pure virtual */
void layer_set_x      (Layer *self, s16 x);		/* pure virtual */
void layer_set_y      (Layer *self, s16 y);		/* pure virtual */
void layer_set_xy     (Layer *self, s16 x, s16 y);	/* pure virtual */

#endif /* !LAYER_H */
