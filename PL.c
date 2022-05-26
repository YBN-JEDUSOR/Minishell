#include "MH.h"

char *ft_34(t_minishell *minishell, int *i)   //extension a rajouter
{
    char *result;
    
    result = malloc(sizeof(char *) * (1));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = '\0';

    if (minishell->line[(*i)] == 34)
    {
        (*i)++;
        while (minishell->line[(*i)] && minishell->line[(*i)] != 34)
        {
            if (minishell->line[(*i)] == '$')
                result = ft_strjoin(result, dollars_sign(minishell, i, 1), 1);
            else
                result = ft_strjoin(result, ft_substr(minishell->line, (*i), 1), 1);
            (*i)++;
        }
        (*i)++;
    }
    return (result);
}

char *ft_39(t_minishell *minishell, int *i)   //extension a rajouter
{
    char *result;
    
    result = malloc(sizeof(char *) * (1));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = '\0';

    if (minishell->line[(*i)] == 39)
    {
        (*i)++;
        while (minishell->line[(*i)] && minishell->line[(*i)] != 39)
        {
            result = ft_strjoin(result, ft_substr(minishell->line, (*i), 1), 1);
            (*i)++;
        }
        (*i)++;
    }
    return (result);
}


int not_token(char c)
{
    if (c == ' ' || c == '|' || c == '<' || c == '(' || c == ')' || c == '&' || c == '>')
        return (0);
    return (1);
}


char *dollars_sign(t_minishell *minishell, int *i, int quote)
{
    char *result;
    int inside;

    inside = 0;
    result = malloc(sizeof(char *) * (1));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = '\0';


    (*i)++;

    if (!minishell->line[(*i)])
        return ("\0");
            
    while (minishell->line[(*i)] && not_token(minishell->line[(*i)]))
    {
        if (minishell->line[*i] == 39)
        {
            result = ft_strjoin(result, ft_39(minishell, i), 1);
            inside = 1;
            break;
        }
        if (minishell->line[*i] == 34)
        {
            result = ft_strjoin(result, ft_34(minishell, i), 1);
            inside = 1;
            break;
        }
        result = ft_strjoin(result, ft_substr(minishell->line, (*i), 1), 1);
        (*i)++;
    }

    if (minishell->line[(*i)] == 34)
        (*i)--;
    
    if (quote == 1)
        return (check_extension(result, minishell));

    if (inside == 0)
        minishell->token = push_list(minishell->info, minishell->token, check_extension(result, minishell), 1);
    if (inside == 1)
        minishell->token = push_list(minishell->info, minishell->token, result, 1);

    while (minishell->line[(*i)] && minishell->line[(*i)] == ' ')
        (*i)++;

    parse_line (minishell, (*i));

    return (0);
}

int simple_pipe(t_minishell *minishell, int *i)
{
    char *result;

    result = malloc(sizeof(char *) * (2));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = minishell->line[*i];
    result[1] = '\0';
    (*i)++;
    minishell->token = push_list(minishell->info,  minishell->token, result, 5);
    while (minishell->line[*i] && minishell->line[*i] == ' ')
        (*i)++;
    parse_line (minishell, (*i));
    return (1);
}

int parenthese(t_minishell *minishell, int *i)
{
    int utils;
    char *result;

    utils = 11;
    if (minishell->line[*i] == ')')
        utils = 12;
    result = malloc(sizeof(char *) * (2));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = minishell->line[*i];
    result[1] = '\0';
    (*i)++;
    minishell->token = push_list(minishell->info, minishell->token, result, utils);         
    while (minishell->line[*i] && minishell->line[*i] == ' ')
        (*i)++;
    parse_line (minishell, (*i));
    return (1);
}

int double_operator (t_minishell *minishell, int *i, int type)
{
    char *result;

    result = malloc(sizeof(char *) * (3));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = minishell->line[*i];
    result[1] = minishell->line[*i];
    result[2] = '\0';
    (*i) = (*i) + 2;
    minishell->token = push_list(minishell->info,  minishell->token, result, type);
    while (minishell->line[*i] && minishell->line[*i] == ' ')
        (*i)++;
    parse_line (minishell, (*i));
    return (1);
}


int redirection (t_minishell *minishell, int *i, int type)
{
    char *result;

    result = malloc(sizeof(char *) * 1);
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = '\0';
    
    (*i) = (*i) + 2;
    
    while (minishell->line[*i] && minishell->line[*i] == ' ')
        (*i)++;

    while (minishell->line[(*i)] && not_token(minishell->line[(*i)]))
    {
        if (minishell->line[*i] == 39)
            result = ft_strjoin(result, ft_34(minishell, i), 1);
        if (minishell->line[*i] == 34)
            result = ft_strjoin(result, ft_34(minishell, i), 1);
        result = ft_strjoin(result, ft_substr(minishell->line, (*i), 1), 1);
        (*i)++;
    }

    minishell->token = push_list(minishell->info,  minishell->token, result, type);

    parse_line (minishell, (*i));

    return (1);
}



