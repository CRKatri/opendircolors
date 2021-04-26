PROGS=	opendircolors dirconvert
SRCS.opendircolors=	opendircolors.c common.c
SRCS.dirconvert=	dirconvert.c common.c
#TODO: Write manpage
MAN=	

BINDIR=/usr/local/bin
SYMLINKS=opendircolors ${BINDIR}/dircolors
#TODO: Write manpage
#MLINKS= opendircolors.1 dircolors.1

MAKEOBJDIR=obj

.include <bsd.progs.mk>
