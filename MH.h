/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlanani <rlanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 16:34:47 by rlanani           #+#    #+#             */
/*   Updated: 2022/05/16 20:14:07 by rlanani          ###   ########.fr       */
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
#include <dirent.h>

/* LEXIQUE =
    0 == other;
    1 == word;
    2 == numbers; 
*/

typedef struct s_wild_handler
{
  char  *chunk;
  char  *str;
  int chunk_type;
  int file_match;
} t_wild_handler;

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
  char  *app_outfile;
  int   cmds_nbr;
  int   args_nbr;
  int   save_input;
  int   save_output;
  int   input;
  int   output;
  int   pipe_fd[2];
  int   here_pipe_fd[2];
  t_token     **here_doc_tab;
  int here_doc_tab_index;
  int   first_cmd;
  char  *cmd;
  char  *path;
  char  *bin;
  char  **args;
  char  ***env;
  int   here_doc;
  int   here_inf;
  struct s_minishell *gate;
} t_pipe_exec;

typedef struct s_minishell
{
  char	      *line;
  char        *result;
  t_token     *token;
  t_db_list   *info;
  t_extension *extension;
  int         quote;
  char        ***env;
  int         grammar;
  int         here_doc;
  t_token     **here_doc_tab;
} t_minishell;



int       parse_line (t_minishell *minishell, int i);
t_token   *push_full_list (t_db_list *info, t_token *token, char *str, int type);
t_token   *push_list(t_db_list *info, t_token *token, char *str, int type);
t_db_list *init_list(t_db_list *info);
t_token   *push_empty_list (t_db_list *info, t_token *token, char *str, int type);
char      *check_extension(char *str, t_minishell *minishell);
t_extension *set_extension(char *str, t_extension *extension);
char	    *ft_strjoin(char *s1, char *s2, int delete);
char	    *ft_substr(char *s, unsigned int start, size_t len);
size_t	  ft_strlen(const char *s);
t_token   **here_doc(t_token *token, t_token *start);
t_token   *put_here_doc(t_token *token);
void      print_here_doc(t_token **token);
char *ft_34(t_minishell *minishell, int *i);
char *ft_39(t_minishell *minishell, int *i);
int not_token(char c);
char *dollars_sign(t_minishell *minishell, int *i, int quote);
void sort_env(char **env);
char	*ft_strdup(const char *s);
char *already_in_env(char *str, char **env);