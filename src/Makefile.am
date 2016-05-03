# $Id: Makefile.am 21 2009-08-28 23:44:56Z gallows $

bin_PROGRAMS=	sort_demo

OBJECTS=	object.c layer.c sprite.c dialog.c menu.c array.c
if HAVE_LIBSDL_TTF
  OBJECTS+= text.c
endif

MODULES=	video.c engine.c log.c

sort_demo_SOURCES=	${OBJECTS} ${MODULES} main.c 

AM_CFLAGS=	-Wall -Wno-switch -g -O2 ${sdl_CFLAGS}

DATADIR ?=	${pkgdatadir}

AM_CPPFLAGS=	-DDATADIR=\"${DATADIR}\"
LDADD=		${sdl_LIBS}
