#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include "cmd.h"
#include "execute.h"

_Bool if_file(const char* filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (fd != -1) {
        close(fd);  // File exists and is readable
        return true;
    } else {
        perror("Error accessing file");
        return false;
    }
}

void execute_from_file(char* filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    size_t buffer_size = 1024;
    char* file_buffer = malloc(buffer_size);
    if (!file_buffer) {
        perror("Memory allocation failed");
        close(fd);
        return;
    }

    ssize_t file_ptr = 0;
    ssize_t read_bytes;
    char c;
    
    while ((read_bytes = read(fd, &c, 1)) > 0) 
    {
        if (c == '\n') 
        {
            file_buffer[file_ptr] = '\0';

            Command* line_parsed = parse_cmd(file_buffer);
            execute_cmd(line_parsed);

            file_ptr = 0;
        } 
        else 
        {
            file_buffer[file_ptr++] = c;

            if (file_ptr >= buffer_size) 
            {
                buffer_size *= 2;
                char* temp = realloc(file_buffer, buffer_size);
                if (!temp) 
                {
                    perror("Failed to expand buffer");
                    free(file_buffer);
                    close(fd);
                    return;
                }
                file_buffer = temp;
            }
        }
    }
    close(fd);
    free(file_buffer);
}
