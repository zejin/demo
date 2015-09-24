import sys
import numpy as np
import pandas as pd
import matplotlib
matplotlib.use('Agg')
import matplotlib.pylab as plt


def main(timings):
    df = pd.read_csv(timings)
    N = 10000000
    batches = df['size']
    times = (df['t1'] + df['t2'] + df['t3'])/3
    A = np.zeros((len(batches), 2))
    A[:,0] = N/batches
    A[:,1] = N
    (c, resid, rank, s) = np.linalg.lstsq(A, times)
    plt.plot(N/np.array(batches), np.array(times), '*')
    plt.plot(N/np.array(batches), np.dot(A,c), ':')
    print(c)
    plt.savefig('timing.svg', bbox_inches='tight')

    
if __name__ == "__main__":
    main(sys.argv[1] if len(sys.argv) > 1 else "timings.csv")
