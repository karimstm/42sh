/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_match.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amoutik <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 13:53:19 by amoutik           #+#    #+#             */
/*   Updated: 2019/10/12 10:51:33 by amoutik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "globing.h"

static const char *rangematch(const char *pattern, int test)
{
  int negate;
  int ok;
  char c;
  int c2;

  negate = 0;
  if ((negate = (*pattern == NOT || *pattern == '^')))
    ++pattern;
  ok = 0;
  while ((c = *pattern++) != RBRACKET)
  {
    if (c == EOS)
      return ((const char *)NULL);
    if (*pattern == RANGE && (c2 = *(pattern + 1)) != EOS && c2 != RBRACKET)
    {
      pattern += 2;
      if (c2 == EOS)
        return ((const char *)NULL);
      if (c <= test && test <= c2)
        ok = 1;
    }
    else if (c == test)
      ok = 1;
  }
  return (ok == negate ? (const char *)NULL : pattern);
}

static int     question_match(const char **string)
{
  if (*(*string) == EOS)
    return (FNM_NOMATCH);
  ++(*string);
  return (0);
}

static int     star_match(const char **pattern, const char **string, char *token)
{
  *token = *(*pattern);
  while (*token == STAR)
    *token = *(++(*pattern));
  if (*token == EOS || *token == SEP)
    return (0);
  while (*(*string) != EOS)
  {
    if (!is_match(*pattern, *string))
      return (0);
    ++(*string);
  }
  return (FNM_NOMATCH);
}

static int     bracket_match(const char **pattern, const char **string)
{
  if (*(*string) == EOS)
    return (FNM_NOMATCH);
  if ((*pattern = rangematch(*pattern, *(*string))) == NULL)
    return (FNM_NOMATCH);
  return (0);
}

int     is_match(const char *pattern, const char *string)
{
  char token;

  while (1)
  {
    token = *pattern++;
    if (token == EOS || token == SEP)
      return (*string == EOS ? 0 : FNM_NOMATCH);
    if (token == QUESTION)
    {
      if (question_match(&string) == FNM_NOMATCH)
        return (FNM_NOMATCH);
    }
    else if (token == STAR)
      return(star_match(&pattern, &string, &token));
    else if (token == LBRACKET)
    {
      if (bracket_match(&pattern, &string))
        return (FNM_NOMATCH);
      ++string;
    }
    else if (token != *string++)
      return (FNM_NOMATCH);
  }
}
