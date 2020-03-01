# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/07 00:56:57 by aait-ihi          #+#    #+#              #
#    Updated: 2020/02/11 12:46:10 by amoutik          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = readline.a

CFLAGS = -g -Wall -Werror -Wextra

CC = gcc

SRCS = readline.c cursor_move.c cursor_helper.c helper.c helper2.c signal.c\
		line_operation.c cmd_history.c select_mode.c ctrl_r.c\
		auto_compilation/compilation_aff.c\
		auto_compilation/context_bin.c\
		auto_compilation/context_path.c\
		auto_compilation/context_var.c\
		auto_compilation/trait_line.c

LIBFT = ../libft/libft.a

OBJ = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	@ar rc  $(NAME) $(OBJ)
	@ranlib $(NAME)

$(OBJ) : ft_readline.h

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all
