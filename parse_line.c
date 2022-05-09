#include "minishell.h"

char * dollars_sign(char *str, t_token **token, int *i, t_db_list **info, char **env, int quote)
{
    int b;
    int a;
    char *result;

    b = 0;
    *i = (*i) + 1;
    a = (*i);
            
    while (str[(*i)] && str[(*i)] != ' ' && str[(*i)] != 34)                   /// FAIRE UNE FOCNtION QUI VERIFIE TOUT LES CARACTERE A ECHAPPER DIRECtEMENT
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
            b = 0;
            result = malloc(sizeof(char *) * (2));
            if (!result)
                perror("MALLOC RESULT PARSING");
            result[0] = str[i];
            result[1] = '\0';
            i++;
            *token = push_list(*info, *token, result, 5);
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, i, info, env, quote);
            return (0);   
        }
        if (str[i] == '$')
        {
            dollars_sign (str, token, &i, info, env, 0);
            return (0);
        }

        if ((str[i] == '<' && str[i + 1] != '<') || (str[i] == '>' && str[i + 1] != '>'))
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
            utils = 11;
            if (str[i] == ')')
                utils = 12;
            result = malloc(sizeof(char *) * (2));
            if (!result)
                perror("MALLOC RESULT PARSING");
            result[0] = str[i];
            result[1] = '\0';
            i++;
            *token = push_list(*info, *token, result, utils);         
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, i, info, env, quote);
            return (0);
        }

        if (str[i] == '&' && str[i + 1] == '&')
        {
            result = malloc(sizeof(char *) * (3));
            if (!result)
                perror("MALLOC RESULT PARSING");
            result[0] = str[i];
            result[1] = str[i];
            result[2] = '\0';
            i = i + 2;
            *token = push_list(*info, *token, result, 9);;
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, i, info, env, quote);
            return (0);   
        }

        if (str[i] == '|' && str[i + 1] == '|')
        {
            result = malloc(sizeof(char *) * (3));
            if (!result)
                perror("MALLOC RESULT PARSING");
            result[0] = str[i];
            result[1] = str[i];
            result[2] = '\0';
            i = i + 2;
            *token = push_list(*info, *token, result, 10);;
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, i, info, env, quote);
            return (0);   
        }
        
        if (str[i] == '<' && str[i + 1] == '<')
        {
            result = malloc(sizeof(char *) * (3));
            if (!result)
                perror("MALLOC RESULT PARSING");
            result[0] = str[i];
            result[1] = str[i];
            result[2] = '\0';
            i = i + 2;
            *token = push_list(*info, *token, result, 14);;
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, i, info, env, quote);
            return (0);   
        }

        if (str[i] == '>' && str[i + 1] == '>')
        {
            result = malloc(sizeof(char *) * (3));
            if (!result)
                perror("MALLOC RESULT PARSING");
            result[0] = str[i];
            result[1] = str[i];
            result[2] = '\0';
            i = i + 2;
            *token = push_list(*info, *token, result, 15);;
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, i, info, env, quote);
            return (0);   
        }

        if (str[i] == '*' && str[i + 1] != '*')
        {
            b = 0;
            result = malloc(sizeof(char *) * (2));
            if (!result)
                perror("MALLOC RESULT PARSING");
            result[0] = str[i];
            result[1] = '\0';
            i++;
            *token = push_list(*info, *token, result, 16);
            while (str[i] && str[i] == ' ')
                i++;

            parse_line (str, token, i, info, env, quote);
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

            while (str[i] && str[i] != ' ')
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
                *token = push_list(*info, *token, result, 1);

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


int here_doc(t_token *token)
{
    
    int     pid;
    char    *str;
    int     fd;
    int     *test;
    struct stat    *stat;

    // if access int fd ... eviter d'avoir un fichier ouvert

    fd = open("./temp",  O_TRUNC | O_CREAT | O_RDWR, 0000644);

    str = readline("> ");
    while (strcmp(str, token->next->str))
    {
        write(fd, str, strlen(str));
        write(fd, "\n", 1);
        str = readline("> ");
    }

    

    return (fd);
}


            
            
            











