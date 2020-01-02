/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 16:34:02 by amoutik           #+#    #+#             */
/*   Updated: 2019/05/11 18:57:43 by zoulhafi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "quote.h"

void	init_token_list(t_token_list *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->node_count = 0;
}

/*
** if ((token = (t_token *)malloc(sizeof(t_token))) == NULL)
**		return ;
** This should not happen ever, if it did then we are in trouble
*/

void	push_token(t_token_list *list, char *tok, enum e_token_type type)
{
	t_token *token;

	if ((token = (t_token *)malloc(sizeof(t_token))) == NULL)
		return ;
	token->token = tok;
	token->tok_type = type;
	token->next = NULL;
	if (list->head == NULL)
	{
		list->head = token;
		list->tail = token;
	}
	else
	{
		list->tail->next = token;
		list->tail = token;
	}
	list->node_count++;
}

/*
** Delete a node from a list of tokens
*/

void	delete_token(t_token_list *list, t_token *tofind)
{
	t_token *current;
	t_token	*prev;

	current = list->head;
	prev = NULL;
	while (current)
	{
		if (current == tofind)
		{
			if (current != list->head && current != list->tail)
				prev->next = current->next;
			else
			{
				if (current == list->head)
					list->head = current->next;
				if (current == list->tail)
					list->tail = prev;
			}
			free_token(current);
			list->node_count--;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

char	free_token_list(t_token_list *list)
{
	t_token *current;
	t_token *tmp;

	current = list->head;
	while (current)
	{
		ft_strdel(&current->token);
		tmp = current->next;
		free(current);
		current = tmp;
	}
	free(list);
	return (1);
}

/*
	insert a token in a specific place
	t_token *new: if allocation failed we are in trouble
		returning null is must though
*/

t_token	*insert_token_indexed(t_token *current, char *str, enum e_token_type type)
{
	t_token *next;
	t_token *new;

	next = NULL;
	if ((new = (t_token *)malloc(sizeof(t_token))) == NULL)
		return (NULL);
	if (current && current->next)
		next = current->next;
	new->token = str;
	new->tok_type = type;
	new->next = next;
	current->next = new;
	return new;
}


/*
	print list of token
	this function to be deleted later on
*/

void print_tokens(t_token_list *list)
{
	t_token *current;

	current = list->head;
	while (current)
	{
		if (current->tok_type == SH_GLOBE)
			ft_printf("===== %s is SH_GLOBE\n", current->token);
		else if (current->tok_type & SH_EXPORT)
			ft_printf("%s - SH_EXPORT \n", current->token);
		ft_printf("=== %s\n", current->token);
		current = current->next;
	}
}