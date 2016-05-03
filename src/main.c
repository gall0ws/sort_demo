/* $Id: main.c 19 2009-08-23 18:41:20Z gallows $
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

#include <stdio.h>
#include <getopt.h>

#include "log.h"
#include "engine.h"

#define USAGE_FMT	\
	"Sort Demo (%s)\n\n"						\
	"Usage: %s [OPTION]...\n\n"					\
	"  -f, --fullscreen\t enable fullscreen mode\n"			\
	"  -d, --datadir=DIR\t load game data from DIR\n"		\
	"                   \t (default: %s)\n\n"			\
	"  --display=DISPLAY\t X display to use\n"			\
	"  --help\t\t display this help and exit\n\n"

enum {
	OPT_DISPLAY,
	OPT_HELP,
};

static struct option long_options[] = {
	{ "fullscreen", no_argument, NULL, 'f' },
	{ "datadir", required_argument, NULL, 'd' },
	{ "display", required_argument, NULL, OPT_DISPLAY },
	{ "help", no_argument, NULL, OPT_HELP },
	{ NULL },
};

int main(int ac, char *av[])
{
	int c;
	char *datadir;
	u8 opts;

	datadir = NULL;
	opts = 0;

	for (;;) {
		c = getopt_long(ac, av, "fd:", long_options, NULL);
		if (c == -1)
			break;

		switch (c) {
		case OPT_HELP:
			printf(USAGE_FMT, VERSION, *av, DATADIR);
			return 0;

		case OPT_DISPLAY:
 			setenv("DISPLAY", optarg, 1);
			break;

		case 'f':
			opts |= ENGINE_OPTION_FS;
			break;

		case 'd':
			datadir = optarg;
			break;

		case '?':
			printf("Try `%s --help' for more information\n", 
			       av[0]);
			return 1;
		}
	}

	if (engine_init(opts, datadir) != 0)
		return 1;

	engine_loop();
	engine_quit();

	return 0;
}
