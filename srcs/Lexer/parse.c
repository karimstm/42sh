#include "ast.h"
#include "lex.h"
#include "parse.h"
#include "libft.h"


const char *token_name(t_token_kind kind)
{
    const char *token_kind_names[] = {
        [TOKEN_AND_IF] = "&&",
        [TOKEN_OR_IF] = "||",
        [TOKEN_LBRACE] = "{",
        [TOKEN_RBRACE] = "}",
        ['|'] = "|"
    };
    return token_kind_names[kind];
}

const char *redirect_name(t_token_kind kind)
{
    const char *redirection_name[] = {
        ['<'] = "<",
        ['>'] = ">",
        [TOKEN_DLESS] = "<<",
        [TOKEN_HERESTRING] = "<<<",
        [TOKEN_DLESSDASH] = "<<-",
        [TOKEN_LESSAND] = "<&",
        [TOKEN_LESSGREAT] = "<>",
        [TOKEN_DGREAT] = ">>",
        [TOKEN_DGREATAND] = ">>&",
        [TOKEN_GREATAND] = ">&",
        [TOKEN_CLOBBER] = ">|",
    };
    return redirection_name[kind];
}

int  grouping_kind(int kind)
{
    const int grouping_kind[] = {
        [TOKEN_LBRACE] = GROUP_BRACE_COMMAND,
        ['('] = GROUP_PARAN_COMMAND,
    };
    return (grouping_kind[kind]);
}

t_list_simple_command *parse_word_cmd()
{
    t_list_simple_command *list = NULL;

    if (g_token.kind == TOKEN_WORD || g_token.kind == TOKEN_ASSIGNMENT_WORD)
    {
        list = malloc_list_simple_command();
        init_list_simple_command(list);
        while (is_token(TOKEN_WORD) || is_token(TOKEN_ASSIGNMENT_WORD))
        {
            // I should handle the case where a token stand alone
            token_push(list, (char *)g_token.word, g_token.kind);
            escape_space();
        }
    }
    return (list);
}

t_redirection       *parse_redirection()
{
    t_redirection *head;
    t_redirection *tmp;

    head = NULL;
    while (redirect_name(g_token.kind))
    {
        if (g_token.kind == '>')
            tmp = output_redirection(g_token.kind);
        else if (g_token.kind == TOKEN_DGREAT)
            tmp = output_redirection(TOKEN_DGREAT);
        else if (g_token.kind == TOKEN_GREATAND)
            tmp = output_aggregate();
        else if (g_token.kind == '<')
            tmp = input_redirection(g_token.kind);
        else if (g_token.kind == TOKEN_DLESS || g_token.kind == TOKEN_DLESSDASH)
            tmp = here_doc(TOKEN_DLESS);
        else if (g_token.kind == TOKEN_HERESTRING)
            tmp = here_string();
        tmp->next = head;
        head = tmp;
        escape_space();
    }
    return (head);
}

t_list_simple_command *parse_list_cmd()
{
    t_list_simple_command *list = NULL;

    if (g_token.kind == TOKEN_WORD)
        list = parse_word_cmd();
    return list;
}

t_node *init_parse_initial()
{
    t_list_simple_command *list = parse_word_cmd();
    t_redirection *redir = parse_redirection();
    t_redirection *tmp = NULL;
    t_token_kind kind;
    t_node *node = NULL;

    while (g_token.kind == TOKEN_WORD || redirect_name(g_token.kind))
    {
        if (g_token.kind == TOKEN_WORD)
            list = merge_list(list, parse_word_cmd());
        else
        {
            tmp = parse_redirection();
            tmp->next = redir;
            redir = tmp;
        }
    }
    if (list && list->head)
    {
        node = command_node(NODE_SIMPLE_COMMAND);
        node->simple_command = list;
        node->redir = redir;
    }
    else if (redir == NULL && (g_token.kind == '(' || g_token.kind == TOKEN_LBRACE))
    {
        kind = g_token.kind;
        escape_space();
        node = parse_commands();
        node->goup_kind = grouping_kind(kind);
        if ((kind == '(' && is_token(')'))
            || (kind == TOKEN_LBRACE && is_token(TOKEN_RBRACE)))
        {
            escape_space();
            while (redirect_name(g_token.kind))
                redir = parse_redirection();
            node->redir = redir;
        }
        else
            printf("here we should handle line continuation %c\n", g_token.kind);

    }
    return (node);
}