int is_infile_or_outfile (t_minishell *minishell, int *i)
{
    char *result;
    int utils;

    utils = 7;
    if (minishell->line[(*i)] == '>')
        utils = 8;
    
    result = malloc(sizeof(char *) * (1));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = '\0';

    (*i)++;
    
    while (minishell->line[(*i)] && minishell->line[(*i)] == ' ')
        (*i)++;

    
    while (minishell->line[(*i)] && not_token(minishell->line[(*i)]))
    {
        if (minishell->line[*i] == 39)
            result = ft_strjoin(result, ft_34(minishell, i), 1);
        if (minishell->line[*i] == 34)
            result = ft_strjoin(result, ft_34(minishell, i), 1);
        result = ft_strjoin(result, ft_substr(minishell->line, (*i), 1), 1);
        (*i)++;
    }

    if (!result[0])
        printf("bash: syntax error near unexpected token `newline'\n");

    
    minishell->token = push_list(minishell->info, minishell->token, result, utils);

    while (minishell->line[(*i)] && minishell->line[(*i)] == ' ')
        (*i)++;

    parse_line (minishell, (*i));

    return (1);
}

int is_wildcard(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '*')
            return (16);
        if (str[i] == '=')
            return (13);
        i++;
    }
    return (1);
}

int regularstr(t_minishell *minishell, int *i)
{
    char *result;
    char *spqr;
    int utils = 0;
    

    result = malloc(sizeof(char) * (1));
    if (!result)
        perror("MALLOC RESULT PARSING");
    result[0] = '\0';



    while (minishell->line[(*i)] && minishell->line[(*i)] != ' ')
    {
        if (minishell->line[(*i)] == 34)
        {
            spqr = ft_34(minishell, i);
            result = ft_strjoin(result, spqr, 1);
            free(spqr);
        }
        if (minishell->line[(*i)] == 39)
         {
            spqr = ft_39(minishell, i);
            result = ft_strjoin(result, spqr, 1);
            free(spqr);
        }
        if (!not_token(minishell->line[(*i)]))
            break;

        if (minishell->line[(*i)] == '=')                                                                 //Extension de variable token contenant "=";
                utils = 1;
        
        spqr = ft_substr(minishell->line, (*i), 1);
        result = ft_strjoin(result, spqr, 1);

        (*i)++;
    }


    if (utils == 0)
        minishell->token = push_list(minishell->info, minishell->token, result, is_wildcard(result));    //Token classique 

    if (utils > 0)
        minishell->token = push_list(minishell->info, minishell->token, result, 13);                      //Extansion de variable token contenant "=";


    while (minishell->line[(*i)] && minishell->line[(*i)] == ' ')
    {
        (*i)++;
    }
    parse_line (minishell, (*i));
    return (1);
}


int parse_line (t_minishell *minishell, int i)
{
    while (minishell->line[i])
    {   
        if (minishell->quote == 0)
        {
            if (minishell->line[i] == '|' && minishell->line[i + 1] != '|')
                if (simple_pipe(minishell, &i))
                    return (1);

            if (minishell->line[i] == '$')
                if (!dollars_sign (minishell, &i, 0))
                    return (1);

            if ((minishell->line[i] == '<' && minishell->line[i + 1] != '<') || (minishell->line[i] == '>' && minishell->line[i + 1] != '>'))
                if (is_infile_or_outfile (minishell, &i))
                    return (1);

            if (minishell->line[i] == '(' || minishell->line[i] == ')')
                if (parenthese(minishell, &i))
                    return (1);

            if (minishell->line[i] == '&' && minishell->line[i + 1] == '&')
                if (double_operator (minishell, &i, 9))
                    return (1);   

            if (minishell->line[i] == '|' && minishell->line[i + 1] == '|')
                if (double_operator (minishell, &i, 10))
                    return (1);   

            if (minishell->line[i] == '<' && minishell->line[i + 1] == '<')
                if (redirection (minishell, &i, 14))
                    return (1);   

            if (minishell->line[i] == '>' && minishell->line[i + 1] == '>')
                if (redirection (minishell, &i, 15))
                    return (1);
        }
        if (minishell->line[i])
            if (regularstr(minishell, &i))
                return (1);
    }
    return (1);
}


t_token **here_doc(t_token *token, t_token *start)
{
    int         size;
    int         i;
    t_token     **here_doc;

    size = 0;
    i = 0;

    while (token)                 
    {
        if (token && token->type == 14 && token->next && token->next->type == 1)
            size++;
        token = token->previous;
    }

    if (size > 0)
    {
        here_doc = malloc(sizeof(t_token*) * size + 1);
        if (!here_doc)
            return (0);
        here_doc[size] = '\0';

        token = start;

        while (token)                 
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
    return (0);
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
        result = push_list(info, result, ft_strjoin(str, "\n", 1), 1);
        str = readline("> ");
    }
    while (result && result->previous)                 
        result = result->previous;
    return (result);
}

/*void print_here_doc(t_token **token)  //Sert a rien a part verifier le bon fonctionement du here doc
{
    int i;

    i = 0;
    while (token[i])
    {
        while (token[i])
        {
            printf("%s",token[i]->str);
            token[i] = token[i]->next;
        }
        i++;
    }
}*/
