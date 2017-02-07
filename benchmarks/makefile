all: pwck_base pwck_avx_base pwck_threaded_O0 pwck_threaded_O1 pwck_threaded_O2 pwck_threaded_O3 pwck_ifcase_O0 pwck_ifcase_O1 pwck_ifcase_O2 pwck_ifcase_O3 pwck_O0 pwck_O1 pwck_O2 pwck_O3



pwck_base: ../basic/loopUnrolled.c ../basic/md5_constants.h
	gcc -o pwck_base.out -O0 -std=gnu99 ../basic/loopUnrolled.c



pwck_avx_base: ../basic/vector.c ../basic/md5_vector_constants.h
	gcc -o pwck_avx_base.out -mavx2 -O0 -std=gnu99 ../basic/vector.c



pwck_threaded_O0: ../threaded/threadedVector.c ../threaded/md5_vector_constants.h
	gcc -o pwck_threaded_O0.out -mavx2 -O0 -std=gnu99 -pthread ../threaded/threadedVector.c

pwck_threaded_O1: ../threaded/threadedVector.c ../threaded/md5_vector_constants.h
	gcc -o pwck_threaded_O1.out -mavx2 -O1 -fomit-frame-pointer -std=gnu99 -pthread -w ../threaded/threadedVector.c

pwck_threaded_O2: ../threaded/threadedVector.c ../threaded/md5_vector_constants.h
	gcc -o pwck_threaded_O2.out -mavx2 -O2 -fomit-frame-pointer -std=gnu99 -pthread ../threaded/threadedVector.c

pwck_threaded_O3: ../threaded/threadedVector.c ../threaded/md5_vector_constants.h
	gcc -o pwck_threaded_O3.out -mavx2 -O3 -fomit-frame-pointer -std=gnu99 -pthread ../threaded/threadedVector.c



pwck_ifcase_O0: ../optimized/avx_optimized/vector.c ../optimized/avx_optimized/md5_vector_constants.h
	gcc -o pwck_ifcase_O0.out -mavx2 -O0 -std=gnu99 ../optimized/avx_optimized/vector.c
pwck_ifcase_O1:  ../optimized/avx_optimized/vector.c ../optimized/avx_optimized/md5_vector_constants.h
	gcc -o pwck_ifcase_O1.out -mavx2 -O1 -fomit-frame-pointer -std=gnu99 ../optimized/avx_optimized/vector.c
pwck_ifcase_O2:  ../optimized/avx_optimized/vector.c ../optimized/avx_optimized/md5_vector_constants.h
	gcc -o pwck_ifcase_O2.out -mavx2 -O2 -fomit-frame-pointer -std=gnu99 ../optimized/avx_optimized/vector.c
pwck_ifcase_O3:  ../optimized/avx_optimized/vector.c ../optimized/avx_optimized/md5_vector_constants.h
	gcc -o pwck_ifcase_O3.out -mavx2 -O3 -fomit-frame-pointer -std=gnu99 ../optimized/avx_optimized/vector.c



pwck_O0: ../optimized/avx_threaded_optimized/threadedVector.c ../optimized/avx_threaded_optimized/md5_vector_constants.h
	gcc -o pwck_O0.out -mavx2 -O0 -std=gnu99 -pthread ../optimized/avx_threaded_optimized/threadedVector.c

pwck_O1: ../optimized/avx_threaded_optimized/threadedVector.c ../optimized/avx_threaded_optimized/md5_vector_constants.h
	gcc -o pwck_O1.out -mavx2 -O1 -fomit-frame-pointer -std=gnu99 -pthread ../optimized/avx_threaded_optimized/threadedVector.c

pwck_O2: ../optimized/avx_threaded_optimized/threadedVector.c ../optimized/avx_threaded_optimized/md5_vector_constants.h
	gcc -o pwck_O2.out -mavx2 -O2 -fomit-frame-pointer -std=gnu99 -pthread ../optimized/avx_threaded_optimized/threadedVector.c

pwck_O3: ../optimized/avx_threaded_optimized/threadedVector.c ../optimized/avx_threaded_optimized/md5_vector_constants.h
	gcc -o pwck_O3.out -mavx2 -O3 -fomit-frame-pointer -std=gnu99 -pthread ../optimized/avx_threaded_optimized/threadedVector.c



benchmark: all
	./pwck_base.out
	./pwck_avx_base.out
	./pwck_threaded_O0.out
	./pwck_threaded_O1.out
	./pwck_threaded_O2.out
	./pwck_threaded_O3.out
	./pwck_ifcase_O0.out
	./pwck_ifcase_O1.out
	./pwck_ifcase_O2.out
	./pwck_ifcase_O3.out
	./pwck_O0.out
	./pwck_O1.out
	./pwck_O2.out
	./pwck_O3.out



clean:
	rm *.out
