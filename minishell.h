/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlanani <rlanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 16:34:47 by rlanani           #+#    #+#             */
/*   Updated: 2022/04/23 18:34:59 by rlanani          ###   ########.fr       */
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

typedef struct dblist
{
  struct token *first;
  struct token *last;
  int lenght;
} db_list ;

typedef struct extension
{
  char              *ancient;
  char              *new;
  struct extension  *previous;
} value ;


int parse_line (char *str, list **token, value **extension, int i, db_list **info);
list *push_full_list (db_list *info, list *token, char *str, int type);
list *push_list(db_list *info, list *token, char *str, int type);
db_list *init_liste (db_list *info);
list *push_empty_list (db_list *info, list *token, char *str, int type);
char *check_extension(char *str, value *extension);
value *set_extension(char *str, value *extension);