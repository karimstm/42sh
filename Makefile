# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/17 16:41:20 by amoutik           #+#    #+#              #
#    Updated: 2020/02/25 11:55:41 by amoutik          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 42sh

SRC = srcs
BIN = bin
INC = includes
LIB = libft
BUILT = builtin
LEXINC = srcs/Lexer/
REALINE = realine

SRC_MIN = $(patsubst %.c, $(SRC)/Minishell/%.c, expand_redirection.c setup_expan.c system.c input_red.c output_red.c handlers.c ft_type.c env_list.c fds.c builtin.c free.c ft_cd.c ft_echo.c \
												ft_env_new.c ft_pwd.c main.c shell.c exec.c redir.c)
SRC_JOBS = $(patsubst %.c, $(SRC)/Minishell/Jobs/%.c, ft_getopt.c ft_strsignal.c job_misc.c node_to_cmd.c job_status.c activejob.c delete_job.c processes.c\
														terminal.c jobs_list.c job.c execute.c job_helpers.c ft_jobs.c ft_fg.c ft_bg.c \
														sep_command.c init_exec.c assignment.c command_utils.c exec2.c)

SRC_EVENT = $(patsubst %.c, $(SRC)/Minishell/events/%.c, event.c pre_parse.c)
SRC_QUO = $(patsubst %.c, $(SRC)/Strings/%.c, free_string.c is_number.c string.c)
SRC_LEXER = $(patsubst %.c, $(SRC)/Lexer/%.c, expans.utils.c parse_error.c parse_utils.c parse_alias_redir.c parse_helper.c lex_helpers.c ft_xmalloc.c herdoc.c input_redir.c ast_rest.c ast_redir.c ast_utils.c stack_helper.c stack.c ast.c lex.c parse.c redirection.c start.c tmpfile.c quote_stripping.c common.c meta.c subs_parse.c)
SRC_BLT = $(patsubst %.c, $(SRC)/Builtins/%.c, ft_exit.c test_utils.c ft_test.c test_expr.c test_file.c)
SRC_AL= $(patsubst %.c, $(SRC)/Alias/%.c, alias.c)
SRC_HSH= $(patsubst %.c, $(SRC)/HashTable/%.c, hash_handle.c hash.c prime.c)
SRC_HISTORY= $(patsubst %.c, $(SRC)/history/%.c, history.c history_helper.c ft_fc.c fc_l.c fc_s.c fc_edit.c)
SRC_EXPNASION = $(shell find $(SRC)/expansion -name '*.c')
SRC_SUB_PROCESS = $(patsubst %.c, $(SRC)/Process_Sub/%.c, sub_core.c fifo.c process_sub.c tpname.c)

# SRC_EXPNASION_CORE = $(addprefix $(SRC)/expansion/Core/, expand_parametre.c expand_sub_art.c expand.c)
# SRC_EXPNASION_PARAMETRE = $(addprefix $(SRC)/expansion/Parametre/, expand_operator.c expand_simple.c expand_remove.c)

OBJ_MIN = $(patsubst %.c, %.o, $(SRC_MIN))
OBJ_JOB = $(patsubst %.c, %.o, $(SRC_JOBS))
OBJ_EVENT = $(patsubst %.c, %.o, $(SRC_EVENT))
OBJ_QUO = $(patsubst %.c, %.o, $(SRC_QUO))
OBJ_REA = $(patsubst %.c, %.o, $(SRC_REA))
OBJ_LEXER = $(patsubst %.c, %.o, $(SRC_LEXER))
OBJ_BLT = $(patsubst %.c, %.o, $(SRC_BLT))
OBJ_AL = $(patsubst %.c, %.o, $(SRC_AL))
OBJ_HSH = $(patsubst %.c, %.o, $(SRC_HSH))
OBJ_HISTORY = $(patsubst %.c, %.o, $(SRC_HISTORY))
OBJ_EXPNASION = $(patsubst %.c, %.o, $(SRC_EXPNASION))
OBJ_SUB_PROCESS = $(patsubst %.c, %.o, $(SRC_SUB_PROCESS))

OBJECT = $(OBJ_MIN) $(OBJ_QUO) $(OBJ_LEXER) $(OBJ_EVENT) $(OBJ_BLT) $(OBJ_AL) $(OBJ_HSH) $(OBJ_JOB) $(OBJ_HISTORY) $(OBJ_EXPNASION) $(OBJ_SUB_PROCESS)
REAL_OBJECT = $(patsubst %, $(BIN)/%, /$(notdir $(OBJECT)))
REAL_SRC = $(SRC_MIN) $(SRC_JOBS) $(SRC_EVENT) $(SRC_QUO) $(SRC_LEXER) $(SRC_BLT) $(SRC_AL) $(SRC_HSH) $(SRC_HISTORY) $(SRC_EXPNASION) $(SRC_SUB_PROCESS)

CC = gcc
FLAGS = -g -Wall -Wextra -Werror
CPP_FLAGS = -I$(INC) -I$(LIB)/includes -I$(LEXINC)
LIBFT = $(LIB)/libft.a

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
NC = \033[1;0m

all: $(LIBFT) $(REALINE) $(NAME)

$(REALINE):
	@make -C readline

$(NAME): $(OBJECT)
	@echo "$(RED)Linking...$(NC)"
	@$(CC) $(FLAGS) $(REAL_OBJECT) -ltermcap readline/readline.a -lreadline $(LIBFT) -o $(NAME)
	@echo "$(GREEN)Finished...$(NC)"

$(LIBFT):
	@echo "$(BLUE)Getting Libraries...$(NC)"
	@make -C $(LIB)

%.o : %.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $(BIN)/$(notdir $@)

clean:
	@echo "$(RED)Cleaning up...$(NC)"
	@rm -rf $(REAL_OBJECT)
	@make -C $(LIB) clean

fclean: clean
	@rm -rf $(NAME)
	@make -C $(LIB) fclean
	@make -C readline fclean

re : fclean all

.PHONY: all clean fclean re help

help :
	@echo "$(GREEN)src_minishell: $(RED)$(notdir $(SRC_MIN))$(NC)"
	@echo "$(GREEN)src_quotes: $(RED)$(notdir $(SRC_QUO))$(NC)"
	@echo "$(GREEN)src_readline: $(RED)$(notdir $(SRC_REA))$(NC)"
	@echo "$(GREEN)src_history: $(RED)$(notdir $(SRC_HISTORY))$(NC)"
	@echo "$(BLUE)obj: $(RED)$(notdir $(REAL_OBJECT)$(NC))"

valgrind :
	@valgrind --tool=memcheck --leak-check=full --track-origins=yes ./$(NAME)

val : $(NAME) valgrind

valre : re valgrind
