CC = gcc
OBJECTS = src/main.o src/system.o src/auth.o src/sql.o src/sqlite3.o

atm: $(OBJECTS)
	$(CC) -o atm $(OBJECTS)

src/main.o: src/main.c src/header.h
	$(CC) -c src/main.c -o src/main.o

src/system.o: src/system.c src/header.h
	$(CC) -c src/system.c -o src/system.o

src/auth.o: src/auth.c src/header.h
	$(CC) -c src/auth.c -o src.auth.o

src/sql.o: src/sql.c src/header.h
	$(CC) -c src/sql.c -o src/sql.o

src/sqlite3.o: src/sqlite3.c src/sqlite3.h
	$(CC) -c src/sqlite3.c -o src/sqlite3.o

clean:
	del /Q $(OBJECTS)
	del atm.exe
