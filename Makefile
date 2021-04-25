PROG=	opendircolors
SRCS=	convert.c
MAN=	

BINDIR=/usr/local/bin
LINKS=	${BINDIR}/opendircolors ${BINDIR}/dircolors

MAKEOBJDIR=obj

.include <bsd.prog.mk>
