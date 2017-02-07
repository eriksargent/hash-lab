all: threaded 

threaded: threaded.c md5_constants.h
	gcc -std=gnu99 -Wno-cpp threaded.c -o threaded.out -pthread -w

threadtest: threaded
	./threaded.out

clean:
	rm *.out
