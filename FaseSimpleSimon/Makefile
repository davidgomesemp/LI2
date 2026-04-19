CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2

OBJ = main.o ssimon.o displaySS.o stack.o card.o

simon: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o simon

main.o: main.c ssimon.h displaySS.h
	$(CC) $(CFLAGS) -c main.c

ssimon.o: ssimon.c ssimon.h stack.h card.h
	$(CC) $(CFLAGS) -c ssimon.c

displaySS.o: displaySS.c displaySS.h ssimon.h stack.h card.h
	$(CC) $(CFLAGS) -c displaySS.c

stack.o: stack.c stack.h card.h
	$(CC) $(CFLAGS) -c stack.c

card.o: card.c card.h
	$(CC) $(CFLAGS) -c card.c

clean:
	rm -f *.o simon
