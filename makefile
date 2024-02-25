all: build 
	
build:
	gcc -Wall -Werror -g -D _POSIX_C_SOURCE=200809L -lpthread main.c receiver.c -o s-talk

run1:
	./s-talk 4455 127.0.0.1 22110
run2:
	./s-talk 22110 127.0.0.1 4455