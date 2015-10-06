from subprocess import check_output


def timing(b):
    return float(check_output(["./omp_offload_mc.x",
                               "-b", "{0}".format(b),
                               "-v", "0"]))

N = 10000000
nruns = 3
batches = [10, 20, 40, 80,
           100, 200, 400, 800,  
           1000, 2000, 4000, 8000]
times1 = [timing(b) for b in batches]
times2 = [timing(b) for b in batches]
times3 = [timing(b) for b in batches]

with open('timings.csv', 'w') as f:
    f.write("size,t1,t2,t3\n")
    for b, t1, t2, t3 in zip(batches, times1, times2, times3):
        f.write("{0},{1},{2},{3}\n".format(b, t1, t2, t3))
