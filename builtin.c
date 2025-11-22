#include <stdio.h>

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit
};

int lsh_num_builtin(){
  return sizeof(builtin_str) / sizeof(sizeof(char*));
}

int lsh_cd(char **args){
  if (args[1] == NULL){
    fprintf(stderr, "lsh: expected argument to: cd");
  } else{
    if (chdir(args[1]) != 0){
      perror("lsh");
    }
  }
  return 1;
}

int lsh_help(char **args){
  (void) args;
  int i;
  printf("The Starter Lsh\tVersion 0.00.1\n");
  printf("types program names and arguments, and then hit enter\n");
  printf("The following are builtin:\n");
  for (i = 0; i<lsh_num_builtin(); i++){
    printf("- %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}
int lsh_exit(char **args){
  exit(0);
  return 0;
}