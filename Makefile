MF=	Makefile

#COMPILER
CC=	mpicc

CFLAGS=	-DC_MPI_PRACTICAL
LFLAGS=	$(CFLAGS) -lnuma

EXE=	mpinuma

SRC= \
	mpinuma.c \
	numamalloc.c \
	utilities.c

INC = \
	numamalloc.h \
	utilities.h

#
# No need to edit below this line
#

.SUFFIXES:
.SUFFIXES: .c .o

OBJ=	$(SRC:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(EXE)

$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

$(OBJ):	$(MF) $(INC)

clean:
	rm -f $(OBJ) $(EXE) core
