PROGS=	opendircolors dirconvert
SRCS.opendircolors=	opendircolors.c common.c
SRCS.dirconvert=	dirconvert.c common.c

BINDIR?=/usr/local/bin
MANDIR?=/usr/local/man/man
SYMLINKS=	opendircolors ${BINDIR}/dircolors
SYMLINKS+=	opendircolors.1.gz ${MANDIR}1/dircolors.1.gz

MAKEOBJDIR=obj

format:
	find ${.CURDIR} -type f -name '*.c' -exec clang-format -i {} \;

.include <bsd.progs.mk>
