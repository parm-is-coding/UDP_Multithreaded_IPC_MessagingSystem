all: build 
	
build:
	gcc -g main.c -o s-talk

run1:
	./s-talk 4455 127.0.0.1 22110
run2:
	./s-talk 22110 127.0.0.1 4455