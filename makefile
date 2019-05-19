CC = gcc
CFLAGS = -Wall -pedantic -Werror -std=c99 -D_POSIX_C_SOURCE=200809L -lm
LDFLAGS =

.PHONY: clean

covertsigs.single: covertsigs.c
	${CC} -o covertsigs covertsigs.c charbinary.c ${CFLAGS} -D SINGLE

covertsigs.double: covertsigs.c
	${CC} -o covertsigs covertsigs.c charbinary.c ${CFLAGS}

clean:
	$(RM) covertsigs charbinary *.o
