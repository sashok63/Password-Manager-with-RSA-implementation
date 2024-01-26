
CFLAGS=-std=c17 -Wall -Wextra -Werror -lm

all:
	gcc old_pm.c -o old_pm $(CFLAGS)