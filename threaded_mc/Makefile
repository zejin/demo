.PHONY: all

all: pthread_mc.x omp_mc.x

pthread_mc.x: pthread_mc.o mt19937p.o
	$(CC) -o $@ $^ -lpthread -lm
	
omp_mc.x: omp_mc.o mt19937p.o
	$(CC) -fopenmp -o $@ $^ -lm

omp_mc.o: omp_mc.c
	$(CC) -c -O3 -fopenmp $< -lpthread

%.o: %.c
	$(CC) -c -O3 $<

clean:
	rm -f *.o *.x
