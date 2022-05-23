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
#include <dirent.h>

/*
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
    file = ft_strdup("/usr/bin/ls");
    args = malloc(sizeof(char *) * (3));
    args[0] = file;
    args[1] = "*";
    args[2] = NULL;
    
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
*/

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
  //printf("%s\n", wildcard);
  //printf("%d\n", *index);
  //printf("*index20: %d\n", *index2);
  //printf("wildcard[*index2]0: %c\n", wildcard[*index2]);  
  if (wildcard[*index2] == '*')
      (*index2)++;
  //printf("*index21: %d\n", *index2);
  //printf("wildcard[*index2]1: %c\n", wildcard[*index2]);
  while (wildcard[*index2])
  {
    //printf("wildcard[*index2]2: %c\n", wildcard[*index2]);
    if (wildcard[*index2] == '*')
      break ;
    (*index2)++;
    //printf("*index22: %d\n", *index2);
  }
  //printf("wildcard[*index2]3: %c\n", wildcard[*index2]);
  //wildcard[*index2 + 1] = 0;
  //printf("chunk: %s\n", wildcard);
  //printf("\n");
  //*index1 = *index2;
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}
/*
void  extract_pattern(char **chunk, int chunk_type)
{
  char  *new_chunk;

  if (chunk_type == 2)
    {
      new_chunk = malloc(sizeof(char) * (ft_strlen(*chunk) - 2 + 1));
      ft_memcpy(new_chunk, *chunk + 1, ft_strlen(*chunk) - 2);
      new_chunk[ft_strlen(*chunk) - 2 + 1] = '0';
    }
    if (chunk_type == 1)
    {
      if ((*chunk)[0] != '*')
      {
        new_chunk = malloc(sizeof(char) * (ft_strlen(*chunk) - 1 + 1));
        ft_memcpy(new_chunk, *chunk, ft_strlen(*chunk) - 1);
        new_chunk[ft_strlen(*chunk) - 1 + 1] = '0';
        //printf("ici new_chunk: %s\n", new_chunk);
      }
      else
      {
        new_chunk = malloc(sizeof(char) * (ft_strlen(*chunk) - 1 + 1));
        ft_memcpy(new_chunk, *chunk + 1, ft_strlen(*chunk) - 1);
        new_chunk[ft_strlen(*chunk) - 1 + 1] = '0';
        //printf("ici new_chunk: %s\n", new_chunk);
      }
    }
    free(*chunk);
    *chunk = new_chunk;
    //printf("ici chunk: %s\n", *chunk);
}
*/

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
  new_chunk[ft_strlen(*chunk) - size + 1] = '0';
  /*
  if (chunk_type == 2)
    {
      new_chunk = malloc(sizeof(char) * (ft_strlen(*chunk) - 2 + 1));
      ft_memcpy(new_chunk, *chunk + 1, ft_strlen(*chunk) - 2);
      new_chunk[ft_strlen(*chunk) - 2 + 1] = '0';
    }
    if (chunk_type == 1)
    {
      if ((*chunk)[0] != '*')
      {
        new_chunk = malloc(sizeof(char) * (ft_strlen(*chunk) - 1 + 1));
        ft_memcpy(new_chunk, *chunk, ft_strlen(*chunk) - 1);
        new_chunk[ft_strlen(*chunk) - 1 + 1] = '0';
        //printf("ici new_chunk: %s\n", new_chunk);
      }
      else
      {
        new_chunk = malloc(sizeof(char) * (ft_strlen(*chunk) - 1 + 1));
        ft_memcpy(new_chunk, *chunk + 1, ft_strlen(*chunk) - 1);
        new_chunk[ft_strlen(*chunk) - 1 + 1] = '0';
        //printf("ici new_chunk: %s\n", new_chunk);
      }
    }
    */

    free(*chunk);
    *chunk = new_chunk;
    //printf("ici chunk: %s\n", *chunk);
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

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		if (s1[i] == '\0' || s2[i] == '\0')
			return (0);
		i++;
	}	
	return (0);
}

