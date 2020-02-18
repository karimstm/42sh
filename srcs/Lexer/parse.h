/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:48:13 by amoutik           #+#    #+#             */
/*   Updated: 2020/02/16 17:34:26 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# define ERRNO (*error_num())

t_list_simple_command	*parse_word_cmd(void);
int						grouping_kind(int kind);
t_node					*parse_commands();
t_node					*parse_and_or();
t_node					*parse_pipe_and();
t_redirection			*here_doc(t_token_kind kind);
t_redirection			*output_redir(void);
t_redirection			*input_redir(void);
t_redirection			*input_redirection(t_token_kind kind);
t_redirection			*output_aggregate(t_token_kind kind, int ignore_number);
t_redirection			*output_redirection(t_token_kind kind);
t_redirection			*here_string();
t_redirection			*input_aggregate();
const char				*redirect_name(t_token_kind kind);
t_redirection			*input_aggregate(t_token_kind kind);
t_redirection			*lenss_great(t_token_kind kind);
const char				*token_name(t_token_kind kind);
int						expect_token(t_token_kind kind);
void					unexpected_error(void);
int						is_n_number(char *word, int len);
void					aggregate_number(int *fd2,
					int *fd1, t_redirection **list, t_token_kind kind);
void					eol_continuation();
t_node					*init_parse_initial(void);
int						is_valid_token(t_token_kind kind);

#endif
