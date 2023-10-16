NAME = ft_ls

SRCS =	srcs/main.c \
		srcs/linked_list_function.c \
		srcs/utils.c \
		srcs/print_default.c \
		srcs/print_listing.c \
		srcs/ls.c \
		srcs/sort.c \

OBJS = ${SRCS:.c=.o}

LIB = libft/libft.a

CC = clang

CFLAGS = -Wall -Wextra -g

RM = rm -f

all		:	${NAME}

.c.o	:	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}	:	${OBJS}
		make -C libft
		${CC} ${CFLAGS} -o ${NAME} ${OBJS} ${LIB}

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