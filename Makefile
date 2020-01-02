NAME = 42sh

SRC = srcs
BIN = bin
INC = includes
LIB = libft
BUILT = builtin
LEXINC = srcs/Lexer/

SRC_MIN = $(patsubst %.c, $(SRC)/minishell/%.c, builtin.c env.c errors.c fork.c free.c ft_cd.c ft_echo.c ft_env.c ft_pwd.c main.c shell.c)
SRC_QUO = $(patsubst %.c, $(SRC)/quotes/%.c, error.c free_string.c is_number.c list.c quote.c quote2.c spliter.c string.c til_dol.c util.c)
SRC_REA = $(patsubst %.c, $(SRC)/readline/%.c, copy.c cursor.c cursor2.c cursor3.c cursor4.c edit_line.c handlers.c history.c line.c paste.c read_line.c terms.c)
SRC_RED = $(patsubst %.c, $(SRC)/redirection/%.c, left_redirect.c parsing.c right_redirect.c)
SRC_LEXER = $(patsubst %.c, $(SRC)/Lexer/%.c, ast.c lex.c parse.c redirection.c start.c tmpfile.c)

OBJ_MIN = $(patsubst %.c, %.o, $(SRC_MIN))
OBJ_QUO = $(patsubst %.c, %.o, $(SRC_QUO))
OBJ_REA = $(patsubst %.c, %.o, $(SRC_REA))
OBJ_RED = $(patsubst %.c, %.o, $(SRC_RED))
OBJ_LEXER = $(patsubst %.c, %.o, $(SRC_LEXER))

OBJECT = $(OBJ_MIN) $(OBJ_QUO) $(OBJ_REA) $(OBJ_RED) $(OBJ_LEXER)
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
	@$(CC) $(REAL_OBJECT) -ltermcap -lreadline $(LIBFT) -o $(NAME)
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
	@echo "$(GREEN)src_redirect: $(RED)$(notdir $(SRC_RED))$(NC)"
	@echo "$(BLUE)obj: $(RED)$(notdir $(REAL_OBJECT)$(NC))"

valgrind :
	@valgrind --tool=memcheck --leak-check=full --track-origins=yes ./$(NAME)

val : $(NAME) valgrind

valre : re valgrind