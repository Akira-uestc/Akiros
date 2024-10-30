/*
 * Filename: fs.c
 * Author: Akira
 * Description: 
 */

#include <stdio.h>
#include "operate.h"

int main(int argc, char *argv[])
{
    char* command = argv[1];
    printf("%s\n",command);
    fs_create();
    return 0;
}
