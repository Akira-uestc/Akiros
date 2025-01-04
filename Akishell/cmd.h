/*
 * Filename: cmd.h
 * Author: Your Name
 * Description: some struct defination of the command line.
 */

#ifndef CMD_H
#define CMD_H

#include <stdio.h>

typedef enum operate {
    PIPE,
    O_REDIRECT,
    I_REDIRECT,
    NONE
} Operate;

typedef struct Command {
    char* program;
    char** args;
    Operate operate;
    FILE* input_direction;
    FILE* output_direction;
    struct Command *next;
} Command;

char *get_cmd(char *cmd_buffer, int buffer_size);
Command *parse_cmd(char *cmd_buffer);
void free_buffer(char *cmd_buffer, Command *current_cmd);

#endif /* CMD_H */
