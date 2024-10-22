#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "cmd.h"
#include "execute.h"
#include "inner.h"

int buffer_size = 1024;
int if_loop = 1;

int main()
{
    printf("welcome to use akira's simple shell!\n");
    
    while (if_loop) 
    {
        char* cmd_buffer = malloc(sizeof(buffer_size));
        Command* current_cmd = malloc(sizeof(Command));
        
        printf("-> ");
        
        cmd_buffer = get_cmd(cmd_buffer,buffer_size);
        current_cmd = parse_cmd(cmd_buffer);

        if(is_builtin(current_cmd,cmd_buffer))
        {
            continue;
        }
        else 
        {
            execute_cmd(current_cmd);
            free_buffer(cmd_buffer, current_cmd);
        }
        
    }

    return 0;
}
