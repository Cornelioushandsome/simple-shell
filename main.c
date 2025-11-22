#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "builtin.c"

#define LSH_RL_BUFSIZ 1024

#define LSH_TOK_BUFSIZ 64
#define LSH_TOK_DELIM " \t\n\r\a"

char *lsh_readline(void){
  int bufsize = LSH_RL_BUFSIZ;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  if (!buffer){
    fprintf(stderr, "lsh_readline: initialization allocation failed\n");
    exit(EXIT_FAILURE);
  }
  int c;
  while((c = getchar()) != EOF && c != '\n'){
    buffer[position++] = c;
    if (position >= bufsize){
      bufsize += LSH_RL_BUFSIZ;
      buffer = realloc(buffer, bufsize);
      if (!buffer){
        fprintf(stderr, "lsh_readline: reallocation failed\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  buffer[position] = '\0';

  //ugly
  if (c == EOF){
    putchar('\n');
    exit(EXIT_SUCCESS);
  }

  return buffer;
}

char **lsh_split_line(char* line){
  int bufsize = LSH_TOK_BUFSIZ;
  int position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *curToken;

  if (!tokens){
    fprintf(stderr, "lsh_split_line: initialization allocation error\n");
    exit(EXIT_FAILURE);
  }

  curToken = strtok(line, LSH_TOK_DELIM);
  while(curToken != NULL){
    tokens[position++] = curToken;

    if (position >= bufsize){
      bufsize += LSH_TOK_BUFSIZ;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens){
        fprintf(stderr, "lsh_split_line: reallocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    curToken = strtok(NULL, LSH_TOK_DELIM);
  }

  tokens[position] = NULL;
  return tokens;
  

}

int lsh_launch(char** args){
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0){
    //child process
    if (execvp(args[0], args) == -1){
      perror("lsh");
    }

    exit(EXIT_FAILURE);
  } else if (pid < 0){
    //error forking
    perror("lsh");
    exit(EXIT_FAILURE);
  } else{
    //parent process
    do{
      wpid = waitpid(pid, &status, WUNTRACED);
    } while(!WIFEXITED(status) && !WIFSIGNALED(status));


  }

  return 1;
}

int lsh_execute(char **args){
  int i;
  if (args[0] == NULL){
    return 1;
  }

  for (i = 0; i < lsh_num_builtin(); i++){
    if (strcmp(args[0], builtin_str[i]) == 0){
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}

void lsh_loop(void){
  char *line;
  char **args;
  int status = 1;

  do{
    printf("> ");
    line = lsh_readline();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    //printing
    

    free(line);
    free(args);
    
  }while(status);
}

int main(int argc, char **argv){
  (void) argc;
  (void) argv;
  lsh_loop();

  return EXIT_SUCCESS;
}