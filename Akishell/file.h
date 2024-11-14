/*
 * Filename: file.h
 * Author: Your Name
 * Description: 
 */

#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

_Bool if_file(char* file_path);
char* read_line(char* filepath);
void execute_from_file(char* filepath);

#endif /* FILE_H */
