#include "minishell.h"


// A faire = faire la difference entre double quote et single quote
//         = substitution de commande : une string prend le resultat d'une commande  
//         = E0F 




/* MAN MINISHELL 
    TYPE 1 = WORD
    TYPE 2 = ARGUMENTS
    TYPE 3 = DOUBLE QUOTE
    TYPE 4 = SIMPLE QUOTE
    TYPE 5 = EXTENSION
    TYPE 6 = PIPE
    TYPE 7 = INFILE
    TYPE 8 = OUTFILE
    TYPE 9 = &&
    TYPE 10 = ||
    TYPE 11 = (
    TYPE 12 = )
*/



/*If the end of input is recognized, the current token shall be delimited. If there is no current token, the end-of-input indicator shall be returned as the token.

If the previous character was used as part of an operator and the current character is not quoted and can be used with the current characters to form an operator, it shall be used as part of that (operator) token.

If the previous character was used as part of an operator and the current character cannot be used with the current characters to form an operator, the operator containing the previous character shall be delimited.

If the current character is backslash, single-quote, or double-quote ( '\', '", or ' )' and it is not quoted, it shall affect quoting for subsequent characters up to the end of the quoted text. 
The rules for quoting are as described in Quoting. During token recognition no substitutions shall be actually performed, and the result token shall contain exactly the characters that appear in the input (except for <newline> joining),
unmodified, including any embedded or enclosing quotes or substitution operators, between the quote mark and the end of the quoted text. The token shall not be delimited by the end of the quoted field.

If the current character is an unquoted '$' or '`', the shell shall identify the start of any candidates for parameter expansion ( Parameter Expansion), 
command substitution ( Command Substitution), or arithmetic expansion ( Arithmetic Expansion) from their introductory unquoted character sequences: '$' or "${", "$(" or '`', and "$((", respectively. 
The shell shall read sufficient input to determine the end of the unit to be expanded (as explained in the cited sections). While processing the characters, 
if instances of expansions or quoting are found nested within the substitution, the shell shall recursively process them in the manner specified for the construct that is found. 
The characters found from the beginning of the substitution to its end, allowing for any recursion necessary to recognize embedded constructs, shall be included unmodified in the result token, including any embedded or 
enclosing substitution operators or quotes. The token shall not be delimited by the end of the substitution.

If the current character is not quoted and can be used as the first character of a new operator, the current token (if any) shall be delimited. The current character shall be used as the beginning of the next (operator) token.

If the current character is an unquoted <newline>, the current token shall be delimited.

If the current character is an unquoted <blank>, any token containing the previous character is delimited and the current character shall be discarded.

If the previous character was part of a word, the current character shall be appended to that word.

If the current character is a '#', it and all subsequent characters up to, but excluding, the next <newline> shall be discarded as a comment. The <newline> that ends the line is not considered part of the comment.

The current character is used as the start of a new word.*/

void free_list(list *token)
{
    list *temp;

    while (token)
    {
        temp = token;
        free(temp);
        token = token->previous;
    }
}

int ft_grammar(list **token)
{
    if (*token);
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
        
        if ((*token)->type == 5 && (!(*token)->previous))
        {
                printf("bash: syntax error near unexpected token `|'\n");
                return (0);
        }
    }
    return (1);
}

value *set_extension(char *str, value *extension)
{
    int i;
    int a; 
    int b;
    int c;

    i = 0;
    a = 0;
    b = 0;
    c = 0;
    value *new_element;
    new_element = malloc(sizeof(value));
    if (!new_element)
        perror ("MALLOC CREATE VALUE");
    while (str[i])
    {
        while (str[i] && str[i] != '=')
            i++;
        a = i;
        i++;
        while (str[i])
            i++;
        b = (i - a - 1);
    }
    new_element->ancient = malloc(sizeof(char *) * a + 1);
    new_element->new = malloc(sizeof(char *) * b + 1);
    if (!new_element->ancient || !new_element->new)
        perror ("MALLOC CREATE STRING VALUE");
    while (c < a)
    {
        new_element->ancient[c] = str[c];
        c++;
    }
    c++;
    a = 0;
    while (str[c])
    {
        new_element->new[a] = str[c];  //integrer integration dans integration a=test -> b=ert$a --->>> A FAIRE
        c++;
        a++;
    }
    str[c] = '\0';
    new_element->previous = extension;
    return (new_element);
}

char *check_extension(char *str, value *extension)
{
    int i;

    i = 0;
    while (extension)
    {
        if(!strcmp((const char *)extension->ancient, (const char *)str))
            return(extension->new);
        extension = extension->previous;
    }
    return (str);
}

list *push_empty_list (db_list *info, list *token, char *str, int type)
{
    list *new_element;
    new_element = malloc(sizeof(list));
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


list *push_full_list (db_list *info, list *token, char *str, int type)
{
    list *new_element;
    new_element = malloc(sizeof(list));
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

list *push_list(db_list *info, list *token, char *str, int type)
{
    if (info->lenght == 0)
        return (push_empty_list (info, token, str, type));

    if (info->lenght > 0)
        return (push_full_list(info, token, str, type));
}

db_list *init_liste (db_list *info)
{
    info = malloc(sizeof(db_list));
    if (!info)
        perror ("MALLOC CREATE LIST");
    info->first = NULL;
    info->last = NULL;
    info->lenght = 0;
}

int main ()
{
    char	*str;
    char    *result;
    list    *token;
    db_list  *info;
    value   *extension;

    token = NULL;
    extension = NULL;

    info = init_liste(info);

    while ((str = readline("minishell$ ")))
    {
        parse_line(str, &token, &extension, 0, &info);

        while (token && token->previous)                 //utilise ce while pour te balader;
            token = token->previous;

        if (!strcmp(token->str, "exit"))
        {
            free_list(token);
            info = init_liste(info);                    //fermeture programme
            free(str);
            break;
        }                 

        while (token)
        {
            if (!(ft_grammar(&token)))          
                break;

            printf("token = %s\ntype = %d\n\n", token->str, token->type);
            token = token->next;
        }

        free_list(token);
        info = init_liste(info);                //je free la structure de controle de la liste
        free(str);
    }
}


            //FAIRE ATTENTION AU STRNCMP