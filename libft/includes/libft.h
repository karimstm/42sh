/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-ihi <aait-ihi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 17:24:38 by zoulhafi          #+#    #+#             */
/*   Updated: 2020/02/21 12:05:23 by aait-ihi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <stdint.h>
# include <fcntl.h>
# define LEN 10
# define LEN_BUF 1000
# define BUFF_SIZE 1000
# define BETWEEN(v,v1,v2) ((v) >= (v1) && (v) <= (v2))
# define ABS(v) (v < 0 ? -(v) : (v))

enum
{
	PT_START,
	PT_QMARK,
	PT_STAR,
	PT_CHAR,
	PT_CHAR_SEQ,
	PT_END
};

typedef struct		s_fdlist
{
	int				fd;
	char			*tmp;
	struct s_fdlist	*next;
}					t_fdlist;

typedef struct		s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*previous;
	struct s_list	*next;
	struct s_list	*tail;
}					t_list;

typedef struct	s_matched_strings
{
	char *start;
	t_list *ends;
}				t_matched_strings;

typedef struct s_pattern
{
	int type;
	char *str;
	struct s_pattern *next;
} t_pattern;

/*
** First Part
*/
void				*ft_memset(void *b, int c, size_t len);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memccpy(void *dst, void *src, int c, size_t n);
void				*ft_memmove(void *dst, const void *src, size_t len);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
size_t				ft_strlen(const char *s);
size_t				ft_strlen_free(const char *str);
char				*ft_strdup(const char *src);
char				*ft_strndup(const char *s1, size_t n);
char				*ft_strcpy(char *dst, const char *src);
char				*ft_strncpy(char *dst, const char *src, size_t len);
char				*ft_strcat(char *s1, const char *s2);
char				*ft_strncat(char *s1, char *s2, size_t nb);
size_t				ft_strlcat(char *dst, char *src, size_t size);
char				*ft_strchr(const char *s, int c);
char				*ft_strrchr(const char *s, int c);
char				*ft_strstr(const char *haystack, const char *needle);
char				*ft_strnstr(const char *haystack,
					const char *needle, size_t len);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_atoi(const char *str);
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_toupper(int c);
int					ft_tolower(int c);

/*
** Second Part
*/
void				*xrealloc(void *ptr, size_t size);
void				*ft_realloc(void *buf, size_t size, size_t old_size);
void				*ft_memalloc(size_t size);
void				ft_memdel(void **ap);
char				*ft_strnew(size_t size);
void				ft_strdel(char **as);
void				ft_strclr(char *s);
void				ft_striter(char *s, void (*f)(char *));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strmap(char const *s, char (*f)(char));
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int					ft_strequ(char const *s1, char const *s2);
int					ft_strnequ(char const *s1, char const *s2, size_t n);
char				*ft_strsub(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strjoin_free(char *s1, char const *s2);
char				*ft_strjoin_pre_free(char *s1, char *del, char const *s2);
char				*ft_strtrim(char const *s);
char				**ft_strsplit(char const *s, char c);
char				*ft_itoa(int n);
int					ft_putchar(int c);
void				ft_putstr(char const *s);
void				ft_putendl(char const *s);
void				ft_putnbr(int n);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putnbr_fd(int n, int fd);

/*
** Bonus Part
*/
t_list				*ft_lstnew(void const *content, size_t content_size);
void				ft_lstdelone(t_list **alst, void (*del)(void*, size_t));
void				ft_lstdel(t_list **alst, void (*del)(void*, size_t));
void				ft_lstadd(t_list **alst, t_list *new);
void				ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list				*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));

/*
** String Functions
*/
void				ft_str_replace(char **str, char *old, char *new);
char				*ft_remove_char(char **str, char c);
char				*ft_strjoin_pre(char const *s1, char *delimiter,
					char const *s2);
int					ft_str_isalnum(char *str);
int					ft_str_isalpha(char *str);
int					ft_str_isnull(char *str);
char				*ft_trim(char *src);

/*
** Array of strings Functions
*/
char				**ft_strsplit_ws(char const *s);
void				ft_revtab(void **tab, int len);
void				ft_sorttab(void **tab, int len, int (*cmp)());
char				**ft_strtabdup(char **tab);
int					ft_strtablen(char **tab);
void				ft_free_strtab(char **tab);

