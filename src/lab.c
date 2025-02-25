#include "lab.h"
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

/**
 * Task 2 - Print Version
 * This function is called with a cmd-line argument "v".
 * It will output the current version of the shell, and exit the program.
 */
void print_vers(){
    printf("Justin Shell version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
    exit(0);
}

/**
 * Task 4 - Get Prompt
 * This function will return the prompt to be displayed in the shell.
 * If the environment variable is not set, the default prompt will be used.
 * @param env - the environment variable
 * @return - the prompt to be displayed
 */
char *get_prompt(const char *env){
    //get the environment variable
    char *prompt = getenv(env);

    if (prompt != NULL) {
        //prompt exists so return it
        return strdup(prompt);     
    } else {
          //no prompt so return default
          return strdup("justinshell>");
    }
}

int change_dir(char **dir){
    // Stub implementation
    UNUSED(dir);
    return 0;
}

char **cmd_parse(char const *line){
    // Stub implementation
    UNUSED(line);
    return NULL;
}

/**
 * Task 3 - Free Command
 * This function will free the memory allocated for command arguments.access
 * @param line - the command arguments
 */
void cmd_free(char **line){
    if (line == NULL){
        return;
    }

    // Free each argument
    for (size_t i = 0; line[i] != NULL; i++){
        free(line[i]);
    }

    free(line);
}

/**
 * Task 3 - Trim white space
 * This function will remove white space from a string
 * This helps to ensure that the command is correctly parsed
 * @param line - the command arguments
 * @return - the command arguments with white space removed
 */
char *trim_white(char *line){
    if (line == NULL){
        return;
    }

    // If line is empty, return
    if (*line == '\0'){
        return line;
    }

    // Trim the leading white space
    while (isspace((unsigned char)*line)){
        line++;
    }

    // Trim the trailing whitespace
    char *end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) {
        end--;
    }
    
    // Null-terminate the string directly after the last non-whitespace character
    *(end + 1) = '\0';
    
    return line;
}

bool do_builtin(struct shell *sh, char **argv){
    // Stub implementation
    UNUSED(sh);
    UNUSED(argv);
    return false;
}

void sh_init(struct shell *sh){
    // Stub implementation
    UNUSED(sh);
}

void sh_destroy(struct shell *sh){
    // Stub implementation
    UNUSED(sh);
}

/**
 * Task 2 & 4 - Parse Arguments
 * This function will parse the command line arguments
 * @param argc - the number of arguments
 * @param argv - the arguments
 */
void parse_args(int argc, char **argv){
   int opt;

   //use getop to handle the command line arguments
   //Wow copilot wrote everything but the default case correctly
    while ((opt = getopt(argc, argv, "v")) != -1){
         switch (opt){
              case 'v':
                print_vers();
                break;
              default:
                  // usage message for invalid arguments
                  fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
                  exit(EXIT_FAILURE);
         }
    }
}