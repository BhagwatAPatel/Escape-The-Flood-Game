CC = gcc
CFLAGS = -Wall -ansi -Werror -pedantic 
CFILES = main.c genMap.c color.c terminal.c gameFunc.c newSleep.c linkedLists.c
HFILES = genMap.h color.h terminal.h gameFunc.h newSleep.h linkedLists.h
OFILES = $(CFILES:.c=.o)
EXEC = escape 
ARGUMENTS = map.txt

$(EXEC) : $(OFILES)
	$(CC) $(OFILES) -o $(EXEC)

$(OFILES) : $(CFILES) $(HFILES)
	$(CC) $(CFILES) $(CFLAGS) -c 

run : clean $(EXEC)
	./$(EXEC)$(ARGUMENTS)

valgrind : clean $(EXEC)
	valgrind ./$(EXEC) $(ARGUMENTS)

clean : 
	rm -f $(EXEC) $(OFILES)