/*
** Linked-List Functions
*/
t_list				*ft_lstsearch(t_list *lst, void *obj, int (*f)());
void				ft_lstrev(t_list **begin_list);
void				ft_lstadd_end(t_list **alst, t_list *new);
void				ft_lstcontent_iter(t_list *lst, void (*f)(void *elem));
void				ft_lstsort(t_list **begin_list, int (*cmp)());
size_t				ft_lstlen(t_list *lst);

/*
** Other Functions
*/
int					get_next_line(const int fd, char **line);
void				free_gnl(int fd);
int					ft_printf(const char *format, ...);
int					ft_printf_fd(int fd, const char *format, ...);
int					ft_vprintf(int fd, const char *format, va_list *ap);
int					ft_nbrlen(int n);
int					ft_iswhitespace(int c);
int					ft_isspace(int c);
void				ft_swap_pt(void **pt1, void **pt2);
int					ft_sqrt(int nb);

/*
** Functions Created To Create Printf
*/
void				free_printf(void);
int					ft_atoi_pr(const char *str);
void				ft_putnbr_base(uintmax_t nb, int flags[LEN]);
void				ft_alloc_mem(char **s, int nb, int flag, int i);
int					print_format(int flags[LEN], va_list ap);
void				ft_putnbr_pr(intmax_t nb, int flags[LEN]);
void				ft_putnbr_u(uintmax_t nb, int flags[LEN]);
void				convert_flags(int flags[LEN]);
int					ft_strlen_null(char *s, int cpt_null);
int					ft_strlen_w(wchar_t *str);
int					ft_strlen_wf(wchar_t *str, int flag6);
int					stock_char(char c, int print);
int					to_w(wchar_t c, char s[5]);
void				print_spaces(int nb);
void				print_zeroes(int nb);
char				jump(char **s, char c);
int					is_conv(char c, char *s_conv);
int					print_sub_0(intmax_t nb, int flags[LEN]);
int					get_max(int len1, int len2);
int					get_min(int len1, int len2);
char				*null_str(char str[7]);
wchar_t				*null_str_w(wchar_t str[7], int flags[LEN]);
char				next_conv(char *s);
int					get_width(char **s, va_list ap, int flags[LEN]);
void				get_flags(char **s, va_list ap, int flags[LEN]);
void				putnbr_u_flag(int flags[LEN], va_list ap);
void				putnbr_flag(int flags[LEN], va_list ap);
void				putnbrbase_flag(int flags[LEN], va_list ap);
int					putstr_flag(int flags[LEN], va_list ap);
void				ft_putchar_pr(char c);
void				ft_putstrr(char *s);
void				putstr_pr(char *str, int flags[LEN]);
int					putstr_w(wchar_t *str, int flags[LEN]);
void				init_s(char s[5]);
char				**get_static_str(void);
void				ft_debug(char *tty, const char *format, ...);

/*
** Functions Created To Create Get_Next_line
*/
t_fdlist			**get_static_list(void);
void				init_fdlist(t_fdlist **elem, int fd);
/*
** add by aait-ihi /(@_@)\
*/

long	ft_max(long a, long b);
long	ft_min(long a, long b);
_Bool ft_swap(int *a, int *b);

void ft_die(const char *message, int error_nb);

int ft_isnalnum(int c);
int ft_str_occurence(char *str, char c);
char *ft_strreplace(char *str, char find, char replace);
char *ft_strnjoin(char **strings, int str_count);
int ft_isinstr(char c, const char *s);

char *get_matched_bracket(char *str, const char *bracket, char *to_skip, int escaping);
t_list *reg_match(char *str, char *str_pattern);

char *ft_skip_chars(const char *str, const char *compare,
					int (*f)(int));
char *ft_rskip_chars(const char *str, const char *cmp,
					 int (*f)(int), int i);
char *ft_rskip_unitl_char(const char *str, const char *compare,
						  int (*f)(int), int i);
char *ft_skip_unitl_char(const char *str, const char *compare,
						 int (*f)(int));

void free_content_list(t_list **list, size_t size);
void ft_lstdel2(t_list **alst, void (*del)(void *));
t_list *ft_lstenqueue(t_list **list, t_list *new);
void ft_lstdequeue(t_list **list, void (*del)(void **));
long long ft_atoi_base(const char *s, const char *s_base, int base);
int ft_strsequ(char *str, char **cmps);
int ft_strindexof(const char *s, char c);
//-----------------------------------------------------------------------------

/*
** Added by amoutik (*__*)
*/

/*
**	ft_pow does not garante overflow
*/
long				ft_pow(long val1, long val2);

#endif
