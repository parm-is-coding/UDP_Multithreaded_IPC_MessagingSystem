all: build 
	
build:
	gcc -g main.c -o s-talk

run:
	./s-talk 127.0.0.1 22110