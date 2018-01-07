#include <R.h>
#include <Rinternals.h>
#include "utils.h"

SEXP resize_row(SEXP x, size_t n, size_t m, size_t d) {
    if (TYPEOF(x) == INTSXP) {
        SEXP y = PROTECT(Rf_allocVector(INTSXP, n*d));
        int* yp = INTEGER(y);
        int* xp = INTEGER(x);
        size_t i, j;
        for (j=0; j<d; j++) {
            for (i=0; i<n; i++) {
                yp[j + i*d] = xp[j + i*m];
            }
        }
        UNPROTECT(1);
        return y;
    } else if (TYPEOF(x) == REALSXP) {
        SEXP y = PROTECT(Rf_allocVector(REALSXP, n*d));
        double* yp = REAL(y);
        double* xp = REAL(x);
        size_t i, j;
        for (j=0; j<d; j++) {
            for (i=0; i<n; i++) {
                yp[j + i*d] = xp[j + i*m];
            }
        }
        UNPROTECT(1);
        return y;
    } else if (TYPEOF(x) == STRSXP) {
        SEXP y = PROTECT(Rf_allocVector(STRSXP, n*d));
        size_t i, j;
        for (j=0; j<d; j++) {
            for (i=0; i<n; i++) {
                SET_STRING_ELT(y, j + i*d, STRING_ELT(x, j + i*m));
            }
        }
        UNPROTECT(1);
        return y;
    }
    return R_NilValue;
}

SEXP resize_col(SEXP x, size_t n, size_t m, size_t d) {
    if (TYPEOF(x) == INTSXP) {
        SEXP y = PROTECT(Rf_allocVector(INTSXP, n*d));
        int* yp = INTEGER(y);
        int* xp = INTEGER(x);
        size_t i;
        for (i=0; i<n*d; i++) yp[i] = xp[i];
        UNPROTECT(1);
        return y;
    } else if (TYPEOF(x) == REALSXP) {
        SEXP y = PROTECT(Rf_allocVector(REALSXP, n*d));
        double* yp = REAL(y);
        double* xp = REAL(x);
        size_t i;
        for (i=0; i<n*d; i++) yp[i] = xp[i];
        UNPROTECT(1);
        return y;
    } else if (TYPEOF(x) == STRSXP) {
        SEXP y = PROTECT(Rf_allocVector(REALSXP, n*d));
        size_t i;
        for (i=0; i<n*d; i++) SET_STRING_ELT(y, i, STRING_ELT(x, i));
        UNPROTECT(1);
        return y;
    }
    return R_NilValue;
}

SEXP resize_list(SEXP x, size_t m, size_t d) {
    PROTECT(x);
    SEXP y = PROTECT(Rf_allocVector(VECSXP, d));
    size_t i;
    for(i=0; i<d; i++) {
        SET_VECTOR_ELT(y, i, VECTOR_ELT(x, i));
    }
    UNPROTECT(2);
    return y;
}
