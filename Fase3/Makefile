CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c jogo.c interface.c menu.c engine.c parser.c stack.c card.c
OBJS = $(SRCS:.c=.o)

TEST_SRCS = testes.c jogo.c engine.c parser.c stack.c card.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

all: jogoCartas

# Como compilar o jogo
jogoCartas: $(OBJS)
	$(CC) $(CFLAGS) -o jogoCartas $(OBJS)

# Como compilar os testes
testes: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o testador $(TEST_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o jogoCartas testador save_teste.txt
