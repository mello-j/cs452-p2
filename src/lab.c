#include "lab.h"
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

/**
 * Task 2 - Print Version
 * This function is called with a cmd-line argument "v".
 * It will output the current version of the shell, and exit the program.
 */
void print_vers(){
    printf("Justin Shell version: %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
    exit(0);
}

char *get_prompt(const char *env) {
    // Stub implementation
    UNUSED(env);
    return strdup("shell>");
}

int change_dir(char **dir) {
    // Stub implementation
    UNUSED(dir);
    return 0;
}

char **cmd_parse(char const *line) {
    // Stub implementation
    UNUSED(line);
    return NULL;
}

void cmd_free(char **line) {
    // Stub implementation
    UNUSED(line);
}

char *trim_white(char *line) {
    // Stub implementation
    UNUSED(line);
    return line;
}

bool do_builtin(struct shell *sh, char **argv) {
    // Stub implementation
    UNUSED(sh);
    UNUSED(argv);
    return false;
}

void sh_init(struct shell *sh) {
    // Stub implementation
    UNUSED(sh);
}

void sh_destroy(struct shell *sh) {
    // Stub implementation
    UNUSED(sh);
}

void parse_args(int argc, char **argv) {
    // Stub implementation
    UNUSED(argc);
    UNUSED(argv);
}