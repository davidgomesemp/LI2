CFLAGS=-ggdb
OBJS=main.o golf.o display.o stack.o card.o

golf: $(OBJS)
	gcc -o golf $(OBJS) $(CFLAGS)

clean:
	rm $(OBJS) golf

# Testes CUnit
test: test_card test_stack test_golf
	./test_card
	./test_stack
	./test_golf

test_card: test_card.c card.c
	gcc $(CFLAGS) -o test_card test_card.c card.c -lcunit

test_stack: test_stack.c stack.c card.c
	gcc $(CFLAGS) -o test_stack test_stack.c stack.c card.c -lcunit

test_golf: test_golf.c golf.c card.c stack.c
	gcc $(CFLAGS) -o test_golf test_golf.c golf.c card.c stack.c -lcunit
