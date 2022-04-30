/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlanani <rlanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 16:34:47 by rlanani           #+#    #+#             */
/*   Updated: 2022/04/23 22:26:37 by rlanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>


/* LEXIQUE =
    0 == other;
    1 == word;
    2 == numbers; 
*/


typedef struct s_token
{
  char          *str;
  int           type;
  struct s_token  *previous; 
  struct s_token  *next;
} t_token;

typedef struct s_db_list
{
  t_token *first;
  t_token *last;
  int lenght;
} t_db_list ;

typedef struct s_extension
{
  char              *ancient;
  char              *new;
  struct s_extension  *previous;
} t_extension;

typedef struct s_pipe_exec
{
  char  *infile;
  char  *outfile;
  int   cmds_nbr;
  int   args_nbr;
  int   save_input;
  int   save_output;
  int   input;
  int   output;
  int   pipe_fd[2];
  char  *cmd;
  char  *path;
  char  *bin;
  char  **newargs;
  char  ***env;
  int status;
} t_pipe_exec;

typedef struct s_minishell
{
  char	*line;
  char    *result;
  t_token    *token;
  t_db_list  *info;
  t_extension *extension;
  char  **env;
} t_minishell;

int parse_line (char *str, t_token **token, int i, t_db_list **info, char **env, int quote);
t_token *push_full_list (t_db_list *info, t_token *token, char *str, int type);
t_token *push_list(t_db_list *info, t_token *token, char *str, int type);
t_db_list *init_list(t_db_list *info);
t_token *push_empty_list (t_db_list *info, t_token *token, char *str, int type);
char *check_extension(char *str, char **env);
t_extension *set_extension(char *str, t_extension *extension);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlen(const char *s);