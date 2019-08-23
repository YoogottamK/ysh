CFLAGS = -g -Wall
CC = gcc

ALL_OBJECTS = cd.o echo.o history.o ls.o parse.o pinfo.o prompt.o pwd.o shell.o system.o utils.o nightswatch.o

ysh: $(ALL_OBJECTS)
	$(CC) -o ysh $(ALL_OBJECTS)
	ctags -R .

cd.o: cd.c cd.h shell.h
	$(CC) $(CFLAGS) -c cd.c

echo.o: echo.c echo.h shell.h
	$(CC) $(CFLAGS) -c echo.c

history.o: history.c history.h shell.h
	$(CC) $(CFLAGS) -c history.c

ls.o: ls.c ls.h shell.h
	$(CC) $(CFLAGS) -c ls.c

nightswatch.o: nightswatch.c nightswatch.h utils.h shell.h
	$(CC) $(CFLAGS) -c nightswatch.c

parse.o: parse.c parse.h shell.h
	$(CC) $(CFLAGS) -c parse.c

pinfo.o: pinfo.c pinfo.h utils.h shell.h
	$(CC) $(CFLAGS) -c pinfo.c

prompt.o: prompt.c prompt.h utils.h shell.h
	$(CC) $(CFLAGS) -c prompt.c

pwd.o: pwd.c pwd.h shell.h
	$(CC) $(CFLAGS) -c pwd.c

shell.o: shell.c shell.h utils.h
	$(CC) $(CFLAGS) -c shell.c

system.o: system.c system.h shell.h
	$(CC) $(CFLAGS) -c system.c

utils.o: utils.c utils.h cd.h echo.h history.h ls.h parse.h pinfo.h prompt.h pwd.h system.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm *.o ysh
