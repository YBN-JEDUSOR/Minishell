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


size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
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

int main(int argc, char *argv[], char *env[])
{
  char  *file;
  char  **args;
  int pid;

  pid = fork();
  if (!pid)
  {
    file = ft_strdup("/usr/bin/lss");
    args = malloc(sizeof(char *) * (4));
    args[0] = file;
    args[1] = "-l";
    args[2] = "-a";
    args[3] = NULL;
    execve(args[0], args, env);
    free(args[0]);
    free(args);
    exit(1);
  }
  int *testptr;

  testptr = NULL;
  printf("testptr: %p\n", testptr);

  return (0);
}