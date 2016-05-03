/* $Id: video.h 20 2009-08-25 12:31:12Z gallows $
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

#ifndef VIDEO_H
#define VIDEO_H

#include "stdinc.h"

int  video_init (void);
void video_quit (void);

int  video_set_icon  (const char *bmp_path);
int  video_set_mode  (int width, int height, int bpp);
void video_set_title (const char *title);

void video_toggle_grab        (void);
void video_toggle_fullscreen  (void);
void video_toggle_cursor      (void);

int  video_flip  (void);

void video_get_driver_name (char *buf, size_t bufsz);
u16  video_get_width       (void);
u16  video_get_height      (void);
u32  video_get_flags       (void);
u8   video_get_bpp	   (void);
u32  video_get_pixel       (s16 x, s16 y);

#endif /* !VIDEO_H */
