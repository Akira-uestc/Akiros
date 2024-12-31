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

//从只包含一个可执行程序的str中解析出Command结构体
Command *parse_cmd(char *cmd_buffer) 
{
    Command *head = NULL;
    Command *current = NULL;
    char *token = strtok(cmd_buffer, "|");

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

char* slice_array(const char* array, int start, int length) {
    char* sub_array = malloc((length + 1) * sizeof(char)); // +1 for null terminator
    if (sub_array == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    strncpy(sub_array, array + start, length);
    sub_array[length] = '\0'; // Ensure null termination

    return sub_array;
}

//将输入的命令字符串 cmd_buffer 按照特定的分隔符解析为一个结构数组
seperate_char** parse_char(char *cmd_buffer) 
{
    seperate_char** sep_char = NULL;

    char cmd_unparsed[1024];
    char tmp[1024];
    int pro_count = 0;
    int buffer_pointer = 0;
    memcpy(cmd_unparsed, cmd_buffer, 1024);

    while(buffer_pointer < 1024)
    {
        if (cmd_unparsed[buffer_pointer] == '|') 
        {
            //不要复制空格到新数组
            memcpy(tmp, cmd_unparsed, buffer_pointer-2);

            sep_char[pro_count]->cmd = strdup(tmp);
            sep_char[pro_count]->operate = PIPE;

            //去除空格
            int remaining_length = strlen(cmd_unparsed) - buffer_pointer - 1;
            char* new_unparsed = slice_array(cmd_unparsed, buffer_pointer + 2, remaining_length-1);
            strcpy(cmd_unparsed, new_unparsed);
            free(new_unparsed);

            buffer_pointer = 0;
            pro_count++;
        }
        else if (cmd_unparsed[buffer_pointer] == '>') 
        {
            memcpy(tmp, cmd_unparsed, buffer_pointer-2);

            sep_char[pro_count]->cmd = strdup(tmp);
            sep_char[pro_count]->operate = REDIRECT;

            int remaining_length = strlen(cmd_unparsed) - buffer_pointer - 1;
            char* new_unparsed = slice_array(cmd_unparsed, buffer_pointer + 2, remaining_length-1);
            strcpy(cmd_unparsed, new_unparsed);
            free(new_unparsed);

            buffer_pointer = 0;
            pro_count++;
        }
        buffer_pointer++;
    }

    return sep_char;
}

void free_buffer(char *cmd_buffer, Command *current_cmd) 
{
    free(cmd_buffer);
    free(current_cmd);
}