t_node          *parse_sep_cmd(t_token_kind kind, t_node *left)
{
    t_sep_op *sep_cmd = NULL;
    t_node *node = NULL;
    escape_space();
    sep_cmd = sep_commands(kind, left, parse_commands());
    node = command_node(NODE_SEMI_AND);
    node->sep_op_command = sep_cmd;
    return (node);
}

t_node *parse_pipe(t_token_kind kind, t_node *left)
{
    t_node *second_node = left;
    t_node *right;
    t_and_or *pipe;
    while (g_token.kind == '|')
    {
        escape_space();
        right = init_parse_initial();
        if (!right)
            syntax_error("should fire up line continuation");
        pipe = and_or_commands(kind, second_node, right);
        second_node = command_node(NODE_PIPE);
        second_node->and_or_command = pipe;
    }
    return (second_node);
}

t_node *parse_and_or()
{
    t_node *node = parse_pipe_and();
    t_node *second_node = node;
    t_and_or *and_or_cmd;
    t_token_kind kind;
    while (g_token.kind == TOKEN_AND_IF || g_token.kind == TOKEN_OR_IF)
    {
        kind = g_token.kind;
        escape_space();
        and_or_cmd = and_or_commands(kind, second_node, parse_pipe_and());
        second_node = command_node(NODE_AND_OR);
        second_node->and_or_command = and_or_cmd;
    }
    return (second_node);
}

t_node *parse_pipe_and()
{
    t_node *node = init_parse_initial();
    if (g_token.kind == '|')
        return parse_pipe(g_token.kind, node);
    return (node);
}

t_node *parse_commands()
{
    t_node *node = parse_and_or();
    if (g_token.kind == ';' || g_token.kind == '&')
        return parse_sep_cmd(g_token.kind, node);
    return (node);
}


void    print_parse(t_node *node)
{
    if (node)
    {
        if (node->kind == NODE_SEMI_AND)
        {
            printf("( %c", node->sep_op_command->kind);
            if (node->sep_op_command->left)
                print_parse(node->sep_op_command->left);
            if (node->sep_op_command->right)
                print_parse(node->sep_op_command->right);
            printf(")");
            print_redir(reverse_redirection(node->redir));

        }
        else if (node->kind == NODE_AND_OR || node->kind == NODE_PIPE)
        {
            printf("( %s ", token_name(node->and_or_command->kind));
            if (node->and_or_command->left)
                print_parse(node->and_or_command->left);
            if (node->and_or_command->right)
                print_parse(node->and_or_command->right);
            printf(")\n");
            print_redir(reverse_redirection(node->redir));

        }
        else if (node->kind == NODE_SIMPLE_COMMAND)
        {
            printf(" ");
            print_list_tokens(node->simple_command);
            print_redir(reverse_redirection(node->redir));
        }
    } else {
        printf("Something went wrong while printing the ast");
    }
}

void    test_parse()
{

    const char *tests[] = {
        "1 >file something 2>&1",
		"ls -la | cat -e | grep sh",
        "echo 1 ; (2 && (ls -la; echo amine)) || (3 && (ls && (echo karim || echo ahmed)))",
        "(ls -la; echo karim) > file ",
        "ls -la | {cat -e; ls -la} >file",
        "(ls -la; echo karim) | cat -e",
        "{ls -la | ls -la } > file",
        "echo $name",
        "echo $(ls -la && echo $(ls -la ')' \")\"))karim ls -la",
        "echo ${name}",
        "echo $(ls -la)",
		"ls -la && echo karim | cat -e",
    };
    for (const char **it = tests; it != tests + sizeof(tests)/sizeof(*tests); it++) {
        init_stream(*it);
        t_node *node = parse_commands();
        print_parse(node);
        printf("\n\n===========================================\n\n");
    }
    // if (g_token.kind != TOKEN_EOF)
    //     syntax_error("Syntax error near %c\n", g_token.kind);
}

