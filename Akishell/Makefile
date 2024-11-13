CC = clang
CFLAGS = -g

OBJ = main.o cmd.o execute.o inner.o file.o

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

