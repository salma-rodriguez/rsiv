CC = gcc48 # gcc for linux
FILES = ../src/main.c ../src/window.c ../src/image.c
OUT = rsiv
LIBS = -lX11 -lImlib2
FLAGS = -O3

build: $(FILES)
	$(CC) -o $(OUT) $(FILES) $(LIBS) $(FLAGS)