int check_match_type_0(char **file_name, char *chunk)
{
  //printf("file_name: %s\n", *file_name);
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
  //printf("file_name: %s\n", *file_name);
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
  //printf("file_name: %s\n", *file_name);
  //printf("chunk: %s\n", chunk);
  while ((*file_name)[i])
  {
    //printf("(*file_name)[i]: %c\n", (*file_name)[i]);
    if (!strncmp(*file_name + i, chunk, ft_strlen(chunk)))
    {
      i += ft_strlen(chunk);
      *file_name += i;
      return (0);
    }
    i++;
    //*file_name += i;
  }
  return (1);
}

int  match_pattern(char **file_name, char *chunk, int chunk_type)
{
  int match;

  //*file_name = *file_name + 1;
  
  //printf("filename: %s\n", *file_name);
  if (chunk_type == 0)
  {
    //printf("OKAY0\n");
    match = check_match_type_0(file_name, chunk);
  }
  if (chunk_type == 1)
  {
    //printf("OKAY1\n");
    match = check_match_type_1(file_name, chunk);
  }
    
  if (chunk_type == 2)
  {
    //printf("OKAY2\n");
    match = check_match_type_2(file_name, chunk);
  }
    
  //printf("match: %d\n", match);
  if (match)
    return (1);
  
  //printf("file_name: %s\n", file_name);
  return (0);
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

/*
int wildcard_match(char *file_name, char *wildcard)
{
  char  *chunk;
  char  *str;
  int chunk_type;
  int file_match;
  int i;
  int j;

  file_match = 1;
  i = 0;
  j = 0;
  str = ft_strdup(file_name);
  //printf("str: %s\n", str);
  while (wildcard[i])
  {
    //printf("OKAY\n");
    determine_chunk(wildcard, &i, &j);
    chunk = malloc(sizeof(char) * (j + 1 - i + 1));
    //printf("wildcard: %s\n", wildcard);
    //printf("j - i: %d\n", j - i);
    ft_memcpy(chunk, wildcard + i, j + 1 - i);
    chunk[j + 1 - i + 1] = '0';
    //printf("chunk0: %s\n", chunk);
    determine_chunk_type(chunk, &chunk_type);
    //printf("chunk_type: %d\n", chunk_type);
    extract_pattern(&chunk, chunk_type);
    //printf("chunk: %s\n", chunk);
    //printf("file_name0: %s\n", file_name);
    //match_pattern(&file_name, chunk, chunk_type);
    //printf("file_name1: %s\n", file_name);
    //printf("")
    
    if (match_pattern(&file_name, chunk, chunk_type))
    {
      file_match = 0;
      break ;
    }

    if (!wildcard[j + 1])
      break ;
    i = j;
    

    
  }
  
  if (file_match)
  {
    //printf("file_name: %s\n", str);
    return (0);
  }
  
}
*/

int wildcard_match(char *file_name, char *wildcard)
{
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
    chunk[j + 1 - i + 1] = '0';
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

int main(void)
{
  DIR *folder;
  struct dirent *entry;
  int files;
  int read;
  char  wildcard[] = "*s*.txt";
  //char  wildcard[] = "test.*";
  //char  wildcard[] = "*test*";
  entry = NULL;
  files = 0;
  read = 1;
  folder = opendir(".");
  if (folder == NULL)
  {
    perror("Unable to read directory");
    return (1);
  }
  while (read)
  {
    entry = readdir(folder);
    if (!entry)
      read = 0;
    else
    {
      //printf("File: %s\n", entry->d_name);
      if (!wildcard_match(entry->d_name, wildcard))
      {
        files++;
        printf("entry->d_name: %s\n", entry->d_name);
      }
      
    }
  }
  closedir(folder);
  //printf("files: %d\n", files);
  return (0);
}
