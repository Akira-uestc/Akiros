/*
 * Filename: execute.c
 * Author: Akira
 * Description: execute parsed command line
 */

#include <stdio.h>
#include "cmd.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void execute_cmd(Command* cmd_list)
{
    int pipe_fds[2];
    pid_t pid;
    int in_fd = 0;

    while (cmd_list != NULL) 
    {
        if (cmd_list->next != NULL) 
        {
            if (pipe(pipe_fds) == -1) 
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if(pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if(pid == 0)
        {
            if (in_fd != 0)
            {
                dup2(in_fd,STDIN_FILENO);
                close(in_fd);
            }

            if(cmd_list->next != NULL)
            {
                dup2(pipe_fds[1],STDIN_FILENO);
                close(pipe_fds[1]);
            }

            execvp(cmd_list->program, cmd_list->args);
            perror("execcvp");
            exit(EXIT_FAILURE);
        }
        else 
        {
            wait(NULL);
            if(cmd_list->next != NULL)
            {
                close(pipe_fds[1]);
            }
            in_fd = pipe_fds[0];
            cmd_list = cmd_list->next;
        }
    }
}
