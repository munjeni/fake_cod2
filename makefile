CFLAGS = -Wall -O2 -I./ -static
CC=gcc
STRIP=strip

CCARM=/root/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-gcc
STRIPARM=/root/arm-linux-androideabi-4.6/bin/arm-linux-androideabi-strip

CCWIN=i586-mingw32msvc-gcc
WINDRES=i586-mingw32msvc-windres

default:fake_cod2 fake_cod2_arm fake_cod2.exe

fake_cod2: md5.o pb_md5.o fake_cod2.o

	${CC} ${CFLAGS} md5.o pb_md5.o fake_cod2.o -o fake_cod2
	${STRIP} fake_cod2

fake_cod2_arm:

	${CCARM} ${CFLAGS} -march=armv7-a -c md5.c -o md5_arm.o
	${CCARM} ${CFLAGS} -march=armv7-a -c pb_md5.c -o pb_md5_arm.o
	${CCARM} ${CFLAGS} -march=armv7-a -c fake_cod2.c -o fake_cod2_arm.o
	${CCARM} ${CFLAGS} -march=armv7-a md5_arm.o pb_md5_arm.o fake_cod2_arm.o -o fake_cod2_arm
	${STRIPARM} fake_cod2_arm

fake_cod2.exe:

	${CCWIN} -c md5.c -o md5_win.o
	${CCWIN} -c pb_md5.c -o pb_md5_win.o
	${CCWIN} -c fake_cod2.c -o fake_cod2_win.o
	${CCWIN} -c strsep.c -o strsep_win.o
	${WINDRES} cod2.rc -O coff -o cod2.res
	${CCWIN} md5_win.o pb_md5_win.o strsep_win.o fake_cod2_win.o cod2.res -o fake_cod2.exe -lwsock32

clean:
	rm -f *.o *.res

distclean:
	rm -f *.o *.res fake_cod2 fake_cod2_arm fake_cod2.exe
