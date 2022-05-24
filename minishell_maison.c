#include "minishell.h"

// GERER les >> correctement (e.g creer fichier si existe pas)
// CHECKER les leaks de la partie exec des que ce sera possible
// GERER les wildcards
// GERER les parentheses
// GERER ./a.out

/* MAN MINISHELL 
    TYPE 1 = WORD
    TYPE 2 = ARGUMENTS
    TYPE 3 = DOUBLE QUOTE
    TYPE 4 = SIMPLE QUOTE
    TYPE 5 = PIPE
    
    TYPE 7 = INFILE
    TYPE 8 = OUTFILE
    TYPE 9 = &&
    TYPE 10 = ||
    TYPE 11 = (
    TYPE 12 = )
    TYPE 13 = reserved for extension
    TYPE 14 = <<
    TYPE 15 = >>
    TYPE 16 = *
    
*/

//////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// UTILS //////////////////////////////////////////////////


int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		if (s1[i] == '\0' || s2[i] == '\0')
			return (0);
		i++;
	}	
	return (0);
}

int	ft_strcmpe(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}

void	ft_putchar(char c)
{
	write (1, &c, 1);
}

void	ft_putstr(char *str)
{
	int i;

	i = 0;

	while (str[i])
	{
		ft_putchar(str[i]);
		i += 2;
	}
}

