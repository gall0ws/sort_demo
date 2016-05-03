/* $Id: sprite_impl.h 20 2009-08-25 12:31:12Z gallows $
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

#ifndef SPRITE_IMPL_H
#define SPRITE_IMPL_H

#ifndef _SPRITE_CHILD
# ifndef _SPRITE_INSIDE
#  error "Only "sprite.h" can be included."
# endif
#endif

#include "sprite.h"

#define SPRITE(OBJ)	((Sprite *)(OBJ))

struct _Sprite {
	LayerVT parent;

	/*< protected >*/
	SDL_Surface *screen;
	SDL_Surface *surface;
	SDL_Rect    dst;
};

void sprite_free(Sprite *self);
int  sprite_blit(Sprite *self);

u16  sprite_get_width  (const Sprite *self);
u16  sprite_get_height (const Sprite *self);
s16  sprite_get_x      (const Sprite *self);
s16  sprite_get_y      (const Sprite *self);

void sprite_set_width  (Sprite *self, u16 w);
void sprite_set_height (Sprite *self, u16 h);
void sprite_set_x      (Sprite *self, s16 x);
void sprite_set_y      (Sprite *self, s16 y);
void sprite_set_xy     (Sprite *self, s16 x, s16 y);

#endif /* !SPRITE_IMPL_H */
