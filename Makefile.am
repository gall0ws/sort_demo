# $Id: Makefile.am 21 2009-08-28 23:44:56Z gallows $

SUBDIRS=		src data
EXTRA_DIST=		COPYING INSTALL
MANPAGE=		sort_demo.1

sort_demomandir=	${mandir}/man1
sort_demoman_DATA=	${MANPAGE}.gz

CONFIG_CLEAN_FILES = ${MANPAGE}.gz

${MANPAGE}.gz:	${MANPAGE}
	cat ${MANPAGE} | gzip ${GZIP_ENV} > ${MANPAGE}.gz
