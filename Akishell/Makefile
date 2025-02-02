CC = clang
CFLAGS = -g -Wall

OBJ = shell.o cmd.o builtin.o execute.o file.o

shell: $(OBJ)
	$(CC) $(OBJ) -o shell

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: shell
	./shell

debug:shell
	@gdb ./shell

clean:
	rm -f $(OBJ) shell


