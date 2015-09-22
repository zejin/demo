.PHONY: all mac clean distclean

CC=icc
OMP_CFLAGS=-openmp
PTHREAD_LIBS=-lpthread 

all: pthread_mc.x omp_mc.x 

mac:
	make CC=gcc-5 OMP_CFLAGS=-fopenmp

pthread_mc.x: pthread_mc.o common_mc.o mt19937p.o
	$(CC) -o $@ $^ $(PTHREAD_LIBS) -lm
	
omp_mc.x: omp_mc.o common_mc.o mt19937p.o
	$(CC) $(OMP_CFLAGS) -o $@ $^ -lm

omp_offload_mc.x: omp_offload_mc.o common_mc.o mt19937p.o
	$(CC) $(OMP_CFLAGS) -o $@ $^ -lm

omp_mc.o: omp_mc.c
	$(CC) -c -O3 $(OMP_CFLAGS) $< 

omp_offload_mc.o: omp_offload_mc.c
	$(CC) -c -O3 $(OMP_CFLAGS) $< 

%.o: %.c
	$(CC) -c -O3 $<

clean:
	rm -f *.o *.x

distclean: clean
	rm -f *.o*
