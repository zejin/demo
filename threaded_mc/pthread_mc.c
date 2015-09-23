/* pthreads_mc.c --
 *
 * This code is a prototype Monte Carlo computation (though right now
 * it simply computes the expected value of the uniform generator,
 * which is a little silly).  See common_mc for the common parts of
 * the implementation across multiple parallel systems; this version
 * has the pthreads-specific bits.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

#include "mt19937p.h"
#include "common_mc.h"


typedef struct thread_param_t {
    pthread_mutex_t* counts_lock;  /* Lock on results */
    mc_result_t* result;           /* Shared results  */
    mc_param_t* param;             /* Params (read only) */
    long seed;                     /* Random seed (read only) */
} thread_param_t;


void* thread_main(void* arg)
{
    thread_param_t* tparam = (thread_param_t*) arg;
    struct mt19937p mt;
    mc_result_t batch;
    int done_flag = 0;
    int nbatch = tparam->param->nbatch;
    sgenrand(tparam->seed, &mt);

    while (!done_flag) {

        /* Run batch of experiments */
        run_trials(&mt, nbatch, &batch);

        /* Update global counts and test for termination */
        pthread_mutex_lock(tparam->counts_lock);
        done_flag = (done_flag || is_converged(tparam->param, tparam->result));
        mc_result_update(tparam->result, &batch);
        done_flag = (done_flag || is_converged(tparam->param, tparam->result));
        pthread_mutex_unlock(tparam->counts_lock);

    }
    return NULL;
}


int main(int argc, char** argv)
{
    mc_param_t param;
    mc_result_t result;
    int nthreads = process_args(argc, argv, &param);
    thread_param_t tparams[MAX_MC_THREADS];
    pthread_t threads[MAX_MC_THREADS];
    int i;
    double EX, EX2, stdX, t_elapsed;
    struct timeval t1, t2;
    pthread_mutex_t counts_lock;

    /* Use default nthreads */
    if (nthreads == 0)
        nthreads = 1;
    srandom(clock());
    mc_result_init(&result);
    pthread_mutex_init(&counts_lock, NULL);
    
    /* Run parallel experiments on nthreads threads */
    gettimeofday(&t1, NULL);
    for (i = 0; i < nthreads; ++i) {
        tparams[i].counts_lock = &counts_lock;
        tparams[i].result = &result;
        tparams[i].param = &param;
        tparams[i].seed = random();
    }
    for (i = 1; i < nthreads; ++i)
        pthread_create(&threads[i], NULL, thread_main, (void*) (tparams+i));
    thread_main((void*) &tparams);
    for (i = 1; i < nthreads; ++i)
        pthread_join(threads[i], NULL);
    gettimeofday(&t2, NULL);
    pthread_mutex_destroy(&counts_lock);

    t_elapsed = (t2.tv_sec-t1.tv_sec) + (t2.tv_usec-t1.tv_usec)/1.0e6;

    if (param.verbose) {
        print_params(&param);
        print_results(&result);
        printf("%d threads (pthreads): %e s\n", nthreads, t_elapsed);
    } else {
        printf("%e\n", t_elapsed);
    }

    return 0;
}
