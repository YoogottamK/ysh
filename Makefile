CFLAGS = -g -Wall
CC = gcc
INCLUDE = include

ALL_OBJECTS = cd.o echo.o external.o history.o ls.o nightswatch.o parse.o pcwd.o pinfo.o prompt.o shell.o utils.o

ysh: $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -I$(INCLUDE) -o ysh $(ALL_OBJECTS)
	ctags -R .

cd.o: cd.c include/cd.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c cd.c

echo.o: echo.c include/echo.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c echo.c

external.o: external.c include/external.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c external.c

history.o: history.c include/history.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c history.c

ls.o: ls.c include/ls.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c ls.c

nightswatch.o: nightswatch.c include/nightswatch.h include/utils.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c nightswatch.c

parse.o: parse.c include/parse.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c parse.c

pcwd.o: pcwd.c include/pcwd.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c pcwd.c

pinfo.o: pinfo.c include/pinfo.h include/utils.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c pinfo.c

prompt.o: prompt.c include/prompt.h include/utils.h include/shell.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c prompt.c

shell.o: shell.c include/shell.h include/utils.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c shell.c

utils.o: utils.c include/utils.h include/cd.h include/echo.h include/external.h include/history.h include/ls.h include/parse.h include/pinfo.h include/pcwd.h include/prompt.h
	$(CC) $(CFLAGS) -I$(INCLUDE) -c utils.c

clean:
	rm *.o ysh
