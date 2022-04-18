/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlanani <rlanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 16:34:47 by rlanani           #+#    #+#             */
/*   Updated: 2022/04/14 17:29:35 by rlanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>


/* LEXIQUE =
    0 == other;
    1 == word;
    2 == numbers; 
*/


typedef struct token
{
  char          *str;
  int           type;
  struct token  *previous; 
  struct token  *next;
} list ;

typedef struct extension
{
  char              *ancient;
  char              *new;
  struct extension  *previous;
} value ;