NAME	= my_objdump

CC	= gcc

RM	= rm -f

SRCS	= ./src/my_objdump.c \
          ./src/print_sections.c    \
          ./src/archive.c   \
          ./src/elf64.c   \
          ./src/elf32.c   \
          ./src/fill_elf.c

OBJS	= $(SRCS:.c=.o)

CFLAGS = -I ./include
CFLAGS += -W -Wall -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
