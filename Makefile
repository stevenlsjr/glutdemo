# emscripten makefile

CC=emcc
CFLAGS=
LDFLAGS=

SRC=$(wildcard src/*.c src/**/*.c)
OBJ=$(patsubst src/%.c, em/%.o, $(SRC))
TGT=em/index.html


em/%.o:src/%.c
	$(CC) $(CFLAGS) -c $@ -o $^

all: $(TGT)
	mkdir -p em

clean:
	rm -fr $(OBJ) $(TGT)

$(TGT):$(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^
