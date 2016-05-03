/* $Id: menu.c 20 2009-08-25 12:31:12Z gallows $
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

#include "sprite.h"
#include "menu.h"

#define SELECTOR_WIDTH		350
#define SELECTOR_HEIGHT		42
#define SELECTOR_X		72
#define SELECTOR_Y		146
#define SELECTOR_STEP		48
#define SELECTOR_COLOR		0x00ff55

#define SELECTOR0_MAX_VAL	4
#define SELECTOR1_MAX_VAL	3

#define ALPHA_COLOR		0xff00ff
#define BG_COLOR		0x0f0f0f

#define MENU0_FILENAME	"menu0.bmp"
#define MENU1_FILENAME	"menu1.bmp"

struct _Menu {
	ObjectVT parent;

	Sprite *menu;
	Sprite *selector;

	u8 value;
	u8 type;
};

static void _menu_select(Menu *self, u8 idx)
{
	while (self->value > idx)
		menu_up(self);

	while (self->value < idx)
		menu_down(self);
}

INLINE_METHOD static void menu_free(Menu *self)
{
	objects_free(self->menu, self->selector, NULL);
}

INLINE_METHOD static int menu_blit(Menu *self)
{
 	return objects_blit(self->menu, self->selector, NULL);
}

Menu *menu_new(int type)
{
	char path[PATH_MAX];
	Menu *self;

	self = calloc(1, sizeof(Menu));

	OBJECT(self)->vtable.dtor = (pfDtor)menu_free;
	OBJECT(self)->vtable.blit = (pfBlit)menu_blit;

	self->type = type;
	self->selector= sprite_new(SELECTOR_WIDTH, SELECTOR_HEIGHT);
	sprite_fill(self->selector, SELECTOR_COLOR); 
	layer_set_xy(self->selector, SELECTOR_X, SELECTOR_Y);

	join_path(engine_get_datadir(), 
		  type ? MENU1_FILENAME : MENU0_FILENAME, path);

	self->menu = sprite_new_from_file(path);

	sprite_set_accel(self->menu, 0x0f0f0f);
 	sprite_set_alpha(self->selector, 64);

	return self;
}

bool menu_up(Menu *self)
{
	s16 y;

	if (self->value == 0)
		return 0;

	y = layer_get_y(self->selector);
	layer_set_y(self->selector, y - SELECTOR_STEP);
	--self->value;

	return 1;
}

bool menu_down(Menu *self)
{
	s16 y = layer_get_y(self->selector);

	if (self->type == 0) {
		if (self->value < SELECTOR0_MAX_VAL) {
			layer_set_y(self->selector, y + SELECTOR_STEP);

			++self->value;
		}
		else {
			return 0;
		}
	}
	else {
		if (self->value < SELECTOR1_MAX_VAL) {
			layer_set_y(self->selector, y + SELECTOR_STEP);
			++self->value;
		}
		else {
			return 0;
		}
	}

	return 1;
}

bool menu_select(Menu *self, u16 x, u16 y)
{
	int i;
	u16 sy, lim;

	if (x < SELECTOR_X || x >= SELECTOR_X + SELECTOR_WIDTH)
		return 0;

	if (self->type == 0)
		lim = SELECTOR0_MAX_VAL;
	else
		lim = SELECTOR1_MAX_VAL;

	for (i=0; i<=lim; ++i) {
		sy = SELECTOR_Y + SELECTOR_STEP * i;
		if (y >= sy && y < sy + SELECTOR_HEIGHT)
			break;
	}

	if (i > lim)
		return 0;

	_menu_select(self, i);

	return 1;
}

INLINE_METHOD u8 menu_get_value(const Menu *self)
{
	return self->value;
}

