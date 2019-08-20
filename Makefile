CFLAGS = -g -Wall
CC = gcc
INCLUDE = 

ysh: shell.o prompt.o parse.o cd.o pwd.o echo.o ls.o
	$(CC) $(CFLAGS) -o ysh shell.o prompt.o parse.o cd.o pwd.o echo.o ls.o
	ctags -R .

shell.o: shell.c shell.h pwd.o cd.o parse.o prompt.o
	$(CC) $(CFLAGS) -c shell.c

parse.o: parse.c parse.h shell.h
	$(CC) $(CFLAGS) -c parse.c

prompt.o: prompt.c prompt.h shell.h
	$(CC) $(CFLAGS) -c prompt.c

cd.o: cd.c cd.h shell.h
	$(CC) $(CFLAGS) -c cd.c

pwd.o: pwd.c pwd.h shell.h
	$(CC) $(CFLAGS) -c pwd.c

echo.o: echo.c echo.h shell.h
	$(CC) $(CFLAGS) -c echo.c

ls.o: ls.c ls.h shell.h
	$(CC) $(CFLAGS) -c ls.c

clean:
	rm *.o ysh
