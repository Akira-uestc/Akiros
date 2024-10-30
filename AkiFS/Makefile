CC = clang
CFLAGS = -g

OBJ = fs.o operate.o

fs:$(OBJ)
	$(CC) $(OBJ) -o AkiFS

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ -Wall

run:fs
	./AkiFS

debug:fs
	gdb ./AkiFS

clean:
	rm $(OBJ) AkiFS
