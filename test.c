#include "minishell.h"

int main(int argc, char *argv[], char *env[])
{
    char *args[] = {"/usr/bin/ls", "-l", "-a", NULL};
    execve("/usr/bin/ls", args, env);
}