#include "ast.h"
#include "parse.h"
#include "shell.h"

void *xmalloc(size_t num_bytes) {
	void *ptr = malloc(num_bytes);
	if (!ptr) {
		perror("xmalloc failed");
		exit(1);
	}
	return ptr;
}

t_node *command_node(t_token_node kind)
{
	t_node *node = (t_node *)xmalloc(sizeof(t_node));
	node->kind = kind;
	node->redir = NULL;
	return (node);
}

t_compound_cmd *compound_cmd(t_compound_kind kind)
{
	t_compound_cmd *com_cmd = (t_compound_cmd *)xmalloc(sizeof(t_compound_cmd));
	com_cmd->kind = kind;
	return (com_cmd);
}

t_command *command(t_cmd_kind kind)
{
	t_command *cmd = (t_command *)xmalloc(sizeof(t_command));
	cmd->kind = kind;
	return (cmd);
}

t_list_simple_command *malloc_list_simple_command()
{
	t_list_simple_command *list = (t_list_simple_command *)xmalloc(sizeof(t_list_simple_command));
	return (list);
}

void                    init_list_simple_command(t_list_simple_command *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->node_count = 0;
}

void    token_push(t_list_simple_command *list, char *token_str, t_token_kind kind)
{
	t_simple_command *token;

	token = (t_simple_command *)xmalloc(sizeof(t_simple_command));
	token->name = token_str;
	token->kind = kind;
	token->next = NULL;
	if (list->node_count == 0 || list->head == NULL)
		list->head = token;
	else
		list->tail->next = token;
	list->tail = token;
	list->node_count++;
}

t_list_simple_command *merge_list(t_list_simple_command *left, t_list_simple_command *right)
{
	if (right == NULL)
		return (left);
	if (left && right)
		left->node_count += right->node_count;
	if (left->head && right->head)
	{
		left->tail->next = right->head;
		left->tail = right->tail;
	}
	else if (right)
		return (right);
	return (left);
}

void    print_list_tokens(t_list_simple_command *list)
{
	t_simple_command *current;

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		ft_printf("%s ", current->name);
		current = current->next;
	}
}

void    free_list(t_list_simple_command *list)
{
	t_simple_command *current;
	t_simple_command *tmp;

	current = NULL;
	if (list && list->head)
		current = list->head;
	while (current)
	{
		ft_strdel(&current->name);
		tmp = current->next;
		free(current);
		current = tmp;
	}
	free(list);
	list = NULL;
}


/****   SEPERATORS SEMI AND & ****/

t_sep_op *sep_commands(t_token_kind kind, t_node *left, t_node *right)
{
	t_sep_op *sep_cmd = (t_sep_op *)xmalloc(sizeof(t_sep_op));
	sep_cmd->kind = kind;
	sep_cmd->left = left;
	sep_cmd->right = right;
	return sep_cmd;
}


t_and_or *and_or_commands(t_token_kind kind, t_node *left, t_node *right)
{
	t_and_or *and_or_cmd = (t_and_or *)xmalloc(sizeof(t_and_or));
	and_or_cmd->kind = kind;
	and_or_cmd->left = left;
	and_or_cmd->right = right;
	return and_or_cmd;
}

/* = = = = = = = = = = = = = = = = = = = = = = = = = */

t_redirection *redirection_init(t_redirection *list)
{
	list->fd1 = -1;
	list->fd2 = -1;
	list->kind = 0;
	list->next = NULL;
	return (list);
}

t_redirection   *new_redir(int fd1, int fd2, t_token_kind kind)
{
	t_redirection *list = (t_redirection *)xmalloc(sizeof(t_redirection));
	list->fd1 = fd1;
	list->fd2 = fd2;
	list->kind = kind;
	list->word = NULL;
	list->next = NULL;
	return (list);
}

t_redirection *reverse_redirection(t_redirection *list)
{
	t_redirection *prev;
	t_redirection *current;
	t_redirection *next;

	current = list;
	prev = NULL;
	next = NULL;
	while (current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	return (prev);
}

/* print redirection list */

void		print_redir(t_redirection *list)
{
	t_redirection *current;

	current = list;
	while (current)
	{
		printf("(%s) %d %d ", redirect_name(current->kind), current->fd1, current->fd2);
		current = current->next;
	}
}

void		free_redir(t_redirection **redir)
{
	t_redirection	*current;
	t_redirection	*tmp;

	current = *redir;
	while (current)
	{
		tmp = current->next;
		ft_strdel(&current->word);
		current->fd1 >= 0 ? close(current->fd1) : 0;
		current->fd2 >= 0 ? close(current->fd2) : 0;
		free(current);
		current = tmp;
	}
	*redir = NULL;
}

void		free_tree(t_node **node)
{
	t_node *current;

	current = *node != NULL ? *node : NULL;

	if (current)
	{
		if (current->kind == NODE_SEMI_AND)
		{
			if (current->sep_op_command->left)
				free_tree(&current->sep_op_command->left);
			if (current->sep_op_command->right)
				free_tree(&current->sep_op_command->right);
			free(current->sep_op_command);
		}
		if (current->kind == NODE_AND_OR || current->kind == NODE_PIPE)
		{
			if (current->and_or_command->left)
				free_tree(&current->and_or_command->left);
			if (current->and_or_command->right)
				free_tree(&current->and_or_command->right);
			free(current->and_or_command);
		}
		else if (current->kind == NODE_SIMPLE_COMMAND)
			free_list(current->simple_command);
		free(current);
		current = NULL;
	}
	*node = NULL;
}
