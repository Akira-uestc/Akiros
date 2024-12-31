/*
 * Filename: cmd.h
 * Author: Your Name
 * Description:
 */

#ifndef CMD_H
#define CMD_H

enum operate {
    PIPE,
    REDIRECT,
    NONE
};

typedef struct Command {
    char* program;
    char** args;
    enum operate operate;
    struct Command *next;
} Command;

typedef struct seperate_char {
    char* cmd;
    enum operate operate;
} seperate_char;

char *get_cmd(char *cmd_buffer, int buffer_size);
Command *parse_cmd(char *cmd_buffer);
void free_buffer(char *cmd_buffer, Command *current_cmd);

#endif /* CMD_H */
