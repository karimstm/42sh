/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <amoutik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 16:48:13 by amoutik           #+#    #+#             */
/*   Updated: 2020/01/25 16:46:40 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

void			test_parse();
t_node			*parse_commands();
t_node			*parse_and_or();
t_node			*parse_pipe_and();
t_redirection	*here_doc(t_token_kind kind);
t_redirection	*input_redirection(t_token_kind kind);
t_redirection	*output_aggregate();
t_redirection	*output_redirection(t_token_kind kind);
t_redirection	*here_string();
t_redirection	*input_aggregate();
const char		*redirect_name(t_token_kind kind);
t_redirection	*input_aggregate(t_token_kind kind);
t_redirection	*lenss_great(t_token_kind kind);
const char		*token_name(t_token_kind kind);
int				expect_token(t_token_kind kind);
void			unexpected_error(void);
#endif
