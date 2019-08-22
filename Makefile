CFLAGS = -g -Wall
CC = gcc
INCLUDE = 

ysh: shell.o prompt.o parse.o cd.o pwd.o echo.o ls.o clear.o system.o pinfo.o history.o
	$(CC) $(CFLAGS) -o ysh shell.o prompt.o parse.o cd.o pwd.o echo.o ls.o clear.o system.o pinfo.o history.o
	ctags -R .

shell.o: shell.c shell.h pwd.o cd.o parse.o prompt.o
	$(CC) $(CFLAGS) -c shell.c

pinfo.o: pinfo.c pinfo.h shell.h prompt.h
	$(CC) $(CFLAGS) -c pinfo.c

history.o: history.c history.h shell.h
	$(CC) $(CFLAGS) -c history.c

parse.o: parse.c parse.h shell.h
	$(CC) $(CFLAGS) -c parse.c

prompt.o: prompt.c prompt.h shell.h
	$(CC) $(CFLAGS) -c prompt.c

system.o: system.c system.h shell.h
	$(CC) $(CFLAGS) -c system.c

cd.o: cd.c cd.h shell.h
	$(CC) $(CFLAGS) -c cd.c

pwd.o: pwd.c pwd.h shell.h
	$(CC) $(CFLAGS) -c pwd.c

echo.o: echo.c echo.h shell.h
	$(CC) $(CFLAGS) -c echo.c

ls.o: ls.c ls.h shell.h
	$(CC) $(CFLAGS) -c ls.c

clear.o: clear.c clear.h
	$(CC) $(CFLAGS) -c clear.c

clean:
	rm *.o ysh
