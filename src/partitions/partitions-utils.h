#ifndef PARTITIONS_UTILS_H__
#define PARTITIONS_UTILS_H__
#include <gmp.h>
#include <stdlib.h>

double n_partitions(int n);

void n_partitions_bigz(mpz_t z, int n);

double n_k_partitions(int n, int k);

void n_k_partitions_bigz(mpz_t z, int n, int k);

double nkm(int n, int k, int m) ;

void nkm_bigz(mpz_t z, int n, int k, int m);

double n_k_min_partitions(int n, int k, int m);

void n_k_min_partitions_bigz(mpz_t z, int n, int k, int m);

double n_k_max_partitions(int n, int k, int m);

void n_k_max_partitions_bigz(mpz_t z, int n, int k, int m);

double n_min_partitions(int n, int m);

void n_min_partitions_bigz(mpz_t z, int n, int m);

double n_max_partitions(int n, int m);

void n_max_partitions_bigz(mpz_t z, int n, int m);

#endif /* end of include guard: PARTITIONS_UTILS_H__ */
