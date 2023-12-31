NAME = ft_ls

SRCS =	srcs/main.c \
		srcs/linked_list_function.c \
		srcs/utils.c \
		srcs/print_default_format.c \
		srcs/print_list_format.c \
		srcs/ls.c \
		srcs/sort.c \
		srcs/color.c \
		srcs/parsing.c \

OBJS = ${SRCS:.c=.o}

LIB = libft/libft.a
LIBACL = libacl/lib/linux/libacl.a

CC = clang

CFLAGS = -Wall -Wextra -Werror

RM = rm -f

all		:	${NAME}

.c.o	:	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}	:	${OBJS}
		make -C libft
		${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIB} ${LIBACL}

libft	:
		make -C libft

clean	:
		make -C libft clean
		${RM} ${OBJS}

fclean	:
		make -C libft fclean
		${RM} ${OBJS}

re		: fclean all

.PHONY	: all clean fclean re libft