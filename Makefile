CFLAGS = -g -Wall
CC = gcc
INCLUDE = include

ALL_OBJECTS = bg.o cd.o cronjob.o echo.o env.o external.o fg.o history.o jobs.o kjob.o list.o ls.o nightswatch.o overkill.o parse.o pcwd.o pinfo.o piping.o prompt.o redirect.o shell.o signals.o utils.o

ysh: $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -I$(INCLUDE) -o ysh $(ALL_OBJECTS)
	ctags -R .

debug: CFLAGS += -DDEBUG
debug: ysh

bg.o: bg.c $(INCLUDE)/bg.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c bg.c

cd.o: cd.c $(INCLUDE)/cd.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c cd.c

cronjob.o: cronjob.c $(INCLUDE)/cronjob.h $(INCLUDE)/shell.h $(INCLUDE)/utils.h $(INCLUDE)/parse.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c cronjob.c

echo.o: echo.c $(INCLUDE)/echo.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c echo.c

env.o: env.c $(INCLUDE)/env.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c env.c

external.o: external.c $(INCLUDE)/external.h $(INCLUDE)/shell.h $(INCLUDE)/list.h $(INCLUDE)/utils.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c external.c

fg.o: fg.c $(INCLUDE)/fg.h $(INCLUDE)/shell.h $(INCLUDE)/utils.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c fg.c

history.o: history.c $(INCLUDE)/history.h $(INCLUDE)/shell.h $(INCLUDE)/utils.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c history.c

jobs.o: jobs.c $(INCLUDE)/jobs.h $(INCLUDE)/shell.h $(INCLUDE)/list.h $(INCLUDE)/utils.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c jobs.c

kjob.o: kjob.c $(INCLUDE)/kjob.h $(INCLUDE)/shell.h $(INCLUDE)/list.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c kjob.c

list.o: list.c $(INCLUDE)/list.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c list.c

ls.o: ls.c $(INCLUDE)/ls.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c ls.c

nightswatch.o: nightswatch.c $(INCLUDE)/nightswatch.h $(INCLUDE)/utils.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c nightswatch.c

overkill.o: overkill.c $(INCLUDE)/overkill.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c overkill.c

parse.o: parse.c $(INCLUDE)/parse.h $(INCLUDE)/shell.h $(INCLUDE)/history.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c parse.c

pcwd.o: pcwd.c $(INCLUDE)/pcwd.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c pcwd.c

pinfo.o: pinfo.c $(INCLUDE)/pinfo.h $(INCLUDE)/utils.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c pinfo.c

piping.o: piping.c $(INCLUDE)/piping.h $(INCLUDE)/utils.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c piping.c

prompt.o: prompt.c $(INCLUDE)/prompt.h $(INCLUDE)/utils.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c prompt.c

redirect.o: redirect.c $(INCLUDE)/redirect.h $(INCLUDE)/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c redirect.c

shell.o: shell.c $(INCLUDE)/shell.h $(INCLUDE)/utils.h $(INCLUDE)/list.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c shell.c

signals.o: signals.c $(INCLUDE)/signals.h $(INCLUDE)/utils.h $(INCLUDE)/prompt.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c signals.c

utils.o: utils.c $(wildcard $(INCLUDE)/*.h)
	$(CC) $(CFLAGS) -I$(INCLUDE) -c utils.c

clean:
	rm *.o ysh
