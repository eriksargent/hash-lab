all: loopUnrolled 

loopUnrolled: loopUnrolled.c md5_constants.h
	gcc -std=gnu99 loopUnrolled.c -o loopUnrolled.out -w

threaded: threaded.c md5_constants.h
	gcc -std=gnu99 -Wno-cpp threaded.c -o threaded.out -pthread

test: loopUnrolled
	./loopUnrolled.out

threadtest: threaded
	./threaded.out

clean:
	rm *.out
