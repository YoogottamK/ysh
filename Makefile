CFLAGS = -g -Wall -lreadline
CC = gcc
INCLUDE = include

ALL_OBJECTS = cd.o echo.o env.o external.o history.o jobs.o kjob.o list.o ls.o nightswatch.o overkill.o parse.o pcwd.o pinfo.o prompt.o signals.o shell.o utils.o

ysh: $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -I$(INCLUDE) -o ysh $(ALL_OBJECTS)
	ctags -R .

cd.o: cd.c $(INCLUDE)/cd.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c cd.c

echo.o: echo.c $(INCLUDE)/echo.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c echo.c

env.o: env.c $(INCLUDE)/env.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c env.c

external.o: external.c $(INCLUDE)/external.h $(INCLUDE)/shell.h $(INCLUDE)/list.h $(INCLUDE)/utils.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c external.c

history.o: history.c $(INCLUDE)/history.h $(INCLUDE)/shell.h $(INCLUDE)/utils.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c history.c

jobs.o: jobs.c $(INCLUDE)/jobs.h $(INCLUDE)/shell.h $(INCLUDE)/list.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c jobs.c

kjob.o: kjob.c $(INCLUDE)/kjob.h $(INCLUDE)/shell.h $(INCLUDE)/list.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c kjob.c

ls.o: ls.c $(INCLUDE)/ls.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c ls.c

list.o: list.c $(INCLUDE)/list.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c list.c

nightswatch.o: nightswatch.c $(INCLUDE)/nightswatch.h $(INCLUDE)/utils.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c nightswatch.c

overkill.o: overkill.c $(INCLUDE)/overkill.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c overkill.c

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

signals.o: signals.c $(INCLUDE)/signals.h $(INCLUDE)/utils.h $(INCLUDE)/prompt.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c signals.c

utils.o: utils.c $(wildcard $(INCLUDE)/*.h)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c utils.c

clean:
	rm *.o ysh
