CC = clang
CFLAGS = -g

OBJ = shell.o cmd.o builtin.o execute.o file.o

shell: $(OBJ)
	$(CC) $(OBJ) -o shell

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -Wall

run: shell
	./shell

debug:shell
	@gdb ./shell

clean:
	rm -f $(OBJ) shell


