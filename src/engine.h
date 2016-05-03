/* $Id: engine.h 20 2009-08-25 12:31:12Z gallows $
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

#ifndef ENGINE_H
#define ENGINE_H

enum EngineOptions {
	ENGINE_OPTION_FS=	1 << 0,
};

/*
 * Initialize sort_demo's engine.
 *
 * `opts' should be or-ed EngineOptions or 0.
 *  If `datadir' is a NULL pointer, default directory will be used (whose was
 *  definited at compilation time).
 *  engine_init() will return 0 if the engine was initialized correctly;
 *  otherwise, on error, the functino will return -1.
 */
int  engine_init  (int opts, const char *datadir);

/*
 * Quit sort_demo's engine.
 */
void engine_quit  (void);

/*
 * sort_demo main loop.
 */
int  engine_loop  (void);

 /**************/
 /* utilities: */
 /**************/

/*
 * Return the engine datadir.
 */
const char *engine_get_datadir(void);

/*
 * Absolutely non-portable utility to join path `a' with `b'.
 * If there is no error, the function returns a pointer to the joined (and
 * resolved) path. Otherwise it returns a NULL pointer and errno is set to
 * indicate the error.
 *
 * If `buf' is a NULL pointer, join_path() uses malloc(3) to allocate a buffer
 * up to PATH_MAX bytes to hold the resolved path-name. The caller should 
 * deallocate the return pointer using free(3).
 */
char *join_path(const char *a, const char *b, char *buf);

#endif /* !ENGINE_H */