void	ft_putendl(char *s)
{
	ft_putstr(s);
	write(1, "\n", 1);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	str = malloc((ft_strlen(s1) + ft_strlen(s2)) * sizeof(char) + 1);
	if (!str)
		return (NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int		lenstr;
	char				*t;
	int					i;

	i = 0;
	if (!s)
		return (0);
	lenstr = strlen(s);
	if (start >= lenstr)
	{
		t = malloc(1);
		t[0] = '\0';
		return (t);
	}
	if (len > (strlen(s) - start))
		t = malloc(sizeof(char) * ((strlen(s) - start)) + 1);
	else
		t = malloc(sizeof(char) * (len) + 1);
	if (!(t))
		return (0);
	while (len-- && s[start])
		t[i++] = s[start++];
	t[i] = '\0';
	return (t);
}

void free_list(t_token *token)
{
    t_token *temp;

    while (token && token->previous)
    {
        temp = token;
        free(temp);
        token = token->previous;
        
    }
}

char	*ft_strdup(const char *s)
{
	char	*new_s;
	int		i;

	new_s = malloc(ft_strlen(s) * sizeof(char) + 1);
	if (!new_s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		new_s[i] = s[i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}



///////////////////////////////////////// UTILS //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// CREATION DE LIST //////////////////////////////////////////////

int ft_grammar(t_token **token)
{
    if (*token)
    {
        if ((*token)->type == 1)
        {
            if ((*token)->type == 1 && (!(*token)->previous))
                return (1);
            
            if ((*token)->type == 1 && (*token)->previous->type == 1)
                (*token)->type = 2;

            if ((*token)->type == 1 && (*token)->previous->type == 2)
                (*token)->type = 2;
        }
        
        if ((*token)->type == 5 && ((!(*token)->previous) || (!(*token)->next)))
        {
                printf("bash: syntax error near unexpected token `|'\n");
                return (0);
        }

        if ((*token)->type == 9 && ((!(*token)->previous) || (!(*token)->next)))
        {
                printf("bash: syntax error near unexpected token `&&'\n");
                return (0);
        }

        if ((*token)->type == 10 && ((!(*token)->previous) || (!(*token)->next)))
        {
                printf("bash: syntax error near unexpected token `&&'\n");
                return (0);
        }

        if ((*token)->type == 15 && ((!(*token)->previous) || (!(*token)->next)))
        {
                printf("bash2: syntax error near unexpected token `newline'\n");
                return (0);
        }

        if ((*token)->type == 14 && ((!(*token)->next)))
        {
                printf("bash1: syntax error near unexpected token `newline'\n");
                return (0);
        }
        
    }
    return (1);
}

t_token *push_empty_list (t_db_list *info, t_token *token, char *str, int type)
{
    t_token *new_env;
    new_env = malloc(sizeof(t_token));
    if (!new_env)
        perror ("MALLOC CREATE LIST");
    new_env->next = NULL;
    new_env->previous = NULL;
    new_env->str = str;
    new_env->type = type;
    info->first = new_env;
    info->last = new_env;
    info->lenght = 1;
    return (new_env);
}

t_token *push_full_list (t_db_list *info, t_token *token, char *str, int type)
{
    t_token *new_env;
    new_env = malloc(sizeof(t_token));
    if (!new_env)
        perror ("MALLOC CREATE LIST");
    new_env->str = str;
    new_env->type = type;
    new_env->next = NULL;
    info->last->next = new_env;
    new_env->previous = info->last;
    info->last = new_env;
    info->lenght = info->lenght + 1;

    return (new_env);
}

t_token *push_list(t_db_list *info, t_token *token, char *str, int type)
{
    if (info->lenght == 0)
        return (push_empty_list (info, token, str, type));

    if (info->lenght > 0)
        return (push_full_list(info, token, str, type));
    return (0);
}

t_db_list *init_list(t_db_list *info)
{
    info = malloc(sizeof(t_db_list));
    if (!info)
        perror ("MALLOC CREATE LIST");
    info->first = NULL;
    info->last = NULL;
    info->lenght = 0;

    return (info);
}

/////////////////////////////////// CREATION DE LIST //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// BUILD IN ////////////////////////////////////////////////

int check_build_in(t_pipe_exec *pipe_exec)
{
    if (!strcmp(pipe_exec->cmd, "exit"))
        return (1); 
    if (!strcmp(pipe_exec->cmd, "env"))
        return (1); 
    if (!strcmp(pipe_exec->cmd, "export"))
        return (1); 
    if (!strcmp(pipe_exec->cmd, "unset"))
        return (1); 
    if (!strcmp(pipe_exec->cmd, "pwd"))
        return (1); 
    if (!strcmp(pipe_exec->cmd, "echo"))
        return (1); 
    if (!strcmp(pipe_exec->cmd, "cd"))
        return (1); 
        
    return (0);
}

char *check_extension(char *str, char **env)
{
    int i;
    int k;
    char *result;

    i = 0;
    k = 0;


    result = malloc(sizeof(char*) * 1);
    if (!result)
        perror("MALLOC CHECK_EXTENSION");
    result[0] = '\0';

    while (env[i])
    {
        if(!strncmp((const char *)env[i], (const char *)str, ft_strlen(str)))
        {
            while (env[i][k] != '=')
                k++;
            result = ft_substr(env[i], k + 1, ft_strlen(env[i]));
            return (result);
        }
        i++;
    }
    return (result);
}

int already_in_env(char *str, char **env, int k)
{
    int i;
    int j;
    int result;

    i = 0;
    j = 0;
    result = 0;

    printf("1\n");

    while (env[i])
    {
       /* if (!strcmpe(env[i], str))
        {
            result++;
        }*/
        j = 0;
        i++;
    }
    return (result);
}


char **export_env(t_token *token, char **env)
{
    int i;
    int k;
    int l;
    int j;
    int is_already;
    char **new_env;

    k = 0;
    i = 0;
    l = 0;
    j = 0;

    while (token->str && token->str[k] != '=')
        k++;

    is_already = already_in_env(token->str, env, k);

    while (env[i])
        i++;

    new_env = malloc(sizeof(char **) * i - is_already + 2);
    if (!new_env)
        perror("MALLOC EXPORT");

    i = 0;
    
    while (env[i])
    {
       
        if (!strncmp((const char *)env[i], (const char *)token->str, k))
        {
           
            while (env[i][j] && env[i][j] != '=')
                j++;
            if (j == k)
                new_env[i] = strdup(token->str);
            if (j != k)
            {
                new_env[i] = strdup(env[i]);
            }
            i++;
            j = 0;
        }
        if (strncmp((const char *)env[i], (const char *)token->str, k))
        {
            new_env[i] = strdup(env[i]);
            i++;
        }
    }


    new_env[i] = '\0';
    if (is_already == 0 )
    {
        new_env[i] = strdup(token->str);
        new_env[i + 1] = '\0';
    }
    return (new_env);
}

char **unset_env(t_token *token, char **env)
{
    int i;
    int k;
    int l;
    char **new_env;

    k = 0;
    i = 0;
    l = 0;

    
    while (token->str[k])
        k++;
    while (env[i])
    {
        if (!strncmp((const char *)env[i], (const char *)token->str, k))
            l = 1;
        i++;
    }
    if (l == 1)
    {
        l = 0;
        new_env = malloc(sizeof(char **) * i + 1);
        if (!new_env)
            perror("MALLOC EXPORT");
        i = 0;
        while (env[i])
        {
            if (strncmp((const char *)env[i], (const char *)token->str, k))
            {
                new_env[l] = strdup(env[i]);
                l++;
            }
            i++;
        }
        new_env[l] = 0x00;
        return (new_env);
    }
    return (env);
}

int print_pwd(int size)
{
    int i;
    char buf[size];

    if (getcwd(buf, size) == NULL) 
    {
        print_pwd(size + 1);
        return (0);
    }
    printf("%s\n", buf);
    return (1);
}

void echo(char **str)
{
    int i;

    i = 1;

    if (!strcmp(str[1], "-n"))
        i++;
    while (str[i])
    {
        printf("%s ", str[i]);
        i++;
    }
    if (strcmp(str[1], "-n"))
        printf("\n");

}

int build_in (t_token *token, char ***env, t_pipe_exec *pipe_exec)
{
    int i;

    i = 0;
    if (!strcmp(pipe_exec->cmd, "exit"))                   
        return (1);

    if (!strcmp(pipe_exec->cmd, "env"))                                             
    {
        while (env[0][i])
            printf("%s\n", env[0][i++]);
        return (1);
    }
    if (!strcmp(pipe_exec->cmd, "export"))
        return (1); 
    if (!strcmp(pipe_exec->cmd, "unset"))
        return (1);
    if (!strcmp(pipe_exec->cmd, "pwd"))
    {
        print_pwd(0);
        return (1);
    }
    if (!strcmp(pipe_exec->cmd, "echo")) 
    {
        echo(pipe_exec->args);
        return (1);
    }
    if (!strcmp(pipe_exec->cmd, "cd"))    
    {
        return (1);
    }
    return (0);
}


void sort_env(char **env)
{
    int i;
    int j;
    int len_env;
    int rank;
    char **result;

    i = 0;
    j = 0;
    len_env = 0;
    rank = 0;

    while(env[len_env])
    {
        len_env++;
    }

    result = malloc(sizeof(char **) * len_env + 1);
    result[len_env] = '\0';

    while (j < len_env)
    {
        while (env[i])
        {
            if (ft_strcmpe(env[j], env[i]) > 0)
                rank++;
            i++;
        }
        result[rank] = ft_strjoin("declare -x ", env[j]);
        rank = 0;
        i = 0;
        j++;
    }

    i = 0;

    while(result[i])
    {
        printf("%s\n", result[i]);
        i++;
    }
}

//////////////////////////////////////  BUILD IN //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// EXECUTION //////////////////////////////////////////////////

void determine_pipe_num(t_token *token, t_pipe_exec *pipe_exec)
{
    t_token *tmp;

    tmp = token;
    pipe_exec->cmds_nbr = 1;
    while (tmp && !(tmp->type == 9 || tmp->type == 10))
    {
        if (tmp->type == 5)
            pipe_exec->cmds_nbr++;
        tmp = tmp->next;
    }
}

void    determine_inf_outf(t_token *token, t_pipe_exec *pipe_exec)
{
    t_token *tmp;

    tmp = token;
    pipe_exec->infile = NULL;
    pipe_exec->outfile = NULL;
    pipe_exec->here_inf = 0;
    while (tmp && !(tmp->type == 5 || tmp->type == 9 || tmp->type == 10))
    {
        if (tmp->type == 7)
            pipe_exec->infile = tmp->str;
        if (tmp->type == 8)
            pipe_exec->outfile = tmp->str;
        if (tmp->type == 14)
            pipe_exec->here_inf = 1;
        tmp = tmp->next;
    }
}

void    handle_cmd(t_token **token, t_pipe_exec *pipe_exec)
{
    pipe_exec->cmd = NULL;
    while (*token && !((*token)->type == 9 || (*token)->type == 10))
    {
        if ((*token)->type == 1)
            pipe_exec->cmd = (*token)->str;
        if (!(*token)->next || pipe_exec->cmd)
            break;
        *token = (*token)->next;
    }
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*pt_dest;
	const unsigned char	*pt_src;

	pt_dest = (unsigned char *)dest;
	pt_src = (const unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	while (n > 0)
	{
		pt_dest[n - 1] = pt_src[n - 1];
		n--;
	}
	return (dest);
}

char  *determine_chunk(char *wildcard, int *index1, int *index2)
{ 
  if (wildcard[*index2] == '*')
      (*index2)++;
  while (wildcard[*index2])
  {
    if (wildcard[*index2] == '*')
      break ;
    (*index2)++;
  }
}

void  extract_pattern(char **chunk, int chunk_type)
{
    char  *new_chunk;
    int wildcard_type;
    int size;

    size = chunk_type;
    if (!chunk_type)
        size++;
    new_chunk = malloc(sizeof(char) * (ft_strlen(*chunk) - size + 1));
    if (chunk_type == 1)
        ft_memcpy(new_chunk, *chunk, ft_strlen(*chunk) - size);
    else
        ft_memcpy(new_chunk, *chunk + 1, ft_strlen(*chunk) - size);
    new_chunk[ft_strlen(*chunk) - size] = 0;
    free(*chunk);
    *chunk = new_chunk;
}

void  determine_chunk_type(char *chunk, int *chunk_type)
{
  int count;
  int i;

  count = 0;
  i = 0;
  while (chunk[i])
  {
    if (chunk[i] == '*')
      count++;
    i++;
  }
  if (count == 1)
  {
    if (chunk[0] == '*')
      *chunk_type = 0;
    else
      *chunk_type = 1;
  }
  else
    *chunk_type = 2;
}

int check_match_type_0(char **file_name, char *chunk)
{
  *file_name += ft_strlen(*file_name) - ft_strlen(chunk);
  if (!strncmp(*file_name, chunk, ft_strlen(chunk)))
  {
    *file_name += ft_strlen(chunk);
    return (0);
  }
  else
    return (1);
}

int check_match_type_1(char **file_name, char *chunk)
{
  if (!strncmp(*file_name, chunk, ft_strlen(chunk)))
  {
    *file_name += ft_strlen(chunk);
    return (0);
  }
  else
    return (1);
}

int check_match_type_2(char **file_name, char *chunk)
{
  int i;

  i = 0;
  while ((*file_name)[i])
  {
    if (!strncmp(*file_name + i, chunk, ft_strlen(chunk)))
    {
      i += ft_strlen(chunk);
      *file_name += i;
      return (0);
    }
    i++;
  }
  return (1);
}

int  match_pattern(char **file_name, char *chunk, int chunk_type)
{
  int match;

  if (chunk_type == 0)
    match = check_match_type_0(file_name, chunk);
  if (chunk_type == 1)
    match = check_match_type_1(file_name, chunk); 
  if (chunk_type == 2)
    match = check_match_type_2(file_name, chunk);   
  if (match)
    return (1);
  return (0);
}

int wildcard_match(char *file_name, char *wildcard)
{
    //faire une structure
  char  *chunk;
  char  *str;
  int chunk_type;
  int file_match;
  int i;
  int j;

  file_match = 1;
  i = 0;
  j = 0;
  while (wildcard[i])
  {
    determine_chunk(wildcard, &i, &j);
    chunk = malloc(sizeof(char) * (j + 1 - i + 1));
    ft_memcpy(chunk, wildcard + i, j + 1 - i);
    chunk[j + 1 - i + 1] = 0;
    determine_chunk_type(chunk, &chunk_type);
    extract_pattern(&chunk, chunk_type);
    if (match_pattern(&file_name, chunk, chunk_type))
      return (1);
    if (!wildcard[j + 1])
      break ;
    i = j;  
  }
  if (file_match)
    return (0);
  return (1);
}

void    count_wild_args(char *wildcard, int *args_nbr)
{
    DIR *folder;
    struct dirent *entry;
    int read;

    folder = opendir(".");
    if (folder == NULL)
        return ;
    read = 1;
    while (read)
    {
        entry = readdir(folder);
        if (!entry)
            read = 0;
        else        
            if (!wildcard_match(entry->d_name, wildcard))
                (*args_nbr)++; 
    }
}

void   fill_tmp_array(char  **tmp_arr, char *wildcard, int args_nbr)
{
    DIR *folder;
    struct dirent *entry;
    int read;

    folder = opendir(".");
    if (folder == NULL)
        return ;
    read = 1;
    tmp_arr[args_nbr] = NULL;  
    while (read)
    {
        entry = readdir(folder);
        if (!entry)
            read = 0;
        else        
            if (!wildcard_match(entry->d_name, wildcard))
            {
                tmp_arr[args_nbr - 1] = entry->d_name;
                args_nbr--;
            }
    }
}

void    sort_array(char  **tmp_arr, int args_nbr)
{
    int is_sorted;
    int i;
    char    *tmp_swap;

    is_sorted = 1;   
    while (is_sorted)
    {
        is_sorted = 0;
        i = 0;
        while (i < args_nbr - 1)
        {
            if (ft_strcmp(tmp_arr[i], tmp_arr[i + 1]) > 0)
            {
                tmp_swap = tmp_arr[i];
                tmp_arr[i] = tmp_arr[i + 1];
                tmp_arr[i + 1] = tmp_swap;
                is_sorted = 1;
            }
            i++;
        }
    }
}

void    prepare_wild_args_arr(char **args, char **tmp_arr, int args_nbr, int index)
{
    int i;

    i = 0;
    while (i < args_nbr)
    {
        args[index] = tmp_arr[i];
        index++;
        i++;
    }
}

int    handle_wild_args(char *wildcard, char  **args, int *index)
{
    DIR *folder;
    struct dirent *entry;
    char    **tmp_arr;
    int read;
    int args_nbr;

    args_nbr = 0;
    count_wild_args(wildcard, &args_nbr);
    tmp_arr = malloc((args_nbr + 1) * sizeof(char *));
    if (!tmp_arr)
        return (1);
    fill_tmp_array(tmp_arr, wildcard, args_nbr);
    sort_array(tmp_arr, args_nbr);
    prepare_wild_args_arr(args, tmp_arr, args_nbr, *index);
    *index += args_nbr;
}
/*
void    handle_args(t_token **token, t_pipe_exec *pipe_exec)
{
    t_token *tmp;
    int i;

    tmp = *token;
    pipe_exec->args_nbr = -1;
    i = -1;  
    // faire une fonction
    while (tmp && (tmp->type == 1 || tmp->type == 16))
    {
        if (tmp->type == 16)
            count_wild_args(tmp->str, &pipe_exec->args_nbr);   
        else
            pipe_exec->args_nbr++;
        tmp = tmp->next;
    }
    //
    pipe_exec->args = malloc((pipe_exec->args_nbr + 2) * sizeof(char *));
    pipe_exec->args[++i] = pipe_exec->cmd;
    if (*token)
        (*token) = (*token)->next;
    i++;
    while (*token && ((*token)->type == 1 || (*token)->type == 16))
    {
        if ((*token)->type == 16)
            handle_wild_args((*token)->str, pipe_exec->args, &i);
        else
        {
            if (i < pipe_exec->args_nbr + 1)
            {
                pipe_exec->args[i] = (*token)->str;
                i++;
            } 
        }        
        *token = (*token)->next;
    }
    pipe_exec->args[i] = NULL; 
}
*/

void    handle_args(t_token **token, t_pipe_exec *pipe_exec)
{
    t_token *tmp;
    int i;

    tmp = *token;
    pipe_exec->args_nbr = -1;
    i = -1;  
    
    while (tmp && (tmp->type == 1 || tmp->type == 16))
    {
        if (tmp->type == 16)
            count_wild_args(tmp->str, &pipe_exec->args_nbr);   
        else
            pipe_exec->args_nbr++;
        tmp = tmp->next;
    }
    //
    pipe_exec->args = malloc((pipe_exec->args_nbr + 2) * sizeof(char *));
    pipe_exec->args[++i] = pipe_exec->cmd;
    if (*token)
        (*token) = (*token)->next;
    i++;
    while (*token && ((*token)->type == 1 || (*token)->type == 16))
    {
        if ((*token)->type == 16)
            handle_wild_args((*token)->str, pipe_exec->args, &i);
        else
            if (i < pipe_exec->args_nbr + 1)
                pipe_exec->args[i++] = (*token)->str;       
        *token = (*token)->next;
    }
    pipe_exec->args[i] = NULL; 
}

void    determine_path(char *cmd, char **path, char **env)
{
    int i;

    if (cmd[0] == '/')
    {
        *path = NULL;
        return ;
    }
    i = -1;
    while (env[++i])
    {
        if (!strncmp(env[i], "PATH=", 5))
            break;
    }
    *path = env[i];
}

int determine_cmd_no_path(t_pipe_exec *pipe_exec)
{
    int fd;

    pipe_exec->cmd = ft_strdup(pipe_exec->cmd);
    if (!(pipe_exec->cmd))
        return (1);
    pipe_exec->args[0] = pipe_exec->cmd;
    if (pipe_exec->here_inf)
    {
        //close(pipe_exec->here_pipe_fd[0]);
        close(pipe_exec->here_pipe_fd[1]);
    }
    execve(pipe_exec->args[0], pipe_exec->args, *pipe_exec->env);
    write(2, "minishell: ", 11);
    perror(pipe_exec->cmd);
    free(pipe_exec->args[0]);
    free(pipe_exec->args);
}

int determine_cmd_path(t_pipe_exec *pipe_exec)
{
    int i;
    char    *tmp;
    char    *save_cmd;

    i = 0;
    pipe_exec->path += 5;
    save_cmd = pipe_exec->cmd;
    //printf("OKAY1\n");
    while (pipe_exec->path[i])
    {
        i = 0;
        // faire une fonction pour raccourcir
        while (pipe_exec->path[i] && pipe_exec->path[i] != ':')
            i++;
        pipe_exec->path[i] = 0;
        tmp = ft_strjoin(pipe_exec->path, "/");
        pipe_exec->cmd = ft_strjoin(tmp, pipe_exec->cmd);
        if (!tmp || !(pipe_exec->cmd))
            return (1);
        free(tmp);
        //
        pipe_exec->args[0] = pipe_exec->cmd;
        if (pipe_exec->here_inf)
        {
            //close(pipe_exec->here_pipe_fd[0]);
            close(pipe_exec->here_pipe_fd[1]);
        }
        execve(pipe_exec->args[0], pipe_exec->args, *pipe_exec->env);
        free(pipe_exec->args[0]);
        i++;
        pipe_exec->path +=  i;
        pipe_exec->cmd = save_cmd;
    }
    write(2, "minishell: ", 11);
    perror(pipe_exec->cmd);
    free(pipe_exec->args);
}


int    infile_redirection(t_pipe_exec *pipe_exec)
{  
    if (pipe_exec->infile)
    {
        //close(*input);
        pipe_exec->input = open(pipe_exec->infile, O_RDONLY);
        if (pipe_exec->input == -1)
            return (1);
    }
    
    if (pipe_exec->here_inf)
    {
        if (pipe(pipe_exec->here_pipe_fd) == -1)
            return (1);
        pipe_exec->input = pipe_exec->here_pipe_fd[0];
    }
    
    if (!(pipe_exec->infile || pipe_exec->here_inf) && pipe_exec->first_cmd)
    {
        //close(*input);
        pipe_exec->input = dup(pipe_exec->save_input);
    }
    //if (!infile && !first_cmd)

    return (0);
}

int    outfile_redirection(char  *outfile, int   *output, int save_output, int cmds_nbr, char *cmd)
{
    FILE *ptr = stderr;


    if (outfile)
    {
        //close(*output);
        //fprintf(ptr, "ICI0\n");
        *output = open(outfile, O_CREAT | O_WRONLY, 0664);
        if (*output == -1)
            return (1);
    }
    if (!outfile && cmds_nbr == 1)
    {
        //fprintf(ptr, "ICI1\n");
        //fprintf(ptr, "cmd: %s\n", cmd);
        
        *output = dup(save_output);
    }
    return (0);
}

int pipe_redirections(int   pipe_fd[2], int *input, int *output, int first_cmd)
{

    if (first_cmd)
    {
        if (pipe(pipe_fd) == -1)
            return (1);
        *input = pipe_fd[0];
        *output = pipe_fd[1];
    }
    else
    {
        *input = pipe_fd[0];
        if (pipe(pipe_fd) == -1)
            return (1);
       *output = pipe_fd[1];
    }
    return (0);
}

void    restore_fd(int save_input, int save_output)
{
    dup2(save_input, STDIN_FILENO);
    dup2(save_output, STDOUT_FILENO);
    close(save_input);
    close(save_output);
}

int handle_execve(t_token **token, t_pipe_exec *pipe_exec)
{
    FILE *ptr = stderr;

    determine_path(pipe_exec->cmd, &pipe_exec->path, *pipe_exec->env);
    if (!pipe_exec->path)
        determine_cmd_no_path(pipe_exec);
    else
        determine_cmd_path(pipe_exec);
    return (0);
}

void    inf_outf_redirections(t_token *token, t_pipe_exec *pipe_exec)
{
    FILE *ptr = stderr;
    determine_inf_outf(token, pipe_exec);
    //printf("pipe_exec->here_inf: %d\n", pipe_exec->here_inf);
    
    //infile_redirection(pipe_exec->infile, &pipe_exec->input, pipe_exec->save_input, pipe_exec->first_cmd);
    infile_redirection(pipe_exec);
    
    outfile_redirection(pipe_exec->outfile, &pipe_exec->output, pipe_exec->save_output, pipe_exec->cmds_nbr, token->str);
    
}

void    write_here_doc(int  here_pipe_fd_out,   t_token **here_doc_tab, int *here_doc_tab_index)
{
    //write(here_pipe_fd_out, , 5);
    int i;
    int j;

    i = 0;
    FILE *ptr = stderr;
    /*
    while (here_doc_tab[i])
    {   
        //j = 0;
        while (here_doc_tab[i])
        {
            //write(here_pipe_fd_out, &here_doc_tab[i]->str[j], 1);
            //printf("%s", here_doc_tab[i]->str);
            j = 0;
            while (here_doc_tab[i]->str[j])
            {
                write(here_pipe_fd_out, &here_doc_tab[i]->str[j], 1);
                j++;
            }
            here_doc_tab[i] = here_doc_tab[i]->next;
        }
        
        
        i++;
    }
    */  
    //j = 0;
    while (here_doc_tab[*here_doc_tab_index])
    {
        //write(here_pipe_fd_out, &here_doc_tab[i]->str[j], 1);
        //fprintf(ptr, "la: %d\n", *here_doc_tab_index);
        //fprintf(ptr, "la: %s\n", here_doc_tab[*here_doc_tab_index]->str);
        j = 0;
        
        while (here_doc_tab[*here_doc_tab_index]->str[j])
        {
            write(here_pipe_fd_out, &here_doc_tab[*here_doc_tab_index]->str[j], 1);
            j++;
        }
        
        here_doc_tab[*here_doc_tab_index] = here_doc_tab[*here_doc_tab_index]->next;
        
    }
    (*here_doc_tab_index)++;   
    //*here_doc_tab_index = 1;
        
    close(here_pipe_fd_out);
}

int handle_execution(t_token **token, t_pipe_exec *pipe_exec, int *pid)
{
    char *str;
    char line [100];
    int i = 0;
    FILE *ptr = stderr;
    *pid = 1;
    
    pipe_redirections(pipe_exec->pipe_fd, &pipe_exec->input, &pipe_exec->output, pipe_exec->first_cmd);

    
    inf_outf_redirections(*token, pipe_exec);


    dup2(pipe_exec->input, STDIN_FILENO);
    close(pipe_exec->input);
    dup2(pipe_exec->output, STDOUT_FILENO);
    close(pipe_exec->output);

    handle_cmd(token, pipe_exec);
    handle_args(token, pipe_exec);
    

    if (!build_in(*token, pipe_exec->env, pipe_exec))
        *pid = fork();
    
    

    if (!(*pid))
    {
        close(pipe_exec->pipe_fd[0]);
        close(pipe_exec->pipe_fd[1]);
        close(pipe_exec->save_input);
        close(pipe_exec->save_output);
		handle_execve(token, pipe_exec);       
        exit(127);
    }


    if (pipe_exec->here_inf)
        write_here_doc(pipe_exec->here_pipe_fd[1], pipe_exec->here_doc_tab, &pipe_exec->here_doc_tab_index);



    if (*pid)
	{       
		if (!strcmp(pipe_exec->cmd, "cd"))
		{
			if(chdir((const char *)pipe_exec->args[1]) == -1)
				fprintf(stderr, "%d\n", errno);
			return (1);
		}
		if (!strcmp(pipe_exec->cmd, "export"))
		{
            if (!pipe_exec->args[1] && ((!*token) || (*token) && (*token)->type != 13))
                sort_env(*(pipe_exec)->env);

            if ((*token) && (*token)->type == 13)
			{
				if (!ft_isalpha((*token)->str[0])) 
				{
					printf("export: `%s': not a valid identifier", (*token)->str);
					return (1);
				}	
				*(pipe_exec)->env = export_env(*token, *(pipe_exec)->env);
				return (1);
			}
		}
		if (!strcmp(pipe_exec->cmd, "unset"))
		{
			if ((*token)->type == 2)
				*(pipe_exec)->env = unset_env(*token, *(pipe_exec)->env);
		}
		if (!strcmp(pipe_exec->cmd, "exit"))                
			exit(0); 
	}
	return (1);

}









int find_operator(t_token **token)
{
    while (*token)
    {
        if ((*token)->type == 9)
        {
            *token = (*token)->next;
            return (1);
        }
        if ((*token)->type == 10)
        {
            *token = (*token)->next;
            return (2);
        }
        if (!(*token)->next)
            return (-1);
        *token = (*token)->next;
    }
}

int handle_continue(t_token **token, int status, int operator)
{
    if (operator == 1 && !status)
        return (0);
    if (operator == 2 && status)
        return (0);   
    return (1);
}



int move_next_cmds(t_token **token, int status)
{
    int operator;
    int continue_exec;

    continue_exec = 1;
    while (continue_exec)
    {
        operator = find_operator(token);
        if (operator == -1)
            return (-1);
        continue_exec = handle_continue(token, status, operator);
        if (!continue_exec)
            return (0);
        // faire une fonction
        if ((*token)->type == 11)
        {
            while ((*token)->type != 12)
                *token = (*token)->next;
        }
        else
        {
            while (*token && ((*token)->type != 9 && (*token)->type != 10))
            {
                if ((*token)->next)
                    *token = (*token)->next;
                else
                    return (-1);
            }
        }
        //
    }
    return (0);
}

int move_next_cmds_pipe(t_token **token)
{
    while (*token)
    {
        if ((*token)->type == 5)
        {
            *token = (*token)->next;
            return (1);
        }
        if (!(*token)->next)
            break ;
        *token = (*token)->next;
    }
    return (-1);
}

int    execute_commands(t_token *token, t_pipe_exec *pipe_exec)
{
    int pid;

    pipe_exec->first_cmd = 1;
    pipe_exec->save_input = dup(STDIN_FILENO);
    pipe_exec->save_output = dup(STDOUT_FILENO);
   
    determine_pipe_num(token, pipe_exec);
    
    
    while (pipe_exec->cmds_nbr > 0)
    {
        handle_execution(&token, pipe_exec, &pid);
        //printf("pipe_exec->here_doc_tab_index: %d\n", pipe_exec->here_doc_tab_index);
        pipe_exec->cmds_nbr--;
        if (pipe_exec->cmds_nbr > 0)
            move_next_cmds_pipe(&token);
        pipe_exec->first_cmd = 0;        
    }
    restore_fd(pipe_exec->save_input, pipe_exec->save_output);
    waitpid(pid, &pipe_exec->status, 0);
    //printf("pipe_exec->status: %d\n", WEXITSTATUS(pipe_exec->status));
    pipe_exec->status = WEXITSTATUS(pipe_exec->status);
    while (wait(NULL) != -1);
    close(pipe_exec->save_input);
    close(pipe_exec->save_output);
    //free(pipe_exec->args);
    
    return (0);
}



int    execute_line(t_minishell *minishell)
{
    t_pipe_exec pipe_exec;
    int continue_exec;

    pipe_exec.env = minishell->env;
    pipe_exec.here_doc_tab = minishell->here_doc_tab;
    pipe_exec.here_doc_tab_index = 0;
    continue_exec = 0;
    while (continue_exec != -1)
    {
       
        execute_commands(minishell->token, &pipe_exec);
        continue_exec = move_next_cmds(&minishell->token, pipe_exec.status);

    } 
    return (0);
}

////////////////////////////////////// EXECUTION //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void		ft_putstr_fd(char const *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

t_db_list *init_minishell(t_minishell *minishell, char **env[])
{
    minishell->env = env;
    minishell->token = NULL;
    minishell->extension = NULL;
    minishell->info = init_list(minishell->info);
    minishell->grammar = 1;
    minishell->here_doc = -1;
    minishell->quote = 0;
    return (minishell->info);
}

void	handler(int code)
{
    if (code == SIGINT)
    {
	    //(exit_status à 1 (comportement similaire à celui de Bash)
        rl_replace_line("", 0); //(vide le buffer de readline)
        ft_putendl("");
        rl_on_new_line(); //(changement de ligne)
        rl_replace_line("", 0); //(vide le buffer de readline)
        rl_redisplay(); //(affiche la nouvelle ligne) En gros, on quitte la ligne courante pour afficher un nouveau prompt sur une nouvelle ligne.
    }

    if (code == SIGQUIT)
    {
	    
    }
}





int main(int argc, char *argv[], char *env[])
{
    t_minishell minishell;
    struct sigaction prepaSignal;

    minishell.info = init_minishell(&minishell, &env);

    prepaSignal.sa_handler=&handler;
    sigaction(SIGINT,&prepaSignal,0);
    sigaction(SIGQUIT,&prepaSignal,0);
    minishell.line = readline("minishell$ ");
    prepaSignal.sa_handler=SIG_IGN;

    while (minishell.line)
    {
        parse_line(&minishell, 0);

        minishell.token = minishell.info->first;

        while (minishell.token)
        {
            /*if (!(ft_grammar(&minishell.token)))
            {
                printf("token = %s\ntype = %d\n\n", minishell.token->str, minishell.token->type);
                minishell.grammar = -1;
                break; 
            }*/
            
            //printf("token = %s\ntype = %d\n\n", minishell.token->str, minishell.token->type);

            add_history(minishell.line);
            
            minishell.token = minishell.token->next;

        }

        minishell.token = minishell.info->first;
        
        if (minishell.token)
        {
            minishell.here_doc_tab = here_doc(minishell.token, minishell.info->first);
            //print_here_doc(minishell.here_doc_tab);  //simple fonction pour afficher
        }
        //printf("minishell.here_doc_tab: %p\n", minishell.here_doc_tab);
        if (minishell.grammar > 0 && ft_strlen(minishell.line))
        {
            while (minishell.token && minishell.token->previous)                 
                minishell.token = minishell.token->previous;

            if (minishell.token)
                if (execute_line(&minishell))
                    return (1);
        }

        

        
        

        ////////////////////////////////////// Reboot de la line et liberation des structures////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        

        if (ft_strlen(minishell.line))
            free_list(minishell.token);

        free(minishell.info);

        minishell.info = init_list(minishell.info);
        
        free(minishell.line);

        prepaSignal.sa_handler=&handler;
        minishell.line = readline("minishell$ ");
        prepaSignal.sa_handler=SIG_IGN;
    


    }
    return (1);
}


// GERER bug ls *.
// problème parsing: echo test0 || (echo test1 || echo test2) le token parenthèse est: test2 au lieu de )