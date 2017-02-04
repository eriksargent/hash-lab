all: loopUnrolled threaded vector

loopUnrolled: basic/loopUnrolled.c basic/md5_constants.h
	gcc -std=gnu99 basic/loopUnrolled.c -o basic/loopUnrolled.out -w

threaded: threaded/threaded.c threaded/md5_constants.h
	gcc -std=gnu99 -Wno-cpp -pthread -O3 threaded/threaded.c -o threaded/threaded.out

vector: basic/vector.c basic/md5_vector_constants.h
	gcc -mavx2 basic/vector.c -o basic/vector.out

test: loopUnrolled
	basic/loopUnrolled.out

threadtest: threaded
	threaded/threaded.out

vectortest: vector
	basic/vector.out

clean:
	rm */*.out
