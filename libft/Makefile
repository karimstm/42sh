# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/22 17:24:15 by zoulhafi          #+#    #+#              #
#    Updated: 2020/02/21 14:30:19 by aait-ihi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

__SRC = srcs/

__INCLUDES = includes

_SRC = ft_pow.c ft_strlen.c ft_nbrlen.c get_next_line.c ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_iswhitespace.c ft_str_isalpha.c ft_str_isalnum.c ft_sqrt.c ft_itoa.c ft_lstadd.c ft_lstadd_end.c ft_lstdel.c ft_lstdelone.c ft_lstiter.c ft_lstcontent_iter.c ft_lstmap.c ft_lstsort.c ft_lstrev.c ft_lstnew.c ft_lstsearch.c ft_realloc.c ft_memalloc.c ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c ft_memmove.c ft_memset.c ft_putchar.c ft_putchar_fd.c ft_putendl.c ft_putendl_fd.c ft_putnbr.c ft_putnbr_fd.c ft_putstr.c ft_putstr_fd.c ft_swap_pt.c ft_sorttab.c ft_revtab.c ft_strcpy.c ft_strcat.c ft_strchr.c ft_strclr.c ft_strcmp.c ft_strdel.c ft_strdup.c ft_strequ.c ft_striter.c ft_striteri.c ft_strjoin.c ft_strjoin_free.c ft_strjoin_pre.c ft_strjoin_pre_free.c ft_strlcat.c ft_strmap.c ft_strmapi.c ft_strncat.c ft_strncmp.c ft_strncpy.c ft_strnequ.c ft_strnew.c ft_strnstr.c ft_strrchr.c ft_strsplit.c ft_strsplit_ws.c ft_strstr.c ft_strsub.c ft_strtrim.c ft_tolower.c ft_toupper.c ft_printf.c ft_putnbr_pr.c ft_putnbr_base.c ft_putstr_pr.c ft_putstr_w.c ft_putstr_w2.c printf_utility1.c printf_utility2.c printf_utility3.c printf_utility4.c printf_memory.c ft_putnbr_u.c printf_flags.c ft_debug.c ft_free_strtab.c ft_strtabdup.c ft_strtablen.c ft_lstlen.c get_next_line_free.c ft_remove_char.c ft_str_replace.c ft_strndup.c ft_trim.c ft_str_isnull.c ft_isspace.c x_realloc.c error.c glob.c ft_match.c ft_strcmp_q.c list.c merge.c pglob.c

_SRC += ft_isinstr.c ft_lstenqueue.c ft_lstdequeue.c ft_skip_chars.c ft_strreplace.c ft_swap.c 
_SRC += ft_atoi_base.c ft_strindexof.c  ft_skip_until_char.c ft_isnalnum.c ft_max.c ft_die.c 
_SRC += get_matched_bracket.c reg_match.c ft_str_occurence.c ft_strnjoin.c free_content_list.c

OBJ = $(_SRC:.c=.o)
CFLAGS = -g -Wall -Wextra -Werror -I $(__INCLUDES)

all: $(NAME)

$(NAME) : $(OBJ) $(__INCLUDES)/libft.h
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)

%.o : srcs/%.c
	@gcc -c $(CFLAGS) -o  $@ $^

%.o : additional_function/%.c
	@gcc -c $(CFLAGS) -o $@ $^

clean:
	@rm -Rf $(OBJ)

fclean: clean
	@rm -Rf $(NAME)

re: fclean all
