#include "f2c.h"
#include "netlib.h"

/* Table of constant values */

static integer c__1 = 1;
static doublereal c_b8 = 0.;

/* Subroutine */ void dlarft_(direct, storev, n, k, v, ldv, tau, t, ldt)
const char *direct, *storev;
const integer *n, *k;
doublereal *v;
const integer *ldv;
const doublereal *tau;
doublereal *t;
const integer *ldt;
{
    /* System generated locals */
    integer t_dim1, t_offset, v_dim1, v_offset, i__1, i__2, i__3;
    doublereal d__1;

    /* Local variables */
    static integer i, j;
    static doublereal vii;

/*  -- LAPACK auxiliary routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     February 29, 1992 */

/*  Purpose                                                               */
/*  =======                                                               */
/*                                                                        */
/*  DLARFT forms the triangular factor T of a real block reflector H      */
/*  of order n, which is defined as a product of k elementary reflectors. */
/*                                                                        */
/*  If DIRECT = 'F', H = H(1) H(2) . . . H(k) and T is upper triangular;  */
/*                                                                        */
/*  If DIRECT = 'B', H = H(k) . . . H(2) H(1) and T is lower triangular.  */
/*                                                                        */
/*  If STOREV = 'C', the vector which defines the elementary reflector    */
/*  H(i) is stored in the i-th column of the array V, and                 */
/*                                                                        */
/*     H  =  I - V * T * V'                                               */
/*                                                                        */
/*  If STOREV = 'R', the vector which defines the elementary reflector    */
/*  H(i) is stored in the i-th row of the array V, and                    */
/*                                                                        */
/*     H  =  I - V' * T * V                                               */
/*                                                                        */
/*  Arguments                                                             */
/*  =========                                                             */
/*                                                                        */
/*  DIRECT  (input) CHARACTER*1                                           */
/*          Specifies the order in which the elementary reflectors are    */
/*          multiplied to form the block reflector:                       */
/*          = 'F': H = H(1) H(2) . . . H(k) (Forward)                     */
/*          = 'B': H = H(k) . . . H(2) H(1) (Backward)                    */
/*                                                                        */
/*  STOREV  (input) CHARACTER*1                                           */
/*          Specifies how the vectors which define the elementary         */
/*          reflectors are stored (see also Further Details):             */
/*          = 'C': columnwise                                             */
/*          = 'R': rowwise                                                */
/*                                                                        */
/*  N       (input) INTEGER                                               */
/*          The order of the block reflector H. N >= 0.                   */
/*                                                                        */
/*  K       (input) INTEGER                                               */
/*          The order of the triangular factor T (= the number of         */
/*          elementary reflectors). K >= 1.                               */
/*                                                                        */
/*  V       (input/output) DOUBLE PRECISION array, dimension              */
/*                               (LDV,K) if STOREV = 'C'                  */
/*                               (LDV,N) if STOREV = 'R'                  */
/*          The matrix V. See further details.                            */
/*                                                                        */
/*  LDV     (input) INTEGER                                               */
/*          The leading dimension of the array V.                         */
/*          If STOREV = 'C', LDV >= max(1,N); if STOREV = 'R', LDV >= K.  */
/*                                                                        */
/*  TAU     (input) DOUBLE PRECISION array, dimension (K)                 */
/*          TAU(i) must contain the scalar factor of the elementary       */
/*          reflector H(i).                                               */
/*                                                                        */
/*  T       (output) DOUBLE PRECISION array, dimension (LDT,K)            */
/*          The k by k triangular factor T of the block reflector.        */
/*          If DIRECT = 'F', T is upper triangular; if DIRECT = 'B', T is */
/*          lower triangular. The rest of the array is not used.          */
/*                                                                        */
/*  LDT     (input) INTEGER                                               */
/*          The leading dimension of the array T. LDT >= K.               */
/*                                                                        */
/*  Further Details                                                       */
/*  ===============                                                       */
/*                                                                        */
/*  The shape of the matrix V and the storage of the vectors which define */
/*  the H(i) is best illustrated by the following example with n = 5 and  */
/*  k = 3. The elements equal to 1 are not stored; the corresponding      */
/*  array elements are modified but restored on exit. The rest of the     */
/*  array is not used.                                                    */
/*                                                                        */
/*  DIRECT = 'F' and STOREV = 'C':         DIRECT = 'F' and STOREV = 'R': */
/*                                                                        */
/*               V = (  1       )                 V = (  1 v1 v1 v1 v1 )  */
/*                   ( v1  1    )                     (     1 v2 v2 v2 )  */
/*                   ( v1 v2  1 )                     (        1 v3 v3 )  */
/*                   ( v1 v2 v3 )                                         */
/*                   ( v1 v2 v3 )                                         */
/*                                                                        */
/*  DIRECT = 'B' and STOREV = 'C':         DIRECT = 'B' and STOREV = 'R': */
/*                                                                        */
/*               V = ( v1 v2 v3 )                 V = ( v1 v1  1       )  */
/*                   ( v1 v2 v3 )                     ( v2 v2 v2  1    )  */
/*                   (  1 v2 v3 )                     ( v3 v3 v3 v3  1 )  */
/*                   (     1 v3 )                                         */
/*                   (        1 )                                         */
/*                                                                        */
/*  ===================================================================== */

    /* Parameter adjustments */
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1 * 1;
    v -= v_offset;
    --tau;
    t_dim1 = *ldt;
    t_offset = 1 + t_dim1 * 1;
    t -= t_offset;

/*     Quick return if possible */

    if (*n == 0) {
        return;
    }

    if (lsame_(direct, "F")) {
        i__1 = *k;
        for (i = 1; i <= i__1; ++i) {
            if (tau[i] == 0.) {

/*              H(i)  =  I */

                i__2 = i;
                for (j = 1; j <= i__2; ++j) {
                    t[j + i * t_dim1] = 0.;
                }
            } else {

/*              general case */

                vii = v[i + i * v_dim1];
                v[i + i * v_dim1] = 1.;
                if (lsame_(storev, "C")) {

/*                 T(1:i-1,i) := - tau(i) * V(i:n,1:i-1)' * V(i:n,i) */

                    i__2 = *n - i + 1;
                    i__3 = i - 1;
                    d__1 = -tau[i];
                    dgemv_("Transpose", &i__2, &i__3, &d__1, &v[i + v_dim1], ldv,
                           &v[i + i * v_dim1], &c__1, &c_b8, &t[i * t_dim1 + 1], &c__1);
                } else {

/*                 T(1:i-1,i) := - tau(i) * V(1:i-1,i:n) * V(i,i:n)' */

                    i__2 = i - 1;
                    i__3 = *n - i + 1;
                    d__1 = -tau[i];
                    dgemv_("No transpose", &i__2, &i__3, &d__1, &v[i * v_dim1 + 1], ldv,
                           &v[i + i * v_dim1], ldv, &c_b8, &t[i * t_dim1 + 1], &c__1);
                }
                v[i + i * v_dim1] = vii;

/*              T(1:i-1,i) := T(1:i-1,1:i-1) * T(1:i-1,i) */

                i__2 = i - 1;
                dtrmv_("Upper", "No transpose", "Non-unit", &i__2, &t[t_offset], ldt, &t[i * t_dim1 + 1], &c__1);
                t[i + i * t_dim1] = tau[i];
            }
        }
    } else {
        for (i = *k; i >= 1; --i) {
            if (tau[i] == 0.) {

/*              H(i)  =  I */

                i__1 = *k;
                for (j = i; j <= i__1; ++j) {
                    t[j + i * t_dim1] = 0.;
                }
            } else {

/*              general case */

                if (i < *k) {
                    if (lsame_(storev, "C")) {
                        vii = v[*n - *k + i + i * v_dim1];
                        v[*n - *k + i + i * v_dim1] = 1.;

/*                    T(i+1:k,i) := */
/*                            - tau(i) * V(1:n-k+i,i+1:k)' * V(1:n-k+i,i) */

                        i__1 = *n - *k + i;
                        i__2 = *k - i;
                        d__1 = -tau[i];
                        dgemv_("Transpose", &i__1, &i__2, &d__1, &v[(i + 1) * v_dim1 + 1], ldv,
                               &v[i * v_dim1 + 1], &c__1, &c_b8, &t[i + 1 + i * t_dim1], &c__1);
                        v[*n - *k + i + i * v_dim1] = vii;
                    } else {
                        vii = v[i + (*n - *k + i) * v_dim1];
                        v[i + (*n - *k + i) * v_dim1] = 1.;

/*                    T(i+1:k,i) := */
/*                            - tau(i) * V(i+1:k,1:n-k+i) * V(i,1:n-k+i)' */

                        i__1 = *k - i;
                        i__2 = *n - *k + i;
                        d__1 = -tau[i];
                        dgemv_("No transpose", &i__1, &i__2, &d__1, &v[i + 1 + v_dim1], ldv,
                               &v[i + v_dim1], ldv, &c_b8, &t[i + 1 + i * t_dim1], &c__1);
                        v[i + (*n - *k + i) * v_dim1] = vii;
                    }

/*                 T(i+1:k,i) := T(i+1:k,i+1:k) * T(i+1:k,i) */

                    i__1 = *k - i;
                    dtrmv_("Lower", "No transpose", "Non-unit", &i__1, &t[i+1+(i+1)*t_dim1], ldt, &t[i+1+i*t_dim1], &c__1);
                }
                t[i + i * t_dim1] = tau[i];
            }
        }
    }
} /* dlarft_ */
