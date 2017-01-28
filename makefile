all: loopUnrolled 

loopUnrolled: loopUnrolled.c md5_constants.h
	gcc -std=gnu99 loopUnrolled.c -o loopUnrolled.out

threaded: threaded.c md5_constants.h
	gcc -std=gnu99 threaded.c -o threaded.out

test: loopUnrolled
	./loopUnrolled.out

threadtest: threaded
	./threaded.out

clean:
	rm *.out
