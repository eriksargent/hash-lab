all: loopUnrolled 

loopUnrolled: loopUnrolled.c md5_constants.h
	gcc -std=gnu99 loopUnrolled.c -o loopUnrolled.out

test: loopUnrolled
	./loopUnrolled.out

clean:
	rm *.out
