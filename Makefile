NAME = 42sh

SRC = srcs
BIN = bin
INC = includes
LIB = libft
BUILT = builtin
LEXINC = srcs/Lexer/
REALINE = realine

SRC_MIN = $(patsubst %.c, $(SRC)/Minishell/%.c, pre_parse.c handlers.c ft_type.c env_list.c fds.c builtin.c free.c ft_cd.c ft_echo.c ft_env_new.c ft_pwd.c main.c shell.c exec.c redir.c exec2.c jobs/jobs_list.c Jobs/job.c Jobs/execute.c Jobs/job2.c Jobs/ft_jobs.c Jobs/ft_fg.c Jobs/ft_bg.c)
SRC_QUO = $(patsubst %.c, $(SRC)/Strings/%.c, free_string.c is_number.c string.c)
SRC_LEXER = $(patsubst %.c, $(SRC)/Lexer/%.c, input_redir.c ast_rest.c ast_redir.c ast_utils.c stack.c ast.c lex.c parse.c redirection.c start.c tmpfile.c quote_stripping.c common.c meta.c subs_parse.c)
SRC_BLT = $(patsubst %.c, $(SRC)/Builtins/%.c, ft_exit.c)
SRC_AL= $(patsubst %.c, $(SRC)/Alias/%.c, alias.c)
SRC_HSH= $(patsubst %.c, $(SRC)/HashTable/%.c, hash_handle.c hash.c prime.c)
SRC_HISTORY= $(patsubst %.c, $(SRC)/history/%.c, history.c ft_fc.c fc_l.c fc_s.c fc_edit.c)

OBJ_MIN = $(patsubst %.c, %.o, $(SRC_MIN))
OBJ_QUO = $(patsubst %.c, %.o, $(SRC_QUO))
OBJ_REA = $(patsubst %.c, %.o, $(SRC_REA))
OBJ_LEXER = $(patsubst %.c, %.o, $(SRC_LEXER))
OBJ_BLT = $(patsubst %.c, %.o, $(SRC_BLT))
OBJ_AL = $(patsubst %.c, %.o, $(SRC_AL))
OBJ_HSH = $(patsubst %.c, %.o, $(SRC_HSH))
OBJ_HISTORY = $(patsubst %.c, %.o, $(SRC_HISTORY))

OBJECT = $(OBJ_MIN) $(OBJ_QUO) $(OBJ_LEXER) $(OBJ_BLT) $(OBJ_AL) $(OBJ_HSH) $(OBJ_HISTORY)
REAL_OBJECT = $(patsubst %, $(BIN)/%, /$(notdir $(OBJECT)))

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
	@$(CC) $(FLAGS) $(REAL_OBJECT) -ltermcap readline/readline.a $(LIBFT) -o $(NAME)
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
