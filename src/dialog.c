/* $Id: dialog.c 20 2009-08-25 12:31:12Z gallows $
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

#include <limits.h>

#include "engine.h"
#include "video.h"

#include "sprite.h"
#include "dialog.h"

#define PANEL_COLOR		0x0a0a0a
#define PANEL_ALPHA		92

#define DIALOG_FILENAME		"dialog.bmp"
#define DIALOG_ALPHA		250
#define DIALOG_Y_PAD		35

#define SELECTOR_WIDTH		70
#define SELECTOR_HEIGHT		32
#define SELECTOR_X	        280
#define SELECTOR_Y		(215 - DIALOG_Y_PAD)
#define SELECTOR_STEP		135
#define SELECTOR_COLOR		0xff0000
#define SELECTOR_ALPHA		64

struct _Dialog {
	ObjectVT parent;

	Sprite *panel;
	Sprite *dialog;
	Sprite *selector;

	u8 value;
};

INLINE_METHOD static void dialog_free(Dialog *self)
{
	objects_free(self->panel, self->dialog, self->selector, NULL);

}

INLINE_METHOD static int dialog_blit(Dialog *self)
{
	return objects_blit(self->panel, self->dialog, self->selector, NULL);
}

Dialog *dialog_new(void)
{
	char path[PATH_MAX];
	Dialog *self;
	u16 w, h;

	self = calloc(1, sizeof(Dialog));

	OBJECT(self)->vtable.dtor = (pfDtor)dialog_free;
	OBJECT(self)->vtable.blit = (pfBlit)dialog_blit;

	w = video_get_width();
	h = video_get_height();

	self->panel = sprite_new(w, h);
	sprite_fill(self->panel, PANEL_COLOR);
	sprite_set_alpha(self->panel, PANEL_ALPHA);

	join_path(engine_get_datadir(), DIALOG_FILENAME, path);

	self->dialog = sprite_new_from_file(path);;
	sprite_set_alpha(self->dialog, DIALOG_ALPHA);
	layer_set_xy(self->dialog, (w - layer_get_width(self->dialog)) / 2,
		     (h - layer_get_height(self->dialog)) / 2 - DIALOG_Y_PAD);

	self->selector= sprite_new(SELECTOR_WIDTH, SELECTOR_HEIGHT);
	sprite_fill(self->selector, SELECTOR_COLOR); 
	sprite_set_alpha(self->selector, SELECTOR_ALPHA);
	layer_set_xy(self->selector, SELECTOR_X, SELECTOR_Y);

	return self;
}

bool dialog_left(Dialog *self)
{
	if (self->value)
		return 0;

	layer_set_x(self->selector, SELECTOR_X - SELECTOR_STEP);
	++self->value;

	return 1;
}

bool dialog_right(Dialog *self)
{
	if (!self->value)
		return 0;

	layer_set_x(self->selector, SELECTOR_X);
	--self->value;

	return 1;
}

bool dialog_select(Dialog *self, u16 x, u16 y)
{
	if (y < SELECTOR_Y || y >= SELECTOR_Y + SELECTOR_HEIGHT)
		return 0;

	if (x >= SELECTOR_X && x < SELECTOR_X + SELECTOR_WIDTH) {
		dialog_right(self);
		return 1;
	}
	else if (x >= SELECTOR_X - SELECTOR_STEP &&
		 x < SELECTOR_X - SELECTOR_STEP + SELECTOR_WIDTH) {
		dialog_left(self);
		return 1;
	}

	return 0;
}

INLINE_METHOD u8 dialog_get_value(const Dialog *self)
{
	return self->value;
}
