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
    TYPE 13 = EXPORT
*/


/*• Afficher un prompt en l’attente d’une nouvelle commande.
• Posséder un historique fonctionnel.
• Chercher et lancer le bon exécutable (en se basant sur la variable d’environnement PATH, ou sur un chemin relatif ou absolu).
• Ne pas utiliser plus d’une variable globale. Réfléchissez-y car vous devrez jus- tifier son utilisation.
• Ne pas interpréter de quotes (guillemets) non fermés ou de caractères spéciaux non demandés dans le sujet, tels que \ (le backslash) ou ; (le point-virgule).
• Gérer ’ (single quote) qui doit empêcher le shell d’interpréter les méta-caractères présents dans la séquence entre guillemets.
• Gérer " (double quote) qui doit empêcher le shell d’interpréter les méta-caractères présents dans la séquence entre guillemets sauf le $ (signe dollar).
4
 Minishell Aussi mignon qu’un vrai shell
• Implémenter les redirections :
◦ < doit rediriger l’entrée.
◦ > doit rediriger la sortie.
◦ << doit recevoir un délimiteur et lire l’input donné jusqu’à rencontrer une ligne contenant le délimiteur. Cependant, l’historique n’a pas à être mis à jour !
◦ >> doit rediriger la sortie en mode append.
• Implémenter les pipes (caractère |). La sortie de chaque commande de la pipeline
est connectée à l’entrée de la commande suivante grâce à un pipe.
• Gérer les variables d’environnement (un $ suivi d’une séquence de caractères)
qui doivent être substituées par leur contenu.
• Gérer $? qui doit être substitué par le statut de sortie de la dernière pipeline exécutée au premier plan.
• Gérer ctrl-C, ctrl-D et ctrl-\ qui doivent fonctionner comme dans bash.
• En mode interactif :
◦ ctrl-C affiche un nouveau prompt sur une nouvelle ligne. ◦ ctrl-D quitte le shell.
◦ ctrl-\ ne fait rien.
• Votre shell doit implémenter les builtins suivantes :
◦ echo et l’option -n
◦ cd uniquement avec un chemin relatif ou absolu ◦ pwd sans aucune option
◦ export sans aucune option
◦ unset sans aucune option*/

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

void free_env(char **env)
{
    int i;

    i = 0;

    while (env[i])
    {
        free(env[i]);
        i = i - 1;
    }
}

int ft_grammar(list **token)
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

    }
    return (1);
}





/*value *set_extension(char *str, value *extension)
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
}*/



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
    return (0);
}

db_list *init_liste (db_list *info)
{
    info = malloc(sizeof(db_list));
    if (!info)
        perror ("MALLOC CREATE LIST");
    info->first = NULL;
    info->last = NULL;
    info->lenght = 0;

    return (info);
}


char **export_env(list *token, char **env)
{
    int i; 

    i = 0;
    char **new_element;
    while (env[i])
        i++;
    new_element = malloc(sizeof(char **) * i + 1);
    if (!new_element)
        perror("MALLOC EXPORT");
    i = 0;
    while (env[i])
    {
        new_element[i] = strdup(env[i]);
        i++;
    }
    new_element[i] = strdup(token->str);
    new_element[i + 1] = 0x00;
    return (new_element);
}





int build_in (list  *token, char ***env)
{
    int i;

    i = 0;
    if (!strcmp(token->str, "exit"))
        return (1);
    if (!strcmp(token->str, "env"))                                         //affiche env;
    {
        while (env[0][i])
            printf("%s\n", env[0][i++]);
        return (2);
    }
    if (!strcmp(token->str, "export") && token->next && token->next->type == 13)
    {
        *env = export_env(token->next, *env);                                     //env
    }                 
    return (0);
}







int main (int argc, char **argv, char **env)
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


        

        
        
        while (token)
        {
            if (!(ft_grammar(&token)))          
                break;

            add_history(str);                       //historique

            if (build_in(token, &env) == 2)          //buildin
                break;


            printf("token = %s\ntype = %d\n\n", token->str, token->type);
            token = token->next;
        }









        free_list(token);
        info = init_liste(info);                //je free la structure de controle de la liste
        free(str);
    }

}


            //FAIRE ATTENTION   - AU STRNCMP
         //                     - AU STRDUP