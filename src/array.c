/* $Id: array.c 20 2009-08-25 12:31:12Z gallows $
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

#include <time.h>
#include <unistd.h>

#include <SDL_thread.h>

#include "video.h"
#include "sprite.h"
#include "array.h"

#define ARRAY_SIZE	128
#define UDELAY		500

#define GREEN		0x00ff00
#define BG_COLOR	0x0f0f0f

typedef int (* ThreadFunc)(void *);
typedef void (* SortFunc)(Array *);

struct _Array {
	ObjectVT parent;

	Sprite *bg;
	Sprite *dot;

	u16 v[ARRAY_SIZE];
	bool sorted;

	u8 algo;
	u8 kase;

	Callback callback;
	SDL_Thread *thd;
};

static void _array_sort     (Array *self);
static void bubble_sort     (Array *self);
static void selection_sort  (Array *self);
static void insertion_sort  (Array *self);
static void quick_sort      (Array *self);
static void heap_sort       (Array *self);

static void swap	(u16 *a, u16 *b);
static int  select_min	(u16 *a, int start, int end);
static int  partition	(u16 *v, int l, int r);
static void _quick_sort	(u16 *v, int l, int r);
static void heapify	(u16 *v, int i, int max);

INLINE_METHOD static void array_free(Array *self)
{
	if (self->thd)
		SDL_KillThread(self->thd);

	objects_free(self->bg, self->dot, NULL);
}

static int array_blit(Array *self)
{
	int i;
	u16 h;
	s16 x, y;
	
	h = video_get_height();

	object_blit(self->bg);
	for (i=0; i<ARRAY_SIZE; ++i) {
		x = i * 4;
		y = h - self->v[i];
		layer_set_xy(self->dot, x, y);
		object_blit(self->dot);
	}

	return 0;
}

Array *array_new(void)
{
	Array *self;

	srand(time(NULL));

	self = calloc(1, sizeof(Array));
	
	OBJECT(self)->vtable.dtor = (pfDtor)array_free;
	OBJECT(self)->vtable.blit = (pfBlit)array_blit;

	self->dot = sprite_new(2, 2);
	sprite_fill(self->dot, GREEN);

	self->bg = sprite_new(video_get_width(), video_get_height());
	sprite_fill(self->bg, BG_COLOR);
	sprite_set_accel(self->bg, BG_COLOR);

	return self;
}

INLINE void array_sort(Array *self, SortType algo, SortCase kase)
{
	if (self->thd) {
		SDL_KillThread(self->thd);
		self->thd = NULL;
	}

	self->algo = algo;
	self->kase = kase;
	self->thd = SDL_CreateThread((ThreadFunc)_array_sort, self);
}

INLINE_METHOD void array_stop_sorting(Array *self)
{
	if (self->thd) {
		SDL_KillThread(self->thd);
		self->thd = NULL;
	}
}

INLINE_METHOD void array_set_callback(Array *self, Callback f)
{
	self->callback = f;
}

static void _array_sort(Array *self)
{
	SortFunc f = NULL;
	int i, x;

	self->sorted = 0;

	switch (self->kase) {
	case CASE_RANDOM:
		for (i=0; i<ARRAY_SIZE; ++i)
			self->v[i] = rand() % 400;
		break;

	case CASE_REVERSED:
		for (i=0; i<ARRAY_SIZE; ++i)
			self->v[i] = (rand() % 5) + 
				+ (3 * ARRAY_SIZE - (i+1) * 3);
		break;

	case CASE_NEARLY_SORTED:
		for (i=0; i<ARRAY_SIZE; ++i)
			if (rand() % 3)
				self->v[i] = i*3;
			else
				self->v[i] =  rand() % 400;
		break;

	case CASE_FEW_UNIQUE:
		self->v[0] = rand() % 400;
		self->v[1] = rand() % 400;

		for (i=2; i<ARRAY_SIZE; ++i) {
			x = rand() % 400;
			if ((x % 2))
				self->v[i] = self->v[rand() % i];
			else
				self->v[i] = x;
		}
		break;

	default:
		log_fixme("unknown case number: #%d", self->kase);
	}
		
	switch (self->algo) {
	case BUBBLE_SORT:
		f = bubble_sort;
		break;

	case SELECTION_SORT:
		f = selection_sort;		
		break;

	case INSERTION_SORT:
		f = insertion_sort;
		break;

	case QUICK_SORT:
		f = quick_sort;
		break;

	case HEAP_SORT:
		f = heap_sort;
		break;

	default:
		log_fixme("wrong algo number: #%d", self->algo);
	}

	if (f) {
		f(self);
		if (self->callback)
			self->callback(self);
	}
}

INLINE static void swap(u16 *a, u16 *b)
{
	if (*a != *b) {
		*a ^= *b;
		*b ^= *a;
		*a ^= *b;
	}
}

static int select_min(u16 *a, int start, int end)
{
	int i, imin;

	imin = start;

	for (i=start+1; i<=end; ++i) {
		usleep(UDELAY);
		if (a[i] < a[imin])
			imin = i;
	}

	return imin;
}

static int partition(u16 *v, int l, int r)
{
	int p, i, j;

	p = (l+r)/2;
	swap(&v[p], &v[r]);
	j = l;

	for (i=l; i<r; ++i) {
		usleep(UDELAY);
		if (v[i] <= v[r]) {
			swap(&v[i], &v[j]);
			++j;
		}
	}

	swap(&v[j], &v[r]);
	
	return j;
}

static void _quick_sort(u16 *v, int l, int r)
{
	int p;

	if (l < r) {
		p = partition(v, l, r);
		_quick_sort(v, l, p-1);
		_quick_sort(v, p+1, r);
	}
}

static void heapify(u16 *v, int i, int max)
{
	int l, r, gr = i;

	usleep(UDELAY);

	l = i * 2 + 1;
	r = i * 2 + 2;

	if (l < max && v[l] > v[gr])
		gr = l;

	if (r < max && v[r] > v[gr])
		gr = r;

	if (gr != i) {
		swap(v+gr, v+i);
		heapify(v, gr, max);
	}
}

static void bubble_sort(Array *self)
{
	int i, j;
	bool swapped;

	swapped = 1;
	for (i=0; swapped && i<ARRAY_SIZE; ++i) {
		swapped = 0;
		for (j=0; j<ARRAY_SIZE-1; ++j) {
			usleep(UDELAY);
			if (self->v[j] > self->v[j+1]) {
				swap(&self->v[j], &self->v[j+1]);
				swapped = 1;
			}
		}
	}

	self->sorted = 1;
}

static void selection_sort(Array *self)
{
	int i, imin;

	for (i=0; i<ARRAY_SIZE; ++i) {
		imin = select_min(self->v, i, ARRAY_SIZE-1);
		if (imin != i)
			swap(&self->v[i], &self->v[imin]);
	}

	self->sorted = 1;
}

static void insertion_sort(Array *self)
{
	int i, j;
	u16 value;

	for (i=1; i<ARRAY_SIZE; ++i) {
		j = i - 1;
		value = self->v[i];
		
		while (j>=0 && self->v[j] > value) {
			usleep(UDELAY);
			self->v[j+1] = self->v[j];
			--j;
		} 

		self->v[j+1] = value;
	}

	self->sorted = 1;
}

INLINE static void quick_sort(Array *self)
{
	_quick_sort(self->v, 0, ARRAY_SIZE-1);
	self->sorted = 1;
}

static void heap_sort(Array *self)
{
	int i;

	for (i=ARRAY_SIZE/2-1; i>=0; --i)
		heapify(self->v, i, ARRAY_SIZE-1);
    
	for (i=ARRAY_SIZE-1; i>0; --i) {
		swap(self->v, self->v+i);
		heapify(self->v, 0, i);
	}

	self->sorted = 1;
}

