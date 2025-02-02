#include <stdlib.h>
#include <stdbool.h>
#include "cmd.h"

//TODO:enable show different prompts read from configuration file.
char* prompt = "Welcome to use akira's simple shell";

//main program control flag.
_Bool if_loop = true;
//Max buffer size of the command line.
const unsigned int buffer_size = 1024;
unsigned int MAX_NUM = 1024;

bool is_builtin(Command* cmd, char* cmd_buffer);
void execute_cmd(Command* cmd_list);
void execute_from_file(char* filepath);

int main(int argc, char *argv[])
{
    if(argc >= 2) {
        for (int i = 1; i < argc; i++) {
            char* file_path = argv[i];
            execute_from_file(file_path);
        }
        return 0;
    }
    printf("%s\n",prompt);

    while (if_loop) 
    {
        char* cmd_buffer = malloc(sizeof(buffer_size));
        Command* current_cmd = malloc(sizeof(Command)*MAX_NUM);

        printf("-> ");

        cmd_buffer = get_cmd(cmd_buffer,buffer_size);
        current_cmd = parse_cmd(cmd_buffer);

        if(is_builtin(current_cmd,cmd_buffer))
        {
            free_buffer(cmd_buffer, current_cmd);
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
