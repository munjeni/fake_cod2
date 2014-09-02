CFLAGS = -O2 -Wall -I./ -static
CC=gcc
STRIP=strip

default:fake_cod2

fake_cod2: md5.o pb_md5.o fake_cod2.o

	${CC} ${CFLAGS} md5.o pb_md5.o fake_cod2.o -o fake_cod2
	#${STRIP} fake_cod2

clean:
	rm -f *.o

distclean:
	rm -f *.o fake_cod2
