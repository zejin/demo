#ifndef COMMON_MC_H
#define COMMON_MC_H

#include "mt19937p.h"

#define MAX_MC_THREADS 255

/* Parameters for termination criterion */
typedef struct mc_param_t {
    int    verbose;    /* Verbosity */
    double rtol;       /* Relative error bars at convergence */
    long   maxtrials;  /* Max trials */
    int    nbatch;     /* Trials per batch */
} mc_param_t;


/* Monte Carlo results */
typedef struct mc_result_t {
    double sum_X;
    double sum_X2;
    long   ntrials;
} mc_result_t;


void mc_param_init(mc_param_t* param);
void mc_result_init(mc_result_t* result);
void mc_result_update(mc_result_t* result, mc_result_t* batch);

void print_params(mc_param_t* param);
void print_results(mc_result_t* result);

int is_converged(mc_param_t* param, mc_result_t* result);

double run_trial(struct mt19937p* mt);
void run_trials(struct mt19937p* mt, int nbatch, mc_result_t* result);
int process_args(int argc, char** argv, mc_param_t* param);

#endif /* COMMON_MC_H */
