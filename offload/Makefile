.PHONY: submit clean

offload:
	icc -o offload offload.c

submit: offload
	qsub offload.pbs

clean:
	rm -f offload offload.o*
