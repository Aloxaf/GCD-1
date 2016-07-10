fun=GCDmain.o GCDtext.o  GCDmouse.o  GCDchoice.o GCDimage.o GCDsyba.o GCDFunc.o
#

%.o : %.c
	${CC} ${CFLAGS} -o $@ -c $^
GCD.exe : ${fun}
	${CC} -o GCD.exe $^ -lgdi32 -lmsimg32 -luser32 -lkernel32 -lgdiplus -lshlwapi
	#strip -s GCD.exe

clean :
	rm *.o *.exe