all: tv_O0 tv_O1 tv_O2 tv_O3

tv_O0: threadedVector.c md5_vector_constants.h
	gcc -o tv_O0.out -mavx2 -O0 -std=gnu99 -pthread -w threadedVector.c
	
tv_O1: threadedVector.c md5_vector_constants.h
	gcc -o tv_O1.out -mavx2 -O1 -std=gnu99 -pthread -w threadedVector.c

tv_O2: threadedVector.c md5_vector_constants.h
	gcc -o tv_O2.out -mavx2 -O2 -std=gnu99 -pthread -w threadedVector.c

tv_O3: threadedVector.c md5_vector_constants.h
	gcc -o tv_O3.out -mavx2 -O3 -std=gnu99 -pthread -w threadedVector.c
