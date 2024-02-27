all: build 
	
build:
	gcc -g -D _POSIX_C_SOURCE=200809L -lpthread main.c receiver.c sender.c list.o -o s-talk

run1: build
	clear
	./s-talk 4455 127.0.0.1 22110
run2: build
	clear
	./s-talk 22110 127.0.0.1 4455

valgrind: 
	valgrind ./s-talk 4455 127.0.0.1 22110
clean:
	rm -f s-talk