all: loopUnrolled threaded vector threadedVector

loopUnrolled: basic/loopUnrolled.c basic/md5_constants.h
	gcc -std=gnu99 basic/loopUnrolled.c -o basic/loopUnrolled.out -w

threaded: threaded/threaded.c threaded/md5_constants.h
	gcc -std=gnu99 -pthread threaded/threaded.c -o threaded/threaded.out

vector: basic/vector.c basic/md5_vector_constants.h
	gcc -std=gnu99 -mavx2 basic/vector.c -o basic/vector.out

threadedVector: threaded/threadedVector.c threaded/md5_vector_constants.h
	gcc -std=gnu99 -pthread -mavx2 threaded/threadedVector.c -o threaded/threadedVector.out

test: loopUnrolled
	basic/loopUnrolled.out

threadtest: threaded
	threaded/threaded.out

vectortest: vector
	basic/vector.out

threadvectest: threadedVector
	threaded/threadedVector.out

clean:
	rm */*.out
