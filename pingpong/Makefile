CC=mpicc
CFLAGS=-std=gnu99 -Wall

.PHONY: intel icc-openmpi gcc-openmpi clean

ping.x: ping.c
	$(CC) $(CFLAGS) $< -o $@

intel:
	( module load cs5220 ; make CC=mpiicc )

icc-openmpi:
	( module load cs5220 ; module load openmpi/1.10.0-icc-15.0.3 ; make )

gcc-openmpi:
	( module load cs5220 ; module load openmpi/1.10.0-gcc-4.9.2 ; make )

clean:
	rm -f ping.x ping-*.o*

