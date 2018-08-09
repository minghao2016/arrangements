#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <gmp.h>
#include "../combinatorics.h"
#include "../utils.h"
#include "../macros.h"

double npermutations_f(int* freq, size_t flen, size_t k);

SEXP next_k_permutations(SEXP _n, SEXP _k, SEXP _d, SEXP state, SEXP labels, SEXP _layout) {
    int i, j;
    int nprotect = 0;
    int status = 1;
    SEXP result;

    int n = as_uint(_n);
    int k = as_uint(_k);
    char layout = layout_flag(_layout);

    double dd = Rf_asInteger(_d) == -1 ? fallfact(n, k) : as_uint(_d);
    int d = verify_dimension(dd, k, layout);

    unsigned int* ap;
    unsigned int* cyclep;

    if (!variable_exist(state, "a", INTSXP, n, (void**) &ap)) {
        for(i=0; i<n; i++) ap[i] = i;
        status = 0;
    }
    if (!variable_exist(state, "cycle", INTSXP, k, (void**) &cyclep)) {
        for(i=0; i<k; i++) cyclep[i] = n - i;;
        status = 0;
    }

    #undef NEXT
    #define NEXT() \
        if (status == 0) { \
            status = 1; \
        } else if (!next_k_permutation(ap, cyclep, n, k)) { \
            status = 0; \
            break; \
        }

    int labels_type = TYPEOF(labels);
    if (labels_type == NILSXP) {
        RESULT_NILSXP(k);
    } else if (labels_type == INTSXP) {
        RESULT_INTSXP(k);
    } else if (labels_type == REALSXP) {
        RESULT_REALSXP(k);
    } else if (labels_type == STRSXP) {
        RESULT_STRSXP(k);
    }

    if (status == 0) {
        result = PROTECT(resize_layout(result, j, layout));
        nprotect++;
    }
    attach_factor_levels(result, labels);
    UNPROTECT(nprotect);
    return result;
}


SEXP num_k_permutations(SEXP _n, SEXP _k) {
    size_t n = as_uint(_n);
    size_t k = as_uint(_k);
    return Rf_ScalarReal(fallfact(n, k));
}

void n_k_permutations_bigz(mpz_t p, size_t n, size_t k) {
    size_t i;
    if (n < k) {
        mpz_set_ui(p, 0);
        return;
    }
    mpz_set_ui(p, 1);
    for(i=0; i<k; i++) {
        mpz_mul_ui(p, p, n - i);
    }
}

SEXP num_k_permutations_bigz(SEXP _n, SEXP _k) {
    size_t n = as_uint(_n);
    size_t k = as_uint(_k);
    mpz_t p;
    mpz_init(p);
    n_k_permutations_bigz(p, n, k);
    char* c = mpz_get_str(NULL, 10, p);
    SEXP out = Rf_mkString(c);
    free(c);
    mpz_clear(p);
    return out;
}


void ith_k_permutation(unsigned int* ar, unsigned int n, unsigned int k, unsigned int index) {
    unsigned int i, j;

    for (i = 0; i < k; i++) {
        j = fallfact(n - 1 - i, k - 1 - i);
        ar[i] = index / j;
        index = index % j;
    }

    for (i = k - 1; i > 0; i--) {
        j = i;
        while (j-- > 0) {
            if (ar[j] <= ar[i]) {
                ar[i]++;
            }
        }
    }
}

void ith_k_permutation_bigz(unsigned int* ar, unsigned int n, unsigned int k, mpz_t index) {
    unsigned int i, j;

    mpz_t q;
    mpz_init(q);
    mpz_t p;
    mpz_init(p);

    for (i = 0; i < k; i++) {
        n_k_permutations_bigz(p, n - 1 - i, k - 1 - i);
        mpz_tdiv_qr(q, index, index, p);
        ar[i] = mpz_get_ui(q);
    }

    for (i = k - 1; i > 0; i--) {
        j = i;
        while (j-- > 0) {
            if (ar[j] <= ar[i]) {
                ar[i]++;
            }
        }
    }

    mpz_clear(q);
    mpz_clear(p);
}

SEXP get_k_permutations(SEXP _n, SEXP _k, SEXP labels, SEXP _layout, SEXP _index, SEXP _nsample) {
    int i, j;
    int nprotect = 0;
    SEXP result = R_NilValue;

    int n = as_uint(_n);
    int k = as_uint(_k);
    char layout = layout_flag(_layout);

    double dd = _index == R_NilValue ? as_uint(_nsample) : Rf_length(_index);
    int d = verify_dimension(dd, k, layout);

    unsigned int* ap;
    ap = (unsigned int*) R_alloc(k, sizeof(int));

    double max = fallfact(n, k);
    int sampling = _index == R_NilValue;
    int bigz = TYPEOF(_index) == STRSXP || max > INT_MAX;

    if (bigz) {
        gmp_randstate_t randstate;
        mpz_t z;
        mpz_t maxz;
        mpz_init(z);

        if (sampling) {
            GetRNGstate();
            set_gmp_randstate(randstate);
            mpz_init(maxz);
            n_k_permutations_bigz(maxz, n , k);
        } else {
            if (TYPEOF(_index) != STRSXP) {
                _index = Rf_coerceVector(_index, STRSXP);
            }
        }

        #undef NEXT
        #define NEXT() \
            if (sampling) { \
                mpz_urandomm(z, randstate, maxz); \
            } else { \
                mpz_set_str(z, CHAR(STRING_ELT(_index, j)), 10); \
                mpz_sub_ui(z, z, 1); \
            } \
            ith_k_permutation_bigz(ap, n, k, z);

        int labels_type = TYPEOF(labels);
        if (labels_type == NILSXP) {
            RESULT_NILSXP(k);
        } else if (labels_type == INTSXP) {
            RESULT_INTSXP(k);
        } else if (labels_type == REALSXP) {
            RESULT_REALSXP(k);
        } else if (labels_type == STRSXP) {
            RESULT_STRSXP(k);
        }

        mpz_clear(z);
        if (sampling){
            mpz_clear(maxz);
            gmp_randclear(randstate);
            PutRNGstate();
        }

    } else {
        int* index;
        if (sampling) {
            GetRNGstate();
        } else {
            index = INTEGER(_index);
        }

        #undef NEXT
        #define NEXT() \
            if (sampling) { \
                ith_k_permutation(ap, n, k, floor(max * unif_rand())); \
            } else { \
                ith_k_permutation(ap, n, k, index[j] - 1); \
            }

        int labels_type = TYPEOF(labels);
        if (labels_type == NILSXP) {
            RESULT_NILSXP(k);
        } else if (labels_type == INTSXP) {
            RESULT_INTSXP(k);
        } else if (labels_type == REALSXP) {
            RESULT_REALSXP(k);
        } else if (labels_type == STRSXP) {
            RESULT_STRSXP(k);
        }

        if (sampling){
            PutRNGstate();
        }
    }

    attach_factor_levels(result, labels);
    UNPROTECT(nprotect);
    return result;
}