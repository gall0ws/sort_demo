/* $Id: sprite.c 18 2009-08-23 18:26:06Z gallows $
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

#define _SPRITE_INSIDE

#include <errno.h>

#include "video.h"
#include "sprite_impl.h"

INLINE_METHOD void sprite_free(Sprite *self)
{
	if (self && self->surface && self->surface != self->screen)
		SDL_FreeSurface(self->surface);
}

INLINE_METHOD int sprite_blit(Sprite *self)
{
	int retv;

	retv = SDL_BlitSurface(self->surface, NULL, self->screen, &self->dst);
	if (retv == -1)
		log_err("blit failed: %s", SDL_GetError());

	return retv;
}

INLINE_METHOD bool sprite_own(const Sprite *self, s16 x, s16 y)
{
	return !(self->dst.x > x || self->dst.y > y ||
		 self->dst.x + self->dst.w < x || 
		 self->dst.y + self->dst.h < y);
}

INLINE_METHOD u16 sprite_get_width(const Sprite *self)
{
	return self->dst.w;
}

INLINE_METHOD u16 sprite_get_height(const Sprite *self)
{
	return self->dst.h;
}

INLINE_METHOD s16 sprite_get_x(const Sprite *self)
{
	return self->dst.x;
}

INLINE_METHOD s16 sprite_get_y(const Sprite *self)
{
	return self->dst.y;
}

INLINE_METHOD void sprite_set_width(Sprite *self, u16 w)
{
	self->dst.w = w;
}

INLINE_METHOD void sprite_set_height(Sprite *self, u16 h)
{
	self->dst.h = h;
}

INLINE_METHOD void sprite_set_x(Sprite *self, s16 x)
{
	self->dst.x = x;
}

INLINE_METHOD void sprite_set_y(Sprite *self, s16 y)
{
	self->dst.y = y;
}

INLINE_METHOD void sprite_set_xy(Sprite *self, s16 x, s16 y)
{
	self->dst.x = x;
	self->dst.y = y;
}

static Sprite *_sprite_new(SDL_Surface *screen, SDL_Surface *surface)
{
	Sprite *self;

	self = calloc(1, sizeof(Sprite));
	OBJECT(self)->vtable.dtor = (pfDtor)sprite_free;
	OBJECT(self)->vtable.blit = (pfBlit)sprite_blit;
	LAYER(self)->vtable.own = (pfOwn)sprite_own;
	LAYER(self)->vtable.get_width = (pfGet)sprite_get_width;
	LAYER(self)->vtable.get_height = (pfGet)sprite_get_height;
	LAYER(self)->vtable.get_x = (pfGet)sprite_get_x;
	LAYER(self)->vtable.get_y = (pfGet)sprite_get_y;
	LAYER(self)->vtable.set_width = (pfSet)sprite_set_width;
	LAYER(self)->vtable.set_height = (pfSet)sprite_set_height;
	LAYER(self)->vtable.set_x = (pfSet)sprite_set_x;
	LAYER(self)->vtable.set_y = (pfSet)sprite_set_y;
	LAYER(self)->vtable.set_xy = (pfSet2)sprite_set_xy;

	self->screen = screen;

	if (surface) {
		self->surface = surface;
		self->dst.w = surface->w;
		self->dst.h = surface->h;
	}

	return self;
}

Sprite *sprite_new(u16 width, u16 height)
{
	SDL_Surface *screen, *surface = NULL;

	screen = SDL_GetVideoSurface();
	if (!screen) {
		log_err("could not obtain video surface: %s", SDL_GetError());
		return NULL;
	}

	if (width && height) {
		surface = SDL_CreateRGBSurface(video_get_flags(),
					       width, height,
					       screen->format->BitsPerPixel,
					       screen->format->Rmask,
					       screen->format->Gmask,
					       screen->format->Bmask,
					       screen->format->Amask);

		if (!surface) {
			log_err("could not create rgb surface: %s", 
				SDL_GetError());
			return NULL;
		}
	}

	return _sprite_new(screen, surface);
}

Sprite *sprite_new_from_file(const char *file)
{
	SDL_Surface *tmp, *surface, *screen;
	
	screen = SDL_GetVideoSurface();
	if (!screen) {
		log_err("could not obtain video surface: %s", SDL_GetError());
		return NULL;
	}

	tmp = SDL_LoadBMP(file);
	if (!tmp) {
		log_err("could not load image: %s", SDL_GetError());
		return NULL;
	}
	
	surface = SDL_ConvertSurface(tmp, screen->format, video_get_flags());

	SDL_FreeSurface(tmp);

	if (!surface) {
		log_err("could not create surface: %s", SDL_GetError());
		return NULL;
	}

	return _sprite_new(screen, surface);
}

Sprite *sprite_new_from_sdl(SDL_Surface *surface)
{
	SDL_Surface *screen;

	screen = SDL_GetVideoSurface();
	if (!screen) {
		log_err("could not obtain video surface: %s", SDL_GetError());
		return NULL;
	}

	return _sprite_new(screen, surface);
}

INLINE_METHOD int sprite_fill(Sprite *self, u32 color)
{
	return sprite_fill_region(self, self->dst.x, self->dst.y, 
				  self->dst.w, self->dst.h, color);
	
}

int sprite_fill_region(Sprite *self, s16 x, s16 y, u16 w, u16 h, u32 color)
{
	SDL_Rect dst;
	u32 rgbcolor;
	int retv;

	rgbcolor = SDL_MapRGB(self->screen->format,
			      color >> 16,
			      (color >> 8) & 0xff,
			      color & 0xff);

	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	
	retv = SDL_FillRect(self->surface, &dst, rgbcolor);
 	if (!retv)
 		retv = SDL_SetColorKey(self->surface, SDL_RLEACCEL, rgbcolor);

	if (retv)
		log_err("%s", SDL_GetError());

	return retv;
}

int sprite_fill_pixel(Sprite *self, s16 x, s16 y, u32 color)
{
	u32 pixel;
	u8 *p, bpp;

	if (x < 0 || x >= self->surface->w || y < 0 || y >= self->surface->h)
		return -EINVAL;

	pixel = SDL_MapRGB(self->screen->format,
			   color >> 16,
			   (color >> 8) & 0xff,
			   color & 0xff);

	bpp = self->surface->format->BytesPerPixel;

 	SDL_LockSurface(self->surface);

	p = (u8 *)self->surface->pixels + y * self->surface->pitch + x * bpp;

	switch (bpp) {
	case 4:
		*(u32 *)p = pixel;
		break;

	case 2:
		*(u16 *)p = pixel;
		break;

	case 3:
		log_fixme("unsupported odd surface format "
			  "(3 bytes per pixel): exit");
		abort();
		break;

	case 1:
		*p = pixel;
		break;
	}

 	SDL_UnlockSurface(self->surface);

	return 0;
}

INLINE_METHOD int sprite_set_alpha(Sprite *self, u8 alpha)
{
	return SDL_SetAlpha(self->surface, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
}

INLINE_METHOD int sprite_set_colorkey(Sprite *self, u32 color)
{
	u32 pixel;
	int retv;

	pixel = SDL_MapRGB(self->screen->format, 
			   color >> 16,
			   (color >> 8) & 0xff,
			   color & 0xff);
	
	retv = SDL_SetColorKey(self->surface, 
			       SDL_SRCCOLORKEY | SDL_RLEACCEL, pixel);

	return retv;
}

INLINE_METHOD int sprite_set_accel(Sprite *self, u32 color)
{
	u32 pixel;
	int retv;

	pixel = SDL_MapRGB(self->screen->format, 
			   color >> 16,
			   (color >> 8) & 0xff,
			   color & 0xff);
	retv = SDL_SetColorKey(self->surface, SDL_RLEACCEL, pixel);

	return retv;
}
