/*
 * Filename: cmd.c
 * Author: Akira
 * Description: get and parse command line from stdin
 */

#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* delimiters = "|><";

//if stdin is not empty, return a char pointer to a string without '\0'
char *get_cmd(char *cmd_buffer, int buffer_size) {
    if (fgets(cmd_buffer, buffer_size, stdin) != NULL) {
        size_t len = strlen(cmd_buffer);
        if (len > 0 && cmd_buffer[len - 1] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            fprintf(stderr, "Warning: Input exceeded buffer size and was truncated.\n");
        }
        else if (len > 0 && cmd_buffer[len - 1] == '\n') {
            cmd_buffer[len - 1] = '\0';
        }
        return cmd_buffer;
    }
    return NULL;
}

char** char_to_cmd(char* input) {
    size_t capacity = 10;
    size_t count = 0;
    char** buffer = malloc(capacity * sizeof(char*));

    if (buffer == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(input, " ");
    while (token != NULL) {
        if (count >= capacity) {
            capacity *= 2;
            buffer = realloc(buffer, capacity * sizeof(char*));
            if (buffer == NULL) {
                perror("realloc failed");
                exit(EXIT_FAILURE);
            }
        }

        buffer[count++] = strdup(token);
        token = strtok(NULL, " ");
    }

    buffer[count] = NULL;
    return buffer;
}

char** copy_args(char** src) {
    if (src == NULL) {
        return NULL;
    }

    size_t count = 0;
    while (src[count] != NULL) {
        count++;
    }

    char** dest = malloc((count + 1) * sizeof(char*));
    if (dest == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < count; i++) {
        dest[i] = strdup(src[i]);
        if (dest[i] == NULL) {
            perror("strdup failed");
            // 清理已分配的内存，防止泄漏
            for (size_t j = 0; j < i; j++) {
                free(dest[j]);
            }
            free(dest);
            exit(EXIT_FAILURE);
        }
    }

    dest[count] = NULL;
    return dest;
}

Command *parse_cmd(char *cmd_buffer) 
{
    Command *head = NULL;
    Command *current = NULL;
    size_t start = 0;
    size_t char_poiter = 0;

    while (char_poiter < strlen(cmd_buffer)) {
        if(strchr(delimiters, cmd_buffer[char_poiter])) {
            Command *cmd = malloc(sizeof(Command));
            char* tmp = malloc(sizeof(char)*(char_poiter - start +1));
            switch (cmd_buffer[char_poiter]) {
                case '|': {
                    memcpy(tmp, &cmd_buffer[start], char_poiter-start);
                    char **cmd_args = char_to_cmd(tmp);
                    cmd->program = strdup(cmd_args[0]);
                    cmd->args = copy_args(cmd_args);
                    free(tmp);
                    cmd->operate = PIPE;
                    cmd->next = NULL;
                } break;
                case '>': {
                    memcpy(tmp, &cmd_buffer[start], char_poiter-start);
                    char **cmd_args = char_to_cmd(tmp);
                    cmd->program = strdup(cmd_args[0]);
                    cmd->args = copy_args(cmd_args);
                    free(tmp);
                    cmd->operate = O_REDIRECT;
                    cmd->next = NULL;
                } break;
                case '<': {
                    memcpy(tmp, &cmd_buffer[start], char_poiter-start);
                    char **cmd_args = char_to_cmd(tmp);
                    cmd->program = strdup(cmd_args[0]);
                    cmd->args = copy_args(cmd_args);
                    free(tmp);
                    cmd->operate = I_REDIRECT;
                    cmd->next = NULL;
                } break;
            }
            if(head == NULL) {
                head = cmd;
                current = head;
            }
            else {
                current->next = cmd;
            }
            current = cmd;
            start = char_poiter + 1;
        }
        char_poiter++;
    }
    if (start < char_poiter) {
        size_t length = char_poiter - start;
        char *tmp = malloc(length + 1);
        if (tmp == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        memcpy(tmp, &cmd_buffer[start], length);
        tmp[length] = '\0'; // 添加字符串终止符

        // 解析命令字符串
        char **cmd_args = char_to_cmd(tmp);
        free(tmp);

        // 创建新节点
        Command *cmd = malloc(sizeof(Command));
        if (cmd == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        cmd->args = cmd_args;
        cmd->operate = NONE;
        cmd->next = NULL;

        // 链接到链表中
        if (head == NULL) {
            head = cmd;
        } else {
            current->next = cmd;
        }
    }

    return head;
}

void free_buffer(char *cmd_buffer, Command *current_cmd) 
{
    free(cmd_buffer);
    free(current_cmd);
}
