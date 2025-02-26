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
    //WOW Copilot wrote this whole function :) except exit 0
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
    return strdup(prompt ? prompt : "shell>"); // Directly return strdup
}

/**
 * Task 5 - Change Directory
 * This function will change the current working directory
 * @param dir - the directory to change to
 * @return - 0 if successful, -1 otherwise
 */
int change_dir(char **dir){
    char *path = NULL;
    
    // This handles our home directory
    if (dir == NULL || dir[1] == NULL) {
        path = getenv("HOME");
        if (!path) {
            struct passwd *pw = getpwuid(getuid());
            if (pw) {
                path = pw->pw_dir;
            } else {
                fprintf(stderr, "cd: Cannot find home directory\n");
                return -1;
            }
        }
    } else {
        path = dir[1];
    }
    
    // Change directory and handle errors
    if (chdir(path) != 0) {
        perror("cd");
        return -1;
    }
    
    return 0;
}

/**
 * Task 3,4,5 - Parse Command
 * This function will parse the command arguments
 * @param line - the command arguments
 * @return - the parsed command arguments
 */
char **cmd_parse(char const *line){
   if (!line){
       return NULL;
   }

    // Get maximum number of arguments
    long arg_max = sysconf(_SC_ARG_MAX);

    // Allocate space for arguments (plus one for NULL terminator)
    char **argv = malloc(sizeof(char*) * arg_max);
    if (!argv) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    char *line_copy = strdup(line);
    if (!line_copy) {
        perror("strdup failed");
        free(argv);
        return NULL;
    }

    // Parse the line
    int argc = 0;
    char *token = strtok(line_copy, " \t\n");

    // Process each token
    while (token != NULL && argc < arg_max - 1) {
        argv[argc] = strdup(token);
        if (argv[argc] == NULL) {
            perror("Memory allocation failed");
            // Free allocated memory
            for (int i = 0; i < argc; i++) {
                free(argv[i]);
            }
            free(argv);
            free(line_copy);
            return NULL;
        }
        argc++;
        token = strtok(NULL, " \t\n");
    }
    
    // null terminate array
    argv[argc] = NULL;
    
    // Free the copy
    free(line_copy);
    
    return argv;
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
 * 
 * I was written with copilot's help
 */
char *trim_white(char *line){
    if (line == NULL){
        return NULL;
    }

    // Trim leading whitespace
    char *start = line;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    // If string is all whitespace, make it an empty string
    if (*start == '\0') {
        *line = '\0';
        return line;
    }
    
    // Move the trimmed string to the beginning of the original buffer
    if (start != line) {
        size_t len = strlen(start);
        memmove(line, start, len + 1);  // +1 to include the null terminator
    }
    
    // Trim trailing whitespace
    char *end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) {
        *end-- = '\0';
    }
    
    return line;
}

/**
 * Task 3,4,5...- Execute Command
 * This function will execute the command
 * @param sh - the shell
 * @param argv - the command arguments
 * @return - true if the command is a built-in command, false otherwise
 */
bool do_builtin(struct shell *sh, char **argv){
    if (!argv || !argv[0]){
        return false;
    }

    // watch for exit command
    if (strcmp(argv[0], "exit") == 0){
        // free the memory allocated for the shell
        sh_destroy(sh);
        // exit the program...no return statement as unreachable code
        exit(0);
    }

    // watch for cd command
    if (strcmp(argv[0], "cd") == 0){
        // change directory
        change_dir(argv);
        return true;
    }

    // watch for history command
    if (strcmp(argv[0], "history") == 0){
        // print the history
        HIST_ENTRY **hist_list = history_list();
        if (hist_list){
            for (int i = 0; hist_list[i] != NULL; i++){
                printf("%d: %s\n", i, hist_list[i]->line);
            }
        }
        return true;
    }
    return false;
}

/**
 * Task 4 - Initialize Shell
 * This function will initialize the shell
 * @param sh - the shell
 */
void sh_init(struct shell *sh){
    if (!sh){
        return; // Return early if null pointer
    }

    // Get prompt from environment or use default
    sh->prompt = get_prompt("MY_PROMPT");
    
    // Get terminal information for job control
    sh->shell_terminal = STDIN_FILENO;
    sh->shell_is_interactive = isatty(sh->shell_terminal);
    
    if (sh->shell_is_interactive) {
        // Loop until we are in the foreground
        while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp())) {
            kill(-sh->shell_pgid, SIGTTIN);
        }
        
        // Ignore interactive and job-control signals
        signal(SIGINT, SIG_IGN);   
        signal(SIGQUIT, SIG_IGN);  
        signal(SIGTSTP, SIG_IGN);  
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        
        // Put ourselves in our own process group
        sh->shell_pgid = getpid();
        if (setpgid(sh->shell_pgid, sh->shell_pgid) < 0) {
            perror("Couldn't put the shell in its own process group");
            exit(1);
        }
        
        // Grab control of the terminal
        tcsetpgrp(sh->shell_terminal, sh->shell_pgid);
        
        // Save default terminal attributes
        tcgetattr(sh->shell_terminal, &sh->shell_tmodes);
    }
}

/**
 * Task 4 - Destroy Shell
 * This function will free the memory allocated for the shell
 * @param sh - the shell
 */
void sh_destroy(struct shell *sh){
   if (sh == NULL){
       return;
   }

   // free prompt if used
   if (sh->prompt != NULL){
       free(sh->prompt);
       sh->prompt = NULL;
   }

   // Clear the readline history
    clear_history();
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