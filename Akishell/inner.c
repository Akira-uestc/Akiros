/*
 * Filename: inner.c
 * Author: Akira
 * Description: shell built-in command
 */

#include "cmd.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

extern int if_loop;

const char* builtin_commands[] = {
    "exit",
    "cd",
    "help",
    "exec",
    NULL
};

void builtin_exit(char* cmd_buffer, Command* current_cmd);
void builtin_cd(char* cmd_buffer, Command* current_cmd);
void builtin_help(char* cmd_buffer, Command* current_cmd);
void builtin_exec(char* cmd_buffer, Command* current_cmd);

typedef void (*builtin_func)(char*, Command*);

struct {
    const char* name;
    builtin_func func;
} builtin_table[] = {
    {"exit", builtin_exit},
    {"cd", builtin_cd},
    {"help", builtin_help},
    {"exec",builtin_exec},
    {NULL, NULL}
};

bool is_builtin(Command* cmd, char* cmd_buffer) 
{
    for (int i = 0; builtin_table[i].name != NULL; i++) 
    {
        if (strcmp(cmd->program, builtin_table[i].name) == 0) 
        {
            builtin_table[i].func(cmd_buffer, cmd);
            return true;
        }
    }
    return false;
}

void builtin_exit(char* cmd_buffer, Command* current_cmd)
{
    free_buffer(cmd_buffer, current_cmd);
    if_loop = 0;
    printf("Exiting...\n");
}

void builtin_cd(char* cmd_buffer, Command* current_cmd) 
{
    char* path = *(current_cmd->args+1);

    if (path == NULL || strcmp(path, "~") == 0) {
        path = getenv("HOME");
        if (path == NULL) {
            printf("cd: HOME environment variable not set.\n");
            return;
        }
    }

    if (chdir(path) == 0) {
        char cwd[256];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Changed directory to: %s\n", cwd);
        } else {
            perror("getcwd() error");
        }
    } else {
        printf("failed to change directory\n");
    }
    free_buffer(cmd_buffer, current_cmd);
}

void builtin_help(char* cmd_buffer, Command* current_cmd) 
{
    printf("Help: Available commands are exit, cd, help, echo\n");
    free_buffer(cmd_buffer, current_cmd);
}

void builtin_exec(char* cmd_buffer, Command* current_cmd)
{
    return;
}
