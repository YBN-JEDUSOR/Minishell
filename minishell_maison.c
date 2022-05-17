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

        /*if ((*token)->type == 7 && (!(*token)->next))
        {
                printf("bash4: syntax error near unexpected token `newline'\n");
                return (0);
        }

        if ((*token)->type == 8 && (!(*token)->next))
        {
                printf("bash3: syntax error near unexpected token `newline'\n");
                return (0);
        }*/

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
    t_token *new_element;
    new_element = malloc(sizeof(t_token));
    if (!new_element)
        perror ("MALLOC CREATE LIST");
    new_element->next = NULL;
    new_element->previous = NULL;
    new_element->str = str;
    new_element->type = type;
    info->first = new_element;
    info->last = new_element;
    info->lenght = 1;
    return (new_element);
}

t_token *push_full_list (t_db_list *info, t_token *token, char *str, int type)
{
    t_token *new_element;
    new_element = malloc(sizeof(t_token));
    if (!new_element)
        perror ("MALLOC CREATE LIST");
    new_element->str = str;
    new_element->type = type;
    new_element->next = NULL;
    info->last->next = new_element;
    new_element->previous = info->last;
    info->last = new_element;
    info->lenght = info->lenght + 1;

    return (new_element);
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
    result = malloc(sizeof(char*) * 1);
    if (!result)
        perror("MALLOC CHECK_EXTENSION");
    result[0] = 0x00;
    return (result);
}

char **export_env(t_token *token, char **env)
{
    int i;
    int k;
    int l;

    k = 0;
    i = 0;
    l = 0;

    while (token->str && token->str[k] != '=')
        k++;
    char **new_element;
    while (env[i])
        i++;
    new_element = malloc(sizeof(char **) * i + 1);
    if (!new_element)
        perror("MALLOC EXPORT");
    i = 0;
    while (env[i])
    {
        if (strncmp((const char *)env[i], (const char *)token->str, k))
            new_element[i] = strdup(env[i]);
        if (!strncmp((const char *)env[i], (const char *)token->str, k))
        {
            new_element[i] = strdup(token->str);
            l++;
        }
        i++;
    }
    if (l == 0 )
        new_element[i] = strdup(token->str);
    new_element[i + 1] = 0x00;
    return (new_element);
}

