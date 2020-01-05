#include "lex.h"
#include "parse.h"
#include "libft.h"

void syntax_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("Syntax Error: ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
    exit(0);
}

int         is_ifs(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return (1);
    return (0);
}

uint8_t get_digit(uint8_t index)
{
    uint8_t char_to_digit[256] = {
        ['0'] = 0,
        ['1'] = 1,
        ['2'] = 2,
        ['3'] = 3,
        ['4'] = 4,
        ['5'] = 5,
        ['6'] = 6,
        ['7'] = 7,
        ['8'] = 8,
        ['9'] = 9,
        ['a'] = 10, ['A'] = 10,
        ['b'] = 11, ['B'] = 11,
        ['c'] = 12, ['C'] = 12,
        ['d'] = 13, ['D'] = 13,
        ['e'] = 14, ['E'] = 14,
        ['f'] = 15, ['F'] = 15,
    };
    return (char_to_digit[index]);
}



int         is_metacharacter(int ch)
{
    char metacharacter[256] = {
        ['|'] = '|',
        ['&'] = '&',
        [';'] = ';',
        ['('] = '(',
        [')'] = ')',
        ['<'] = '<',
        ['>'] = '>',
    };
    return metacharacter[ch];
}


/*
** escape spaces
*/

void    escape_space()
{
    next_token();
    while (is_token(TOKEN_SPACE))
        next_token();
}

/*
** This function will make sure let you know if you've some uncomplete command
*/
int     get_new_line()
{
    char *tmp;
    char *tmp2;
    size_t     len;
    char *new_line = readline("> ");

    len = g_line - g_token.line;
    tmp = ft_strjoin(g_token.line, "\n");
    ft_strdel(&g_token.line);
    tmp2 = new_line;
    new_line = ft_strjoin(tmp, new_line);
    ft_strdel(&tmp2);
    g_line = new_line + len;
    g_token.line = new_line;
    return(1);
}

/*
** escape characters and expansion shall be handled later on
*/
void        scan_dquotes()
{
    int flag = 1;
    g_line++;
    while (*g_line)
    {
        if (*g_line == '"')
        {
             flag += 1;
             g_line++;
             break;
        }
        if (*g_line == '\\')
        {
            g_line++;
            if (*g_line == '"' || *g_line == '$')
                g_line++;
        } else
            g_line++;
    }
    if (flag % 2 != 0)
    {
        get_new_line();
        scan_dquotes();
    }
}

void        scan_squotes()
{
    int flag = 1;
    g_line++;
    while (*g_line && !is_ifs(*g_line))
    {
        if (*g_line == '\'')
        {
             flag += 1;
             g_line++;
             break;
        }
        g_line++;
    }
    if (flag % 2 != 0)
    {
        get_new_line();
        scan_squotes();
    }
}

void    scan_meta_condition(char k1, t_token_kind k2)
{
    g_token.kind = *g_line++;
    if (*g_line == k1)
    {
        g_token.kind = k2;
        g_line++;
        if (is_metacharacter(*g_line))
            syntax_error("near unexpected token `%c'", *g_line);
    }
}

void        scan_redirection_input()
{
    g_token.kind = *g_line++;
    if (*g_line == '<')
    {
        g_token.kind = TOKEN_DLESS;
        g_line++;
        if (*g_line == '<' || *g_line == '-')
        {
            g_token.kind = *g_line == '<' ? TOKEN_HERESTRING : TOKEN_DLESSDASH;
            g_line++;
        }
    } else if (*g_line == '&' || *g_line == '>')
    {
        g_token.kind = *g_line == '&' ? TOKEN_LESSAND : TOKEN_LESSGREAT;
        g_line++;
    }
}

void      scan_redirection_output()
{
    g_token.kind = *g_line++;
    if (*g_line == '>')
    {
        g_token.kind = TOKEN_DGREAT;
        g_line++;
        if (*g_line == '&')
        {
            g_token.kind = TOKEN_DGREATAND;
            g_line++;
        }
    } else if (*g_line == '&' || *g_line == '|')
    {
        g_token.kind = *g_line == '&' ? TOKEN_GREATAND : TOKEN_CLOBBER;
        g_line++;
    }
}

void        scan_meta_semi()
{
    g_token.kind = *g_line++;
    if (*g_line == ';')
    {
        g_token.kind = TOKEN_DSEMI;
        g_line++;
        if (is_metacharacter(*g_line))
            syntax_error("near unexpected token `%c'", *g_line);
    }
}

void        scan_meta()
{
    if (*g_line == '&')
        scan_meta_condition('&', TOKEN_AND_IF);
    else if (*g_line == '|')
        scan_meta_condition('|', TOKEN_OR_IF);
    else if (*g_line == ';')
        scan_meta_semi(); // do not forget to add ;} as RBRACE
    else if (*g_line == '<')
        scan_redirection_input();
    else if (*g_line == '>')
        scan_redirection_output();
    else
        g_token.kind = *g_line++;
}

void        scan_curly()
{
    if (*g_line == '{')
        g_token.kind = TOKEN_LBRACE;
    else if (*g_line == '}')
        g_token.kind = TOKEN_RBRACE;
    g_line++;
}

void check_assignment ()
{
    if (ft_isalpha(*g_line) || *g_line == '_')
    {
        while (ft_isalnum(*g_line) || *g_line == '_')
            g_line++;
        if (*g_line == '=')
        {
            g_token.kind = TOKEN_ASSIGNMENT_WORD;
            g_line++;
        }
    }

}

