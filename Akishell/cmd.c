/*
 * Filename: cmd.c
 * Author: Akira
 * Description: get and parse command line from stdin
 */

#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_cmd(char *cmd_buffer, int buffer_size) 
{
    if (fgets(cmd_buffer, buffer_size, stdin) != NULL) 
    {
        size_t len = strlen(cmd_buffer);
        if (len > 0 && cmd_buffer[len - 1] != '\n') 
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            fprintf(stderr, "Warning: Input exceeded buffer size and was truncated.\n");
        }
        else if (len > 0 && cmd_buffer[len - 1] == '\n') 
        {
            cmd_buffer[len - 1] = '\0';
        }
        return cmd_buffer;
    }
    return NULL;
}

Command *parse_cmd(char *cmd_buffer) 
{
    Command *head = NULL;
    Command *current = NULL;
    char *token = strtok(cmd_buffer, "| ");

    while (token != NULL) 
    {
        Command *cmd = malloc(sizeof(Command));
        cmd->next = NULL;

        char *prog_token = strtok(token, " ");
        cmd->program = strdup(prog_token);

        int arg_count = 0;
        char *arg_list[100];
        while (prog_token != NULL) {
            arg_list[arg_count++] = strdup(prog_token);
            prog_token = strtok(NULL, " ");
        }
        arg_list[arg_count] = NULL;
        cmd->args = malloc(sizeof(char *) * (arg_count + 1));
        memcpy(cmd->args, arg_list, sizeof(char *) * (arg_count + 1));

        if (head == NULL) 
        {
            head = cmd;
        }
        else
        {
            current->next = cmd;
        }
        current = cmd;

        token = strtok(NULL, "|");
    }

    return head;
}

void free_buffer(char *cmd_buffer, Command *current_cmd) 
{
    free(cmd_buffer);
    free(current_cmd);
}
