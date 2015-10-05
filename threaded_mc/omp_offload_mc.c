/* omp_offload_mc.c --
 *
 * This code is a prototype Monte Carlo computation (though right now
 * it simply computes the expected value of the uniform generator,
 * which is a little silly).  See common_mc for the common parts of
 * the implementation across multiple parallel systems; this version
 * has the OpenMP-specific bits.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#pragma offload_attribute(push,target(mic)) //{

#include <omp.h>
#include "mt19937p.h"
#include "common_mc.h"


void thread_main(mc_result_t* result, mc_param_t* param, long seed)
{
    struct mt19937p mt;
    mc_result_t batch;
    int done_flag = 0;
    int nbatch = param->nbatch;
    sgenrand(seed, &mt);
    
    while (!done_flag) {

        /* Run batch of experiments */
        run_trials(&mt, nbatch, &batch);

        /* Update global counts and test for termination */
        #pragma omp critical
        {
            mc_result_update(result, &batch);
            done_flag = (done_flag || is_converged(param, result));
        }
    }
}

#pragma offload_attribute(pop) //}


int main(int argc, char** argv)
{
    mc_param_t param;
    mc_result_t result;
    double t1, t2;
    long seed;
    int nthreads;
    srandom(clock());
    mc_result_init(&result);
    nthreads = process_args(argc, argv, &param);
    if (nthreads == 0)
        omp_set_num_threads(nthreads);
    
    t1 = omp_get_wtime();
    #pragma offload target(mic)
    #pragma omp parallel shared(result, param, nthreads) private(seed)
    {
        #pragma omp single
        nthreads = omp_get_num_threads();
        
        #pragma omp critical        
        seed = random();
        
        thread_main(&result, &param, seed);
    }        
    t2 = omp_get_wtime();
    
    /* Print results */
    if (param.verbose) {
        print_params(&param);
        print_results(&result);
        printf("%d threads (OpenMP): %e s\n", nthreads, t2-t1);
    } else {
        printf("%e\n", t2-t1);
    }

    return 0;
}
