#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

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

char* read_line(const char* filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return NULL;
    }

    size_t buffer_size = 1024;
    char* file_buffer = malloc(buffer_size);
    if (!file_buffer) {
        perror("Memory allocation failed");
        close(fd);
        return NULL;
    }

    size_t file_ptr = 0;
    char c;
    ssize_t read_bytes;
    while ((read_bytes = read(fd, &c, 1)) > 0) {
        if (c == '\n' || c == EOF) {
            break;
        }
        file_buffer[file_ptr++] = c;

        // Expand buffer if necessary
        if (file_ptr >= buffer_size) {
            buffer_size *= 2;
            char* temp = realloc(file_buffer, buffer_size);
            if (!temp) {
                perror("Failed to expand buffer");
                free(file_buffer);
                close(fd);
                return NULL;
            }
            file_buffer = temp;
        }
    }

    file_buffer[file_ptr] = '\0';  // Null-terminate the string
    close(fd);
    return file_buffer;
}

