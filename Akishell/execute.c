/*
 * Filename: execute.c
 * Author: Akira
 * Description: Execute parsed command line with pipes and redirections
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "cmd.h"

void execute_cmd(Command* cmd_list) {
    int pipe_fds[2] = {-1, -1};
    pid_t pid;
    int in_fd = STDIN_FILENO;
    int saved_stdout = dup(STDOUT_FILENO);

    while (cmd_list != NULL) {
        if (cmd_list->operate == O_REDIRECT) {
            int fd = open(cmd_list->program, O_WRONLY|O_CREAT|O_TRUNC, 0644);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        if (cmd_list->operate == I_REDIRECT) {
            int fd = open(cmd_list->program, O_RDONLY);
            if (fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (cmd_list->operate == PIPE || cmd_list->next != NULL) {
            if (pipe(pipe_fds) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            if (cmd_list->operate != O_REDIRECT) {
                dup2(saved_stdout, STDOUT_FILENO);
            }

            if (in_fd != STDIN_FILENO) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            if (cmd_list->next != NULL) {
                close(pipe_fds[0]);
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }

            execvp(cmd_list->program, cmd_list->args);
            perror("execvp");
            exit(EXIT_FAILURE);
        } 
        else {
            if (in_fd != STDIN_FILENO) {
                close(in_fd);
            }

            if (cmd_list->next != NULL) {
                close(pipe_fds[1]);
                in_fd = pipe_fds[0];
            }

            if (cmd_list->operate == O_REDIRECT) {
                dup2(saved_stdout, STDOUT_FILENO);
            }
            if (cmd_list->operate == I_REDIRECT) {
                dup2(STDIN_FILENO, in_fd);
            }

            cmd_list = cmd_list->next;
        }
    }

    while (waitpid(-1, NULL, 0) > 0);
    close(saved_stdout);
}