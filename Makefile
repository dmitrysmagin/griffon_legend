
#
# MinGW Makefile for Griffon Legend
#

NAME      = griffon
CC        = gcc
CFLAGS    = -g -O2 -Wall -Wextra -std=c99
INCS      = -I.
LDFLAGS   =
LIBS      = -lm -lSDL_gfx -lSDL -lSDL_image -lSDL_mixer

OBJS      = griffon.o config.o state.o

# Magic starts here

ifeq ($(OS),Windows_NT)
NAME = griffon.exe
endif

all: $(NAME)

$(NAME): $(OBJS)
	@mkdir -p build
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o build/$@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCS) $< -o $@

clean:
	rm -f $(OBJS) build/$(NAME)
