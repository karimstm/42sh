NAME = 42sh

SRC = srcs
BIN = bin
INC = includes
LIB = libft
BUILT = builtin
LEXINC = srcs/Lexer/

SRC_MIN = $(patsubst %.c, $(SRC)/minishell/%.c, env_list.c fds.c builtin.c free.c ft_cd.c ft_echo.c ft_env_new.c ft_pwd.c main.c shell.c exec.c redir.c job.c jobs_list.c exec2.c JobTesting/execute.c JobTesting/job2.c JobTesting/ft_jobs.c JobTesting/ft_fg.c JobTesting/ft_bg.c)
SRC_QUO = $(patsubst %.c, $(SRC)/strings/%.c, free_string.c is_number.c string.c)
SRC_REA = $(patsubst %.c, $(SRC)/readline/%.c, copy.c cursor.c cursor2.c cursor3.c cursor4.c edit_line.c handlers.c history.c line.c paste.c read_line.c terms.c)
SRC_LEXER = $(patsubst %.c, $(SRC)/Lexer/%.c, stack.c ast.c lex.c parse.c redirection.c start.c tmpfile.c quote_stripping.c)
SRC_BLT = $(patsubst %.c, $(SRC)/builtins/%.c, ft_exit.c)

OBJ_MIN = $(patsubst %.c, %.o, $(SRC_MIN))
OBJ_QUO = $(patsubst %.c, %.o, $(SRC_QUO))
OBJ_REA = $(patsubst %.c, %.o, $(SRC_REA))
OBJ_LEXER = $(patsubst %.c, %.o, $(SRC_LEXER))
OBJ_BLT = $(patsubst %.c, %.o, $(SRC_BLT))

OBJECT = $(OBJ_MIN) $(OBJ_QUO) $(OBJ_REA) $(OBJ_RED) $(OBJ_LEXER) $(OBJ_BLT)
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

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECT)
	@echo "$(RED)Linking...$(NC)"
	@$(CC) $(FLAGS) $(REAL_OBJECT) -ltermcap -lreadline $(LIBFT) -o $(NAME)
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

re : fclean all

.PHONY: all clean fclean re help

help :
	@echo "$(GREEN)src_minishell: $(RED)$(notdir $(SRC_MIN))$(NC)"
	@echo "$(GREEN)src_quotes: $(RED)$(notdir $(SRC_QUO))$(NC)"
	@echo "$(GREEN)src_readline: $(RED)$(notdir $(SRC_REA))$(NC)"
	@echo "$(BLUE)obj: $(RED)$(notdir $(REAL_OBJECT)$(NC))"

valgrind :
	@valgrind --tool=memcheck --leak-check=full --track-origins=yes ./$(NAME)

val : $(NAME) valgrind

valre : re valgrind
