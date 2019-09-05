CFLAGS = -g -Wall
CC = gcc
INCLUDE = include

ALL_OBJECTS = cd.o echo.o env.o external.o history.o list.o ls.o nightswatch.o parse.o pcwd.o pinfo.o prompt.o signals.o shell.o utils.o

ysh: $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -I$(INCLUDE) -o ysh $(ALL_OBJECTS)
	ctags -R .

cd.o: cd.c $(INCLUDE)/cd.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c cd.c

echo.o: echo.c $(INCLUDE)/echo.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c echo.c

env.o: env.c $(INCLUDE)/env.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c env.c

external.o: external.c $(INCLUDE)/external.h $(INCLUDE)/shell.h $(INCLUDE)/list.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c external.c

history.o: history.c $(INCLUDE)/history.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c history.c

ls.o: ls.c $(INCLUDE)/ls.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c ls.c

list.o: list.c $(INCLUDE)/list.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c list.c

nightswatch.o: nightswatch.c $(INCLUDE)/nightswatch.h $(INCLUDE)/utils.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c nightswatch.c

parse.o: parse.c $(INCLUDE)/parse.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c parse.c

pcwd.o: pcwd.c $(INCLUDE)/pcwd.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c pcwd.c

pinfo.o: pinfo.c $(INCLUDE)/pinfo.h $(INCLUDE)/utils.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c pinfo.c

prompt.o: prompt.c $(INCLUDE)/prompt.h $(INCLUDE)/utils.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c prompt.c

shell.o: shell.c $(INCLUDE)/shell.h $(INCLUDE)/utils.h $(INCLUDE)/list.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c shell.c

signals.o: signals.c $(INCLUDE)/signals.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c signals.c

utils.o: utils.c $(wildcard $(INCLUDE)/*.h)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c utils.c

clean:
	rm *.o ysh
