/*
 * Filename: cmd.h
 * Author: Your Name
 * Description:
 */

#ifndef CMD_H
#define CMD_H

typedef struct Command {
    char* program;
    char** args;
    char* operate;
    struct Command *next;
} Command;

char *get_cmd(char *cmd_buffer, int buffer_size);
Command *parse_cmd(char *cmd_buffer);
void free_buffer(char *cmd_buffer, Command *current_cmd);

#endif /* CMD_H */