void        scan_dollar()
{
    const char *start;
    int     stack[100];
    int     index;

    index = 0;
    start = g_line;
    g_line++;
    if (*g_line == '(')
    {
        while (*g_line)
        {
            if (*g_line == '(')
                stack[index++] = '(';
            else if (*g_line == ')')
                index--;
            if (index == 0)
                break;
            g_line++;
        }
        g_line++;
    }
    else if (*g_line == '{')
    {
        while (*g_line && *g_line != '}')
            g_line++;
        g_line++;
    }
    g_token.start = start;
}

void        scan_string()
{
    g_token.kind = TOKEN_WORD;
    check_assignment();
    while (*g_line && !is_metacharacter(*g_line) && !is_ifs(*g_line) && *g_line != '}' && *g_line != '{')
    {
        if (*g_line == '"')
            scan_dquotes();
        else if (*g_line == '\'')
            scan_squotes();
        else if (*g_line == '$')
            scan_dollar();
        else
            g_line++;
    }
    g_token.word = strndup(g_token.start, g_line - g_token.start);
}

void scan_int()
{
    g_token.int_val = -1;
    int64_t val = 0;
    int64_t base = 10;
    int64_t digit;
    while (ft_isdigit(*g_line))
    {
        digit = get_digit(*g_line);
        val = val*base + digit;
        g_line++;
    }
    g_token.int_val = val;
}

void         next_token()
{
    g_token.start = g_line;
    if (*g_line)
    {
        if (isspace(*g_line))
        {
            g_token.kind = TOKEN_SPACE;
            g_line++;
        }
        else if (ft_isdigit(*g_line))
        {
            while (ft_isdigit(*g_line))
                g_line++;
            if (*g_line == '>' || *g_line == '<')
            {
                g_line = g_token.start;
                scan_int();
                scan_meta();
            }
            else
                scan_string();
        }
        else if (is_metacharacter(*g_line))
        {
            g_token.int_val = -1;
            scan_meta();
        }
        else if (*g_line == '{' || *g_line == '}')
            scan_curly();
        else
        {
            g_token.start = g_line;
            scan_string();
        }
    } else
    {
        g_token.kind = TOKEN_EOF;
    }
    g_token.end = g_line;
}



int         is_token(t_token_kind kind)
{
    return (g_token.kind == kind);
}

int        expect_token(t_token_kind kind)
{
    if (is_token(kind))
    {
        g_token.kind = TOKEN_EOF;
        next_token();
        return (1);
    } else
    {
        // syntax error to be raised later on
        return (0);
    }
}

int is_token_eof()
{
    return g_token.kind == TOKEN_EOF;
}

int match_token(t_token_kind kind) {
    if (is_token(kind)) {
        next_token();
        return (1);
    } else {
        return (0);
    }
}

int match_token_test(t_token_kind kind)
{
    while (is_token(TOKEN_SPACE))
        next_token();
    if (is_token(kind))
    {
        next_token();
        return (1);
    } else {
        return (0);
    }
}

# define assert_token_word(x) assert(strcmp(x, g_token.word) == 0 && match_token(TOKEN_WORD))
// # define assert_token_dqoute_word(x) assert(strcmp(x, g_token.word) == 0 && match_token(TOKEN_WORD_DQUOTED))
// # define assert_token_sqoute_word(x) assert(strcmp(x, g_token.word) == 0 && match_token(TOKEN_WORD_SQOUTED))
# define assert_token(x) assert(match_token(x))
# define assert_token_eof() assert(g_token.kind == '\0')

void init_stream(const char *str) {
    g_line = str;
    g_token.line = (char *)str;
    next_token();
}


// Test the lexer
void    test_lex()
{
    init_stream("ls filename&echo name>file&&so||ls -la;;end;");
    assert_token_word("ls");
    assert_token_word("filename");
    assert_token('&');
    assert_token_word("echo");
    assert_token_word("name");
    assert_token('>');
    assert_token_word("file");
    assert_token(TOKEN_AND_IF);
    assert_token_word("so");
    assert_token(TOKEN_OR_IF);
    assert_token_word("ls");
    assert_token_word("-la");
    assert_token(TOKEN_DSEMI);
    assert_token_word("end");
    assert_token(';');
    assert_token_eof();

    init_stream("echo \"something\" to look 'forward' \"\" name=value name=\"value\"");
    assert_token_word("echo");
    assert_token_word("\"something\"");
    assert_token_word("to");
    assert_token_word("look");
    assert_token_word("'forward'");
    assert_token_word("\"\"");
    assert_token_word("name=value");
    assert_token_word("name=\"value\"");
    assert_token_eof();

    init_stream("echo LOLO> cat >&2 >> << <<- >>&() <<< \"()\"");
    assert_token_word("echo");
    assert_token_word("LOLO");
    assert_token('>');
    assert_token_word("cat");
    assert_token(TOKEN_GREATAND);
    assert_token_word("2");
    assert_token(TOKEN_DGREAT);
    assert_token(TOKEN_DLESS);
    assert_token(TOKEN_DLESSDASH);
    assert_token(TOKEN_DGREATAND);
    assert_token('(');
    assert_token(')');
    assert_token(TOKEN_HERESTRING);
    assert_token_word("\"()\"");
    assert_token_eof();
}


/*
int     main()
{
    // test_lex();
    test_parse();
    return (0);
}
*/
