#include "minishell.h"


int is_token(char c)
{
    if (c == ' ' || c == '|' || c == '<' || c == '(' || c == ')' || c == '&' || c == '>')
        return (0);
    return (1);
}

char * dollars_sign(char *str, t_token **token, int *i, t_db_list **info, char **env, int quote)
{
    int b;
    int a;
    char *result;

    b = 0;
    *i = (*i) + 1;
    a = (*i);
            
    while (str[(*i)] && str[(*i)] != ' ' && str[(*i)] != 34 && is_token(str[(*i)]))
        *i = (*i) + 1;
    result = malloc(sizeof(char *) * ((*i) - a + 1));
    if (!result)
        perror("MALLOC RESULT PARSING");
    while (a < (*i))
    {
        result[b] = str[a];
        b++;
        a++;
    }
    result[b] = '\0';
    if (quote == 1)
        return (check_extension(result, env));
    *token = push_list(*info, *token, check_extension(result, env), 2);
    while (str[(*i)] && str[(*i)] == ' ')
        *i = (*i) + 1;
    parse_line (str, token, (*i), info, env, quote);
    return (0);
}

int simple_pipe(char *str, t_token **token, int *i, t_db_list **info, char **env, int quote)
{
    char *result;

    result = malloc(sizeof(char *) * (2));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = str[*i];
    result[1] = '\0';
    (*i)++;
    *token = push_list(*info, *token, result, 5);
    while (str[*i] && str[*i] == ' ')
        (*i)++;
    parse_line (str, token, *i, info, env, quote);
    return (0);
}

int parenthese(char *str, t_token **token, int *i, t_db_list **info, char **env, int quote)
{
    int utils;
    char *result;

    utils = 11;
    if (str[*i] == ')')
        utils = 12;
    result = malloc(sizeof(char *) * (2));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = str[*i];
    result[1] = '\0';
    (*i)++;
    *token = push_list(*info, *token, result, utils);         
    while (str[*i] && str[*i] == ' ')
        (*i)++;
    parse_line (str, token, *i, info, env, quote);
    return (0);
}

int double_operator (char *str, t_token **token, int *i, t_db_list **info, char **env, int quote, int type)
{
    char *result;

    result = malloc(sizeof(char *) * (3));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = str[*i];
    result[1] = str[*i];
    result[2] = '\0';
    (*i) = (*i) + 2;
    *token = push_list(*info, *token, result, type);;
    while (str[*i] && str[*i] == ' ')
        (*i)++;
    parse_line (str, token, *i, info, env, quote);
    return (0);
}


int is_wildcard(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '*')
            return (16);
        i++;
    }
    i = 0;
    return (1);
}



int parse_line (char *str, t_token **token, int i, t_db_list **info, char **env, int quote)
{
    char                *result; 
    int                 a;
    int                 b;
    int                 utils;

    utils = 0;
    while (str[i])
    {   
        if (str[i] == '|' && str[i + 1] != '|')
        {
            simple_pipe(str, token, &i, info, env, quote);
            return (0);   
        }
        
        if (str[i] == '$')
        {
            dollars_sign (str, token, &i, info, env, 0);
            return (0);
        }

        if ((str[i] == '<' && str[i + 1] != '<') || (str[i] == '>' && str[i + 1] != '>'))     // voir pou la grammaie fonctionneme tet optimisation
        {
            utils = 7;
            if (str[i] == '>')
                utils = 8;
            i++;
            while (str[i] && str[i] == ' ')
                i++;
            b = 0;
            a = i;
            while (str[i] && str[i] != ' ')
            {
                i++;
                if (str[i] == ')')
                    break;
            }
            result = malloc(sizeof(char *) * (i - a + 1));
            if (!result)
                perror("MALLOC RESULT PARSING");
            while (a < i)
            {
                result[b] = str[a];
                b++;
                a++;
            }
            result[b] = '\0';
            *token = push_list(*info, *token, result, utils);         
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, i, info, env, quote);
            return (0);
        }

        if (str[i] == '(' || str[i] == ')')
        {
            parenthese(str, token, &i, info, env, quote);
            return (0);
        }

        if (str[i] == '&' && str[i + 1] == '&')
        {
            double_operator (str, token, &i, info, env, quote, 9);
            return (0);   
        }

        if (str[i] == '|' && str[i + 1] == '|')
        {
            double_operator (str, token, &i, info, env, quote, 10);
            return (0);   
        }
        
        if (str[i] == '<' && str[i + 1] == '<')
        {
            double_operator (str, token, &i, info, env, quote, 14);
            return (0);   
        }

        if (str[i] == '>' && str[i + 1] == '>')
        {
            double_operator (str, token, &i, info, env, quote, 15);
            return (0);
        }

        if (str[i])
        {
            b = 0;
            a = i;

            result = malloc(sizeof(char *) * (1));
            if (!result)
                perror("MALLOC RESULT PARSING");
            result[0] = '\0';

            while (str[i] && is_token(str[i]))
            {
                if (str[i] == 34)
                {
                    i++;
                    b = 1;
                }
                if (str[i] == 39)
                    i++;
                if (str[i] == '$' && b == 1)
                    result = ft_strjoin(result, dollars_sign(str, token, &i, info, env, 1));
                else
                {
                    if (str[i] == '=')
                        utils = 1;
                    result = ft_strjoin(result, ft_substr(str, i, 1));
                    i++;
                }
            }
            if (utils == 0)
                *token = push_list(*info, *token, result, is_wildcard(str));

            if (utils == 1)
            {
               *token = push_list(*info, *token, result, 13);
                utils = 0;
            }
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, i, info, env, quote);
            return (0);
        }
    }
    return (1);
}


t_token **here_doc(t_token *token)
{
    char    *str;
    int     size;
    int     i;
    t_token **here_doc;

    
    size = 0;
    i = 0;
    while (token->previous)                 
    {
        if (token && token->type == 14 && token->next && token->next->type == 1)
            size++;
        token = token->previous;
    }

    here_doc = malloc(sizeof(t_token*) * size + 1);
    if (!here_doc)
        return (0);
    here_doc[size] = '\0';

    while (token && token->next)                 
    {
        if (token && token->type == 14 && token->next && token->next->type == 1)
        {
            here_doc[i] = put_here_doc(token);
            i++;
        }
        token = token->next;
    }

    return (here_doc);
}

t_token *put_here_doc(t_token *token)
{
    char    *str;

    t_token     *result;
    t_db_list   *info;
    result = NULL;
    info = init_list(info);

    str = readline("> ");
    while (strcmp(str, token->next->str))
    {
        result = push_list(info, result, ft_strjoin(str, "\n"), 1);
        str = readline("> ");
    }

    while (result && result->previous)                 
        result = result->previous;

    return (result);
}

void print_here_doc(t_token **token)  //Sert a rien a part veirifier le bon fonctionement
{
    int i = 0;

    while (token[i])
    {
        while (token[i])
        {
            printf("%s",token[i]->str);
            token[i] = token[i]->next;
        }
        i++;
    }
}
    











