CFLAGS = -Wall -O2 -I./ -static
CC=gcc
STRIP=strip

CCARM=/root/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-gcc
STRIPARM=/root/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-strip

default:fake_cod2 fake_cod2_arm

fake_cod2: md5.o pb_md5.o fake_cod2.o

	${CC} ${CFLAGS} md5.o pb_md5.o fake_cod2.o -o fake_cod2
	${STRIP} fake_cod2

fake_cod2_arm:

	${CCARM} ${CFLAGS} -march=armv7-a -c md5.c -o md5_arm.o
	${CCARM} ${CFLAGS} -march=armv7-a -c pb_md5.c -o pb_md5_arm.o
	${CCARM} ${CFLAGS} -march=armv7-a -c fake_cod2.c -o fake_cod2_arm.o
	${CCARM} ${CFLAGS} -march=armv7-a md5_arm.o pb_md5_arm.o fake_cod2_arm.o -o fake_cod2_arm
	${STRIPARM} fake_cod2_arm

clean:
	rm -f *.o

distclean:
	rm -f *.o fake_cod2 fake_cod2_arm
