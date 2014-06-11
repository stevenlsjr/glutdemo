CC=gcc
CFLAGS=-g -Wall -std=gnu99
LDLIBS=-lGL -lGLEW -lglut -lILUT -lILU -lIL -lm

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
TARGET=bin/glutdemo

all:$(TARGET) 
	@echo $(OBJECTS)
	@$(TARGET)

$(TARGET): build $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDLIBS)
	
$(OBJECTS):

.PHONY:build
build:
	@mkdir -p bin build objects
	
.PHONY:run
run:
	@$(TARGET)
	
.PHONY:clean
clean:
	@rm -f $(TARGET)
	@rm -f $(OBJECTS)
