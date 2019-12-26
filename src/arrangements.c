#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include "combinations.h"
#include "compositions.h"
#include "permutations.h"
#include "partitions.h"
#include "macros.h"


SEXP validate_n_value(SEXP _x, SEXP _k, SEXP _n, SEXP _v, SEXP _freq, SEXP _replace) {
    int i;

    int has_vector = !Rf_isNull(_v);
    int multiset = !Rf_isNull(_freq);

    int n, k;
    int* fp;
    int flen;

    VALIDATE_ARGUMENTS();

    if (multiset) {
        n = flen;
    }

    return Rf_ScalarInteger(n);
}


static const R_CallMethodDef CallEntries[] = {
    {"validate_n_value", (DL_FUNC) &validate_n_value, 6},

    {"ncombinations", (DL_FUNC) &ncombinations, 7},
    {"collect_combinations", (DL_FUNC) &collect_combinations, 13},

    {"ncompositions", (DL_FUNC) &ncompositions, 3},
    {"collect_compositions", (DL_FUNC) &collect_compositions, 10},

    {"npermutations", (DL_FUNC) &npermutations, 7},
    {"collect_permutations", (DL_FUNC) &collect_permutations, 13},

    {"npartitions", (DL_FUNC) &npartitions, 3},
    {"collect_partitions", (DL_FUNC) &collect_partitions, 10},

    {NULL, NULL, 0}
};

void R_init_arrangements(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
