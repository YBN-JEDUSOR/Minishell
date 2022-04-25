#include "minishell.h"


int parse_line (char *str, list **token, value **extension, int i, db_list **info, char ** env)
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
            parse_line (str, token, extension, i, info, env);
            return (0);   
        }
        if (str[i] == '$')
        {
            b = 0;
            i++;
            a = i;
            
            while (str[i] && str[i] != ' ')
                i++;
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
            *token = push_list(*info, *token, check_extension(result, env), 6);         
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, extension, i, info, env);
            return (0);
        }

        if (str[i] == '<' || str[i] == '>')
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
            parse_line (str, token, extension, i, info, env);
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
            parse_line (str, token, extension, i, info, env);
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
            parse_line (str, token, extension, i, info, env);
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
            parse_line (str, token, extension, i, info, env);
            return (0);   
        }

        if (str[i])
        {
            b = 0;
            a = i;

            while (str[i] && str[i] != ' ')
            {
                if (str[i] == '=')
                    utils = 1;
                if (str[i] == ')')
                    break;
                i++;
            }
            result = malloc(sizeof(char *) * (i - a + 1));
            if (!result)
                perror("MALLOC RESULT PARSING");
            while (a < i)
            {
                if (str[a] == 34 || str[a] == 39)                      //erreur si pas double quote qui referme,
                    a++;
                result[b] = str[a];
                b++;
                a++;
            }
            result[b] = '\0';
            if (utils == 0)
                *token = push_list(*info, *token, result, 1);

            if (utils == 1)
            {
               *token = push_list(*info, *token, result, 13);
                utils = 0;
            }
            while (str[i] && str[i] == ' ')
                i++;
            parse_line (str, token, extension, i, info, env);
            return (0);
        }
    }
    return (1);
}

