# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/17 16:41:20 by amoutik           #+#    #+#              #
#    Updated: 2020/03/03 01:30:18 by aait-ihi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 42sh

SRC = srcs
BIN = bin
INC = includes
LIB = libft
BUILT = builtin
LEXINC = srcs/Lexer/

PATH_MINISHELL = $(SRC)/Minishell/
PATH_JOBS = $(SRC)/Minishell/Jobs/
PATH_EVENT = $(SRC)/Minishell/events/
PATH_QUO = $(SRC)/Strings/
PATH_LEXER = $(SRC)/Lexer/
PATH_BLT = $(SRC)/Builtins/
PATH_AL = $(SRC)/alias/
PATH_HSH = $(SRC)/HashTable/
PATH_HISTORY = $(SRC)/history/
PATH_EXPNASION_CORE = $(SRC)/expansion/Core/
PATH_EXPNASION_PARAM = $(SRC)/expansion/Parametre/
PATH_EXPNASION_ARITHM = $(SRC)/expansion/Arithmetic/
PATH_EXPNASION_SUBS = $(SRC)/expansion/Substitution/
PATH_SUB_PROCESS = $(SRC)/Process_Sub/
PATH_ENV = $(SRC)/Env/

SRC_MINISHELL = expand_redirection.o setup_expan.o system.o input_red.o output_red.o\
				 handlers.o ft_type.o  fds.o builtin.o free.o  ft_echo.o ft_pwd.o main.o shell.o exec.o redir.o

SRC_JOBS =  ft_getopt.o ft_strsignal.o job_misc.o node_to_cmd.o job_status.o activejob.o\
			delete_job.o processes.o terminal.o jobs_list.o job.o execute.o job_helpers.o\
			ft_jobs.o ft_fg.o ft_bg.o sep_command.o init_exec.o assignment.o command_utils.o exec2.o

SRC_EVENT =  event.o pre_parse.o event_utils.o

SRC_QUO = free_string.o is_number.o string.o

SRC_LEXER = expans.utils.o parse_error.o parse_utils.o parse_alias_redir.o parse_helper.o\
			lex_helpers.o ft_xmalloc.o herdoc.o input_redir.o ast_rest.o ast_redir.o ast_utils.o\
			stack_helper.o stack.o ast.o lex.o parse.o redirection.o start.o tmpfile.o\
			quote_stripping.o common.o meta.o subs_parse.o subs_parce2.o back_slash.o

SRC_BLT = ft_exit.o test_utils.o ft_test.o test_expr.o test_file.o cd.o cd_helper.o

SRC_AL = alias.o alias_utils.o handle_alias.o builtins_alias.o

SRC_HSH = hash_handle.o hash.o prime.o hash_resize.o hash2.o

SRC_HISTORY = history.o history_helper.o ft_fc.o fc_l.o fc_s.o fc_edit.o

SRC_EXPNASION_CORE = expand.o expand_parametre.o expand_sub_art.o

SRC_EXPNASION_PARAM = expand_operator.o expand_remove.o expand_simple.o

SRC_EXPNASION_ARITHM = ar_parse_helper.o arithmetic_expansion_parse.o evaluation_core.o evaluation_helper.o

SRC_EXPNASION_SUBS = substitution.o

SRC_SUB_PROCESS = sub_core.o fifo.o process_sub.o tpname.o

SRC_ENV = handle_env2.o handle_env.o handle_dupenv.o ft_env_new.o builtins_env.o

# OBJ_MIN = $(patsubst %.c, %.o, $(SRC_MIN))
# OBJ_JOB = $(patsubst %.c, %.o, $(SRC_JOBS))
# OBJ_EVENT = $(patsubst %.c, %.o, $(SRC_EVENT))
# OBJ_QUO = $(patsubst %.c, %.o, $(SRC_QUO))
# OBJ_REA = $(patsubst %.c, %.o, $(SRC_REA))
# OBJ_LEXER = $(patsubst %.c, %.o, $(SRC_LEXER))
# OBJ_BLT = $(patsubst %.c, %.o, $(SRC_BLT))
# OBJ_AL = $(patsubst %.c, %.o, $(SRC_AL))
# OBJ_HSH = $(patsubst %.c, %.o, $(SRC_HSH))
# OBJ_HISTORY = $(patsubst %.c, %.o, $(SRC_HISTORY))
# OBJ_EXPNASION = $(patsubst %.c, %.o, $(SRC_EXPNASION))
# OBJ_SUB_PROCESS = $(patsubst %.c, %.o, $(SRC_SUB_PROCESS))
# OBJ_ENV = $(patsubst %.c, %.o, $(SRC_ENV))

OBJECT =	$(SRC_MINISHELL) $(SRC_JOBS) $(SRC_EVENT) $(SRC_QUO) $(SRC_LEXER) $(SRC_BLT) $(SRC_AL)
OBJECT +=	$(SRC_HSH) $(SRC_HISTORY) $(SRC_EXPNASION_CORE) $(SRC_EXPNASION_PARAM) 
OBJECT +=	$(SRC_EXPNASION_ARITHM) $(SRC_EXPNASION_SUBS) $(SRC_SUB_PROCESS) $(SRC_ENV)


REAL_OBJECT = $(patsubst %, $(BIN)/%, $(OBJECT))

CC = gcc
FLAGS = -g -Wall -Wextra -Werror
CPP_FLAGS = -I$(INC) -I$(LIB)/includes -I$(LEXINC)

LIBFT = $(LIB)/libft.a
READLINE = readline/readline.a

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
NC = \033[1;0m

all: $(LIBFT) $(READLINE) $(NAME)

$(NAME): $(addprefix bin/,$(OBJECT))
	@echo "$(RED)Linking...$(NC)"
	@$(CC) $(FLAGS) $(REAL_OBJECT) -ltermcap $(READLINE) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)Finished...$(NC)"

$(LIBFT):
	@echo "$(BLUE)Getting Libft Library...$(NC)"
	@make -C $(LIB)

$(READLINE):
	@echo "$(BLUE)Getting readline Library...$(NC)"
	@make -C readline

clean:
	@echo "$(RED)Cleaning up...$(NC)"
	@rm -rf bin
	@make -C $(LIB) clean
	@make -C readline clean

fclean: clean
	@rm -rf $(NAME)
	@make -C $(LIB) fclean
	@make -C readline fclean

re : fclean all

.PHONY: all clean fclean re

#Object rules

bin/%.o : $(PATH_MINISHELL)%.c 
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_JOBS)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_EVENT)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_QUO)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_LEXER)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_BLT)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_AL)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_HSH)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_HISTORY)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_EXPNASION_CORE)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_EXPNASION_PARAM)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_EXPNASION_ARITHM)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_EXPNASION_SUBS)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_SUB_PROCESS)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

bin/%.o : $(PATH_ENV)%.c
	@mkdir -p $(BIN)
	@$(CC) $(FLAGS) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

