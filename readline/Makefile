# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/07 00:56:57 by aait-ihi          #+#    #+#              #
#    Updated: 2020/01/25 00:18:26 by cjamal           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = readline.a

CFLAGS = -Wall -Werror -Wextra

CC = gcc

SRCS = readline.c cursor_move.c cursor_helper.c helper.c helper2.c signal.c\
		line_operation.c cmd_history.c select_mode.c

LIBFT = ../libft/libft.a

OBJ = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	@ar rc  $(NAME) $(OBJ)
	@ranlib $(NAME)

$(OBJ) : ft_readline.h

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
