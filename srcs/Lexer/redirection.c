# include "ast.h"
# include "lex.h"
# include "libft.h"


static char	ft_isnumber(char *str, int len)
{
    int i;

    i = 0;
    if (str == NULL || *str == '\0' || len == 0)
        return (0);
	while (*str && i < len)
	{
		if(!ft_isdigit(*str))
			return (0);
        i++;
		str++;
	}
	return (1);
}

t_redirection *output_redirection(t_token_kind kind)
{
    int fd1;
    int fd2;
    t_redirection *list;

    list = NULL;
    fd1 = g_token.int_val == -1 ? 1 : g_token.int_val;
    fd2 = -1;
    escape_space();
    if (g_token.kind == TOKEN_WORD)
    {
        list = new_redir(fd1, fd2, kind);
        list->word = g_token.word;
    }
    else
        syntax_error("42sh: syntax error near unexpected token"); // need to free ast
    return (list);
}

t_redirection *output_aggregate()
{
    int fd1;
    int fd2;
    int len;
    t_redirection *list;

    list = NULL;
    fd1 = g_token.int_val == -1 ? 1 : g_token.int_val;
    escape_space();
    if (g_token.kind == TOKEN_WORD)
    {
        ft_printf_fd(2, "kdkd %s\n", g_token.word);
        len = ft_strlen(g_token.word);
        if (g_token.word[len - 1] == '-')
            len -= 1;
        if (ft_isnumber(g_token.word, len))
        {
        ft_printf_fd(2, "kdkd %s\n", g_token.word);

            fd2 = ft_atoi(g_token.word);
            ft_strdel(&g_token.word);
            list = new_redir(fd1, fd2, TOKEN_GREATAND);
            list->word = ft_strdup("-");
        }
        else
        {
            if (fd1 < 0)
                syntax_error("42sh: %s: ambiguous redirect", g_token.word);
            else
            {
                fd2 = -1;
                list = new_redir(fd1, fd2, TOKEN_GREATAND);
                list->word = g_token.word;
            }
        }
    }
    else
        syntax_error("42sh: syntax error near unexpected token");
    return (list);
}

/*

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

*/

t_redirection *input_redirection(t_token_kind kind)
{
    int fd1;
    int fd2;
    t_redirection *list;

    list = NULL;
    fd1 = g_token.int_val == -1 ? 0 : g_token.int_val;
    fd2 = -1;
    escape_space();
    if (g_token.kind == TOKEN_WORD)
    {
        list = new_redir(fd1, fd2, kind);
        list->word = g_token.word;
    } else
        syntax_error("42sh: syntax error near unexpected token");
    return (list);
}

t_redirection *here_doc(t_token_kind kind)
{
    int fd1;
    int fd2;
    char *buf;
    char *pathname;
    t_redirection *list;

    list = NULL;
    fd1 = g_token.int_val == -1 ? 0 : g_token.int_val;
    fd2 = -1;
    escape_space();
    if (g_token.kind == TOKEN_WORD)
    {
        pathname = ft_tmpfile();
        fd2 = open_tmp(pathname);
        while (1)
        {
            buf = readline("> ");
            if (ft_strcmp(buf, g_token.word) == 0)
            {
                ft_strdel(&buf);
                break;
            }
            ft_printf_fd(fd2, "%s\n", buf);
            ft_strdel(&buf);
        }
        close(fd2);
        fd2 = -1;
        list = new_redir(fd1, fd2, kind);
        list->herdoc = pathname;
    }
    else
        syntax_error("42sh: syntax error near unexpected token");
    return (list);
}

t_redirection *here_string()
{
    int fd1;
    int fd2;
    t_redirection *list;

    list = NULL;
    fd1 = g_token.int_val == -1 ? 0 : g_token.int_val;
    escape_space();
    fd2 = 1;
    list = new_redir(fd1, fd2, TOKEN_HERESTRING);
    list->herdoc = ft_strndup(g_token.start, g_token.end - g_token.start);
    if (g_token.kind == TOKEN_WORD)
        ft_strdel(&g_token.word);
    return (list);
}


// t_redirection *input_aggregate()
// {
//     int fd1;
//     int fd2;
//     t_redirection *list;

//     list = NULL;
//     fd1 = g_token.int_val == -1 ? 0 : g_token.int_val;
//     fd2 = 0;
//     escape_space();
//     if (g_token.kind == TOKEN_WORD)
//     {
//         if (ft_strcmp(g_token.word, "-") == 0)

//     }
//     return (list);
// }