char **unset_env(t_token *token, char **env)
{
    int i;
    int k;
    int l;
    char **new_element;

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
        new_element = malloc(sizeof(char **) * i + 1);
        if (!new_element)
            perror("MALLOC EXPORT");
        i = 0;
        while (env[i])
        {
            if (strncmp((const char *)env[i], (const char *)token->str, k))
            {
                new_element[l] = strdup(env[i]);
                l++;
            }
            i++;
        }
        new_element[l] = 0x00;
        return (new_element);
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
        print_pwd(10);
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
    while (tmp && !(tmp->type == 5 || tmp->type == 9 || tmp->type == 10))
    {
        if (tmp->type == 7)
            pipe_exec->infile = tmp->str;
        if (tmp->type == 8)
            pipe_exec->outfile = tmp->str;
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

void    handle_args(t_token **token, t_pipe_exec *pipe_exec)
{
    t_token *tmp;
    int i;

    tmp = *token;
    pipe_exec->args_nbr = -1;
    i = -1;
    while (tmp && tmp->type == 1)
    {
        pipe_exec->args_nbr++;
        tmp = tmp->next;
    }
    pipe_exec->args = malloc((pipe_exec->args_nbr + 2) * sizeof(char *));
    pipe_exec->args[++i] = pipe_exec->cmd;
    if (*token)
        (*token) = (*token)->next;
    while (*token && (*token)->type == 1)
    {
        pipe_exec->args[++i] = (*token)->str;
        *token = (*token)->next;
    }
    pipe_exec->args[i + 1] = NULL;
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

int determine_cmd_no_path(char *cmd, char **args, char **env)
{
    int fd;

    cmd = ft_strdup(cmd);
    if (!(cmd))
        return (1);
    args[0] = cmd;
    execve(args[0], args, env);
    write(2, "minishell: ", 11);
    perror(cmd);
    free(args[0]);
    free(args);
}

int determine_cmd_path(char *cmd, char **args, char **env, char *path)
{
    int i;
    char    *tmp;
    char    *save_cmd;

    i = 0;
    path += 5;
    save_cmd = cmd;
    while (path[i])
    {
        i = 0;
        // faire une fonction pour raccourcir
        while (path[i] && path[i] != ':')
            i++;
        path[i] = 0;
        tmp = ft_strjoin(path, "/");
        cmd = ft_strjoin(tmp, cmd);
        if (!tmp || !(cmd))
            return (1);
        free(tmp);
        //
        args[0] = cmd;
        execve(args[0], args, env);
        free(args[0]);
        i++;
        path +=  i;
        cmd = save_cmd;
    }
    write(2, "minishell: ", 11);
    perror(cmd);
    free(args);
}
/*
int handle_cmd_args(t_token **token, t_pipe_exec *pipe_exec)
{
    int found;
    
    handle_cmd(token, pipe_exec);
    handle_args(token, pipe_exec);
    /*
    if (check_build_in(pipe_exec))
        return (0);

    determine_path(pipe_exec->cmd, &pipe_exec->path, *pipe_exec->env);
    if (!pipe_exec->path)
        found = determine_cmd_no_path(pipe_exec->cmd, pipe_exec->args, *pipe_exec->env);
    else
        found = determine_cmd_path(&pipe_exec->bin, pipe_exec->cmd, pipe_exec->path);
    if (found)
        return (1);
    return (0);
}
*/
int    infile_redirection(char  *infile, int   *input, int save_input, int first_cmd)
{  
    if (infile)
    {
        //close(*input);
        *input = open(infile, O_RDONLY);
        if (*input == -1)
            return (1);
    }
    if (!infile && first_cmd)
    {
        //close(*input);
        *input = dup(save_input);
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

    handle_cmd(token, pipe_exec);
    
    handle_args(token, pipe_exec);
    
    determine_path(pipe_exec->cmd, &pipe_exec->path, *pipe_exec->env);
    
    if (!pipe_exec->path)
        determine_cmd_no_path(pipe_exec->cmd, pipe_exec->args, *pipe_exec->env);
    else
        determine_cmd_path(pipe_exec->cmd, pipe_exec->args, *pipe_exec->env, pipe_exec->path);
    return (0);
}

void    inf_outf_redirections(t_token *token, t_pipe_exec *pipe_exec)
{
    FILE *ptr = stderr;
    determine_inf_outf(token, pipe_exec);
    
    infile_redirection(pipe_exec->infile, &pipe_exec->input, pipe_exec->save_input, pipe_exec->first_cmd);
    
    outfile_redirection(pipe_exec->outfile, &pipe_exec->output, pipe_exec->save_output, pipe_exec->cmds_nbr, token->str);
    /*
    if (pipe_exec->infile)
        handle_infile(pipe_exec->input, pipe_exec->infile);
    else
    {
        if (pipe_exec->first_cmd)
            dup(pipe_exec->save_input, STDIN_FILENO);
    }
    */
    //if (pipe_exec->outfile)

}

int handle_execution(t_token **token, t_pipe_exec *pipe_exec, int *pid)
{
    char *str;
    char line [100];
    int i = 0;
    FILE *ptr = stderr;
    /*
    if (handle_cmd_args(token, pipe_exec))
        return (1);
    */
    /*
    if ((*pipe_exec)->here_doc != -1)
        (*pipe_exec)->input = (*pipe_exec)->here_doc;
    */
    //printf("fichier = %d",(*pipe_exec)->input);

    
    //peut etre mettre pipe_fd en variable locale
    pipe_redirections(pipe_exec->pipe_fd, &pipe_exec->input, &pipe_exec->output, pipe_exec->first_cmd);
    //printf("pipe_exec->pipe_fd[0]: %d\n", pipe_exec->pipe_fd[0]);
    
    inf_outf_redirections(*token, pipe_exec);
    //fprintf(ptr, "pipe_exec->cmds_nbr6673: %d\n", pipe_exec->cmds_nbr);

    dup2(pipe_exec->input, STDIN_FILENO);
    close(pipe_exec->input);
    dup2(pipe_exec->output, STDOUT_FILENO);
    close(pipe_exec->output);

    //printf("pipe_exec->input: %d\n", pipe_exec->input);
    //determine_inf_outf(*token, pipe_exec);
    //printf("pipe_exec->infile: %s\n", pipe_exec->infile);
    //printf("pipe_exec->outfile: %s\n", pipe_exec->outfile);
    
    //dup2(pipe_exec->input, STDIN_FILENO);

    //close(pipe_exec->input);


    /*
    if (pipe_exec->cmds_nbr == 1)
        output_redirection(pipe_exec->outfile, &pipe_exec->output, pipe_exec->save_output);
    
    else
        pipe_redirections(pipe_exec->pipe_fd, &pipe_exec->input, &pipe_exec->output);
    dup2(pipe_exec->output, STDOUT_FILENO);
    close(pipe_exec->output);
    */
    *pid = fork();
    
    
    if (!(*pid))
    {

        
        //if (!build_in(*token, pipe_exec->env, *pipe_exec))
        //fprintf(fptr,"(*token)->str_before: %s\n", (*token)->str);
        /*
        if (pipe_exec->cmds_nbr != 1)
            close(pipe_exec->input);
        */
        //close(pipe_exec->input);
        close(pipe_exec->pipe_fd[0]);
        close(pipe_exec->pipe_fd[1]);
        close(pipe_exec->save_input);
        close(pipe_exec->save_output);
        handle_execve(token, pipe_exec);      
        exit(127);
    }
    /*
    pipe_exec->input = pipe_exec->pipe_fd[0];
    pipe_exec->output = pipe_exec->pipe_fd[1];
    */
    /*
    if (*id)
    {       
        if (!strcmp((*pipe_exec)->cmd, "cd"))
        {
            if(chdir((const char *)(*pipe_exec)->args[1]) == -1)
                fprintf(stderr, "%d\n", errno);
            return (1);
        }
        if (!strcmp((*pipe_exec)->cmd, "export"))
        {
            if ((*token)->type == 13)
                *env = export_env(*token, *env);
            return (1);
        }
        if (!strcmp((*pipe_exec)->cmd, "unset"))
        {
            if ((*token)->type == 2)
                *env = unset_env(*token, *env);
        }
        if (!strcmp((*pipe_exec)->cmd, "exit"))                
            exit(0); 
    }
    */
    //printf("ICI\n");
    return (1);

}

int move_next_cmds(t_token **token)
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
            break ;
        *token = (*token)->next;
    }
    return (-1);
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
    //input_redirection(pipe_exec->infile, &pipe_exec->input, pipe_exec->save_input); A GERER AU NIVEAU DE CHAQUE COMMANDE
    determine_pipe_num(token, pipe_exec);
    
    
    while (pipe_exec->cmds_nbr > 0)
    {
        handle_execution(&token, pipe_exec, &pid);
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
    continue_exec = 0;
    while (continue_exec != -1)
    {
        //determine_in_out(minishell->token, &pipe_exec); A GERER AU NIVEAU DE CHAQUE COMMANDE
        //determine_pipe_num(minishell->token, &pipe_exec); A GERER AU NIVEAU DE PIPEX
        //pipe_exec.env = minishell->env; A GERER AU NIVEAU DE CHAQUE COMMANDE
        //pipe_exec.here_doc = minishell->here_doc; A GERER AU NIVEAU DE CHAQUE COMMANDE
        if (!continue_exec)
            execute_commands(minishell->token, &pipe_exec);
        else
        {
            if (continue_exec == 1 && !pipe_exec.status)
                execute_commands(minishell->token, &pipe_exec);
            if (continue_exec == 2 && pipe_exec.status != 0)
                execute_commands(minishell->token, &pipe_exec);
        }
        continue_exec = move_next_cmds(&minishell->token);
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
            
            printf("token = %s\ntype = %d\n\n", minishell.token->str, minishell.token->type);

            add_history(minishell.line);
            
            minishell.token = minishell.token->next;

        }

        minishell.token = minishell.info->first;

        if (minishell.token)
        {
            minishell.here_doc_tab = here_doc(minishell.token, minishell.info->first);
            print_here_doc(minishell.here_doc_tab);  //simple fonction pour afficher
        }

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
