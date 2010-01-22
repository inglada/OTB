C   VERSION 2    DOES NOT USE EISPACK
C
C ------------------------------------------------------------------
C
      SUBROUTINE DNLASO(OP, IOVECT, N, NVAL, NFIG, NPERM,
     *   NMVAL, VAL, NMVEC, VEC, NBLOCK, MAXOP, MAXJ, WORK,
     *   IND, IERR)
C
      INTEGER N, NVAL, NFIG, NPERM, NMVAL, NMVEC, NBLOCK,
     *   MAXOP, MAXJ, IND(1), IERR
      DOUBLE PRECISION VEC(NMVEC,1), VAL(NMVAL,4), WORK(1)
      EXTERNAL OP, IOVECT
C
C AUTHOR/IMPLEMENTER D.S.SCOTT-B.N.PARLETT/D.S.SCOTT
C
C COMPUTER SCIENCES DEPARTMENT
C UNIVERSITY OF TEXAS AT AUSTIN
C AUSTIN, TX 78712
C
C VERSION 2 ORIGINATED APRIL 1982
C
C CURRENT VERSION  JUNE 1983
C
C DNLASO FINDS A FEW EIGENVALUES AND EIGENVECTORS AT EITHER END OF
C THE SPECTRUM OF A LARGE SPARSE SYMMETRIC MATRIX.  THE SUBROUTINE
C DNLASO IS PRIMARILY A DRIVER FOR SUBROUTINE DNWLA WHICH IMPLEMENTS
C THE LANCZOS ALGORITHM WITH SELECTIVE ORTHOGONALIZATION AND
C SUBROUTINE DNPPLA WHICH POST PROCESSES THE OUTPUT OF DNWLA.
C HOWEVER DNLASO DOES CHECK FOR INCONSISTENCIES IN THE CALLING
C PARAMETERS AND DOES PREPROCESS ANY USER SUPPLIED EIGENPAIRS.
C DNLASO ALWAYS LOOKS FOR THE SMALLEST (LEFTMOST) EIGENVALUES.  IF
C THE LARGEST EIGENVALUES ARE DESIRED DNLASO IMPLICITLY USES THE
C NEGATIVE OF THE MATRIX.
C
C
C ON INPUT
C
C
C   OP   A USER SUPPLIED SUBROUTINE WITH CALLING SEQUENCE
C     OP(N,M,P,Q).  P AND Q ARE N X M MATRICES AND Q IS
C     RETURNED AS THE MATRIX TIMES P.
C
C   IOVECT   A USER SUPPLIED SUBROUTINE WITH CALLING SEQUENCE
C     IOVECT(N,M,Q,J,K).  Q IS AN N X M MATRIX.  IF K = 0
C     THE COLUMNS OF Q ARE STORED AS THE (J-M+1)TH THROUGH
C     THE JTH LANCZOS VECTORS.  IF K = 1 THEN Q IS RETURNED
C     AS THE (J-M+1)TH THROUGH THE JTH LANCZOS VECTORS.  SEE
C     DOCUMENTATION FOR FURTHER DETAILS AND EXAMPLES.
C
C   N   THE ORDER OF THE MATRIX.
C
C   NVAL   NVAL SPECIFIES THE EIGENVALUES TO BE FOUND.
C     DABS(NVAL)  IS THE NUMBER OF EIGENVALUES DESIRED.
C     IF NVAL < 0 THE ALGEBRAICALLY SMALLEST (LEFTMOST)
C     EIGENVALUES ARE FOUND.  IF NVAL > 0 THE ALGEBRAICALLY
C     LARGEST (RIGHTMOST) EIGENVALUES ARE FOUND.  NVAL MUST NOT
C     BE ZERO.  DABS(NVAL) MUST BE LESS THAN  MAXJ/2.
C
C   NFIG   THE NUMBER OF DECIMAL DIGITS OF ACCURACY DESIRED IN THE
C     EIGENVALUES.  NFIG MUST BE GREATER THAN OR EQUAL TO 1.
C
C   NPERM   AN INTEGER VARIABLE WHICH SPECIFIES THE NUMBER OF USER
C     SUPPLIED EIGENPAIRS.  IN MOST CASES NPERM WILL BE ZERO.  SEE
C     DOCUMENTAION FOR FURTHER DETAILS OF USING NPERM GREATER
C     THAN ZERO.  NPERM MUST NOT BE LESS THAN ZERO.
C
C   NMVAL   THE ROW DIMENSION OF THE ARRAY VAL.  NMVAL MUST BE GREATER
C     THAN OR EQUAL TO DABS(NVAL).
C
C   VAL   A TWO DIMENSIONAL DOUBLE PRECISION ARRAY OF ROW
C     DIMENSION NMVAL AND COLUMN DIMENSION AT LEAST 4.  IF NPERM
C     IS GREATER THAN ZERO THEN CERTAIN INFORMATION MUST BE STORED
C     IN VAL.  SEE DOCUMENTATION FOR DETAILS.
C
C   NMVEC   THE ROW DIMENSION OF THE ARRAY VEC.  NMVEC MUST BE GREATER
C     THAN OR EQUAL TO N.
C
C   VEC   A TWO DIMENSIONAL DOUBLE PRECISION ARRAY OF ROW
C     DIMENSION NMVEC AND COLUMN DIMENSION AT LEAST DABS(NVAL).  IF
C     NPERM > 0 THEN THE FIRST NPERM COLUMNS OF VEC MUST
C     CONTAIN THE USER SUPPLIED EIGENVECTORS.
C
C   NBLOCK   THE BLOCK SIZE.  SEE DOCUMENTATION FOR CHOOSING
C     AN APPROPRIATE VALUE FOR NBLOCK.  NBLOCK MUST BE GREATER
C     THAN ZERO AND LESS THAN  MAXJ/6.
C
C   MAXOP   AN UPPER BOUND ON THE NUMBER OF CALLS TO THE SUBROUTINE
C     OP.  DNLASO TERMINATES WHEN MAXOP IS EXCEEDED.  SEE
C     DOCUMENTATION FOR GUIDELINES IN CHOOSING A VALUE FOR MAXOP.
C
C   MAXJ   AN INDICATION OF THE AVAILABLE STORAGE (SEE WORK AND
C     DOCUMENTATION ON IOVECT).  FOR THE FASTEST CONVERGENCE MAXJ
C     SHOULD BE AS LARGE AS POSSIBLE, ALTHOUGH IT IS USELESS TO HAVE
C     MAXJ LARGER THAN MAXOP*NBLOCK.
C
C   WORK   A DOUBLE PRECISION ARRAY OF DIMENSION AT LEAST AS
C     LARGE AS
C
C         2*N*NBLOCK + MAXJ*(NBLOCK+NV+2) + 2*NBLOCK*NBLOCK + 3*NV
C
C            + THE MAXIMUM OF
C                 N*NBLOCK
C                   AND
C         MAXJ*(2*NBLOCK+3) + 2*NV + 6 + (2*NBLOCK+2)*(NBLOCK+1)
C
C     WHERE NV = DABS(NVAL)
C
C     THE FIRST N*NBLOCK ELEMENTS OF WORK MUST CONTAIN THE DESIRED
C     STARTING VECTORS.  SEE DOCUMENTATION FOR GUIDELINES IN
C     CHOOSING STARTING VECTORS.
C
C   IND   AN INTEGER ARRAY OF DIMENSION AT LEAST DABS(NVAL).
C
C   IERR   AN INTEGER VARIABLE.
C
C
C ON OUTPUT
C
C
C   NPERM   THE NUMBER OF EIGENPAIRS NOW KNOWN.
C
C   VEC   THE FIRST NPERM COLUMNS OF VEC CONTAIN THE EIGENVECTORS.
C
C   VAL   THE FIRST COLUMN OF VAL CONTAINS THE CORRESPONDING
C     EIGENVALUES.  THE SECOND COLUMN CONTAINS THE RESIDUAL NORMS OF
C     THE EIGENPAIRS WHICH ARE BOUNDS ON THE ACCURACY OF THE EIGEN-
C     VALUES.  THE THIRD COLUMN CONTAINS MORE DOUBLE PRECISIONISTIC ESTIMATES
C     OF THE ACCURACY OF THE EIGENVALUES.  THE FOURTH COLUMN CONTAINS
C     ESTIMATES OF THE ACCURACY OF THE EIGENVECTORS.  SEE
C     DOCUMENTATION FOR FURTHER INFORMATION ON THESE QUANTITIES.
C
C   WORK   IF WORK IS TERMINATED BEFORE COMPLETION (IERR = -2)
C     THE FIRST N*NBLOCK ELEMENTS OF WORK CONTAIN THE BEST VECTORS
C     FOR RESTARTING THE ALGORITHM AND DNLASO CAN BE IMMEDIATELY
C     RECALLED TO CONTINUE WORKING ON THE PROBLEM.
C
C   IND   IND(1)  CONTAINS THE ACTUAL NUMBER OF CALLS TO OP.  ON SOME
C     OCCASIONS THE NUMBER OF CALLS TO OP MAY BE SLIGHTLY LARGER
C     THAN MAXOP.
C
C   IERR   AN ERROR COMPLETION CODE.  THE NORMAL COMPLETION CODE IS
C     ZERO.  SEE THE DOCUMENTATION FOR INTERPRETATIONS OF NON-ZERO
C     COMPLETION CODES.
C
C
C INTERNAL VARIABLES.
C
C
      INTEGER I, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11,
     *  I12, I13, M, NBAND, NOP, NV, IABS, MAX0
      LOGICAL RARITZ, SMALL
      DOUBLE PRECISION DELTA, EPS, TEMP, DNRM2, DABS, TARR(1)
      EXTERNAL DNPPLA, DNWLA, DORTQR, DCOPY, DNRM2, DVSORT
C
C NOP   RETURNED FROM DNWLA AS THE NUMBER OF CALLS TO THE
C   SUBROUTINE OP.
C
C NV   SET EQUAL TO DABS(NVAL), THE NUMBER OF EIGENVALUES DESIRED,
C   AND PASSED TO DNWLA.
C
C SMALL   SET TO .TRUE. IF THE SMALLEST EIGENVALUES ARE DESIRED.
C
C RARITZ   RETURNED FROM DNWLA AND PASSED TO DNPPLA.  RARITZ IS .TRUE.
C   IF A FINAL RAYLEIGH-RITZ PROCEDURE IS NEEDED.
C
C DELTA   RETURNED FROM DNWLA AS THE EIGENVALUE OF THE MATRIX
C   WHICH IS CLOSEST TO THE DESIRED EIGENVALUES.
C
C DNPPLA   A SUBROUTINE FOR POST-PROCESSING THE EIGENVECTORS COMPUTED
C   BY DNWLA.
C
C DNWLA   A SUBROUTINE FOR IMPLEMENTING THE LANCZOS ALGORITHM
C   WITH SELECTIVE ORTHOGONALIZATION.
C
C DMVPC   A SUBROUTINE FOR COMPUTING THE RESIDUAL NORM AND
C   ORTHOGONALITY COEFFICIENT OF GIVEN RITZ VECTORS.
C
C DORTQR   A SUBROUTINE FOR ORTHONORMALIZING A BLOCK OF VECTORS
C   USING HOUSEHOLDER REFLECTIONS.
C
C DAXPY,DCOPY,DDOT,DNRM2,DSCAL,DSWAP   A SUBSET OF THE BASIC LINEAR
C   ALGEBRA SUBPROGRAMS USED FOR VECTOR MANIPULATION.
C
C DLARAN   A SUBROUTINE TO GENERATE RANDOM VECTORS
C
C DLAEIG, DLAGER, DLABCM, DLABFC   SUBROUTINES FOR BAND EIGENVALUE
C   CALCULATIONS.
C
C ------------------------------------------------------------------
C
C THIS SECTION CHECKS FOR INCONSISTENCY IN THE INPUT PARAMETERS.
C
      NV = IABS(NVAL)
      IND(1) = 0
      IERR = 0
      IF (N.LT.6*NBLOCK) IERR = 1
      IF (NFIG.LE.0) IERR = IERR + 2
      IF (NMVEC.LT.N) IERR = IERR + 4
      IF (NPERM.LT.0) IERR = IERR + 8
      IF (MAXJ.LT.6*NBLOCK) IERR = IERR + 16
      IF (NV.LT.MAX0(1,NPERM)) IERR = IERR + 32
      IF (NV.GT.NMVAL) IERR = IERR + 64
      IF (NV.GT.MAXOP) IERR = IERR + 128
      IF (NV.GE.MAXJ/2) IERR = IERR + 256
      IF (NBLOCK.LT.1) IERR = IERR + 512
      IF (IERR.NE.0) RETURN
C
      SMALL = NVAL.LT.0
C
C ------------------------------------------------------------------
C
C THIS SECTION SORTS AND ORTHONORMALIZES THE USER SUPPLIED VECTORS.
C IF A USER SUPPLIED VECTOR IS ZERO OR IF DSIGNIFICANT CANCELLATION
C OCCURS IN THE ORTHOGONALIZATION PROCESS THEN IERR IS SET TO  -1
C AND DNLASO TERMINATES.
C
      IF (NPERM.EQ.0) GO TO 110
C
C THIS NEGATES THE USER SUPPLIED EIGENVALUES WHEN THE LARGEST
C EIGENVALUES ARE DESIRED, SINCE DNWLA WILL IMPLICITLY USE THE
C NEGATIVE OF THE MATRIX.
C
      IF (SMALL) GO TO 20
      DO 10 I=1,NPERM
         VAL(I,1) = -VAL(I,1)
   10 CONTINUE
C
C THIS SORTS THE USER SUPPLIED VALUES AND VECTORS.
C
   20 CALL DVSORT(NPERM, VAL, VAL(1,2), 0, TARR, NMVEC, N, VEC)
C
C THIS STORES THE NORMS OF THE VECTORS FOR LATER COMPARISON.
C IT ALSO INSURES THAT THE RESIDUAL NORMS ARE POSITIVE.
C
      DO 60 I=1,NPERM
         VAL(I,2) = DABS(VAL(I,2))
         VAL(I,3) = DNRM2(N,VEC(1,I),1)
   60 CONTINUE
C
C THIS PERFORMS THE ORTHONORMALIZATION.
C
      M = N*NBLOCK + 1
      CALL DORTQR(NMVEC, N, NPERM, VEC, WORK(M))
      M = N*NBLOCK - NPERM
      DO 70 I = 1, NPERM
         M = M + NPERM + 1
         IF(DABS(WORK(M)) .GT. 0.9*VAL(I,3)) GO TO 70
         IERR = -1
         RETURN
C
   70 CONTINUE
C
C THIS COPIES THE RESIDUAL NORMS INTO THE CORRECT LOCATIONS IN
C THE ARRAY WORK FOR LATER REFERENCE IN DNWLA.
C
      M = 2*N*NBLOCK + 1
      CALL DCOPY(NPERM, VAL(1,2), 1, WORK(M), 1)
C
C THIS SETS EPS TO AN APPROXIMATION OF THE RELATIVE MACHINE
C PRECISION
C
C ***THIS SHOULD BE REPLACED BY AN ASDSIGNMENT STATEMENT
C ***IN A PRODUCTION CODE
C
  110 EPS = 1.0D0
      DO 120 I = 1,1000
         EPS = 0.5D0*EPS
         TEMP = 1.0D0 + EPS
         IF(TEMP.EQ.1.0D0) GO TO 130
  120 CONTINUE
C
C ------------------------------------------------------------------
C
C THIS SECTION CALLS DNWLA WHICH IMPLEMENTS THE LANCZOS ALGORITHM
C WITH SELECTIVE ORTHOGONALIZATION.
C
  130 NBAND = NBLOCK + 1
      I1 = 1 + N*NBLOCK
      I2 = I1 + N*NBLOCK
      I3 = I2 + NV
      I4 = I3 + NV
      I5 = I4 + NV
      I6 = I5 + MAXJ*NBAND
      I7 = I6 + NBLOCK*NBLOCK
      I8 = I7 + NBLOCK*NBLOCK
      I9 = I8 + MAXJ*(NV+1)
      I10 = I9 + NBLOCK
      I11 = I10 + 2*NV + 6
      I12 = I11 + MAXJ*(2*NBLOCK+1)
      I13 = I12 + MAXJ
      CALL DNWLA(OP, IOVECT, N, NBAND, NV, NFIG, NPERM, VAL, NMVEC,
     *   VEC, NBLOCK, MAXOP, MAXJ, NOP, WORK(1), WORK(I1),
     *   WORK(I2), WORK(I3), WORK(I4), WORK(I5), WORK(I6),
     *   WORK(I7), WORK(I8), WORK(I9), WORK(I10), WORK(I11),
     *   WORK(I12), WORK(I13), IND, SMALL, RARITZ, DELTA, EPS, IERR)
C
C ------------------------------------------------------------------
C
C THIS SECTION CALLS DNPPLA (THE POST PROCESSOR).
C
      IF (NPERM.EQ.0) GO TO 140
      I1 = N*NBLOCK + 1
      I2 = I1 + NPERM*NPERM
      I3 = I2 + NPERM*NPERM
      I4 = I3 + MAX0(N*NBLOCK,2*NPERM*NPERM)
      I5 = I4 + N*NBLOCK
      I6 = I5 + 2*NPERM + 4
      CALL DNPPLA(OP, IOVECT, N, NPERM, NOP, NMVAL, VAL, NMVEC,
     *  VEC, NBLOCK, WORK(I1), WORK(I2), WORK(I3), WORK(I4),
     *  WORK(I5), WORK(I6), DELTA, SMALL, RARITZ, EPS)
C
  140 IND(1) = NOP
      RETURN
      END
C
C ------------------------------------------------------------------
C
      SUBROUTINE DNWLA(OP, IOVECT, N, NBAND, NVAL, NFIG, NPERM, VAL,
     *   NMVEC, VEC, NBLOCK, MAXOP, MAXJ, NOP, P1, P0,
     *   RES, TAU, OTAU, T, ALP, BET, S, P2, BOUND, ATEMP, VTEMP, D,
     *   IND, SMALL, RARITZ, DELTA, EPS, IERR)
C
      INTEGER N, NBAND, NVAL, NFIG, NPERM, NMVEC, NBLOCK, MAXOP, MAXJ,
     *   NOP, IND(1), IERR
      LOGICAL RARITZ, SMALL
      DOUBLE PRECISION VAL(1), VEC(NMVEC,1), P0(N,1), P1(N,1),
     *   P2(N,1), RES(1), TAU(1), OTAU(1), T(NBAND,1),
     *   ALP(NBLOCK,1), BET(NBLOCK,1), BOUND(1), ATEMP(1),
     *   VTEMP(1), D(1), S(MAXJ,1), DELTA, EPS
      EXTERNAL OP, IOVECT
C
C DNWLA IMPLEMENTS THE LANCZOS ALGORITHM WITH SELECTIVE
C ORTHOGONALIZATION.
C
C   NBAND  NBLOCK + 1  THE BAND WIDTH OF T.
C
C   NVAL   THE NUMBER OF DESIRED EIGENVALUES.
C
C   NPERM   THE NUMBER OF PERMANENT VECTORS (THOSE EIGENVECTORS
C     INPUT BY THE USER OR THOSE EIGENVECTORS SAVED WHEN THE
C     ALGORITHM IS ITERATED).  PERMANENT VECTORS ARE ORTHOGONAL
C     TO THE CURRENT KRYLOV SUBSPACE.
C
C   NOP   THE NUMBER OF CALLS TO OP.
C
C   P0, P1, AND P2   THE CURRENT BLOCKS OF LANCZOS VECTORS.
C
C   RES   THE (APPROXIMATE) RESIDUAL NORMS OF THE PERMANENT VECTORS.
C
C   TAU AND OTAU   USED TO MONITOR THE NEED FOR ORTHOGONALIZATION.
C
C   T   THE BAND MATRIX.
C
C   ALP   THE CURRENT DIAGONAL BLOCK.
C
C   BET   THE CURRENT OFF DIAGONAL BLOCK.
C
C   BOUND, ATEMP, VTEMP, D  TEMPORARY STORAGE USED BY THE BAND
C      EIGENVALUE SOLVER DLAEIG.
C
C   S   EIGENVECTORS OF T.
C
C   SMALL   .TRUE.  IF THE SMALL EIGENVALUES ARE DESIRED.
C
C   RARITZ  RETURNED AS  .TRUE.  IF A FINAL RAYLEIGH-RITZ PROCEDURE
C     IS TO BE DONE.
C
C   DELTA   RETURNED AS THE VALUE OF THE (NVAL+1)TH EIGENVALUE
C     OF THE MATRIX.  USED IN ESTIMATING THE ACCURACY OF THE
C     COMPUTED EIGENVALUES.
C
C
C INTERNAL VARIABLES USED  
C
      INTEGER I, I1, II, J, K, L, M, NG, NGOOD,
     *   NLEFT, NSTART, NTHETA, NUMBER, MIN0, MTEMP
      LOGICAL ENOUGH, TEST
      DOUBLE PRECISION ALPMAX, ALPMIN, ANORM, BETMAX, BETMIN,
     *   EPSRT, PNORM, RNORM, TEMP,
     *   TMAX, TMIN, TOLA, TOLG, UTOL, DABS,
     *   DMAX1, DMIN1, DSQRT, DDOT, DNRM2, TARR(1), DZERO(1)
      EXTERNAL DMVPC, DORTQR, DAXPY, DCOPY, DDOT,
     *   DNRM2, DSCAL, DLAEIG, DLAGER, DLARAN, DVSORT
C
C J   THE CURRENT DIMENSION OF T.  (THE DIMENSION OF THE CURRENT
C    KRYLOV SUBSPACE. 
C
C NGOOD   THE NUMBER OF GOOD RITZ VECTORS (GOOD VECTORS
C    LIE IN THE CURRENT KRYLOV SUBSPACE).
C
C NLEFT   THE NUMBER OF VALUES WHICH REMAIN TO BE DETERMINED,
C    I.E.  NLEFT = NVAL - NPERM.
C
C NUMBER = NPERM + NGOOD.
C
C ANORM   AN ESTIMATE OF THE NORM OF THE MATRIX.
C
C EPS   THE RELATIVE MACHINE PRECISION.
C
C UTOL   THE USER TOLERANCE.
C
C TARR  AN ARRAY OF LENGTH ONE USED TO INSURE TYPE CONSISTENCY IN CALLS TO
C       DLAEIG
C
C DZERO AN ARRAY OF LENGTH ONE CONTAINING DZERO, USED TO INSURE TYPE
C       CONSISTENCY IN CALLS TO DCOPY
C
      DZERO(1) = 0.0D0
      RNORM = 0.0D0
      IF (NPERM.NE.0) RNORM = DMAX1(-VAL(1),VAL(NPERM))
      PNORM = RNORM
      DELTA = 10.D30
      EPSRT = DSQRT(EPS)
      NLEFT = NVAL - NPERM
      NOP = 0
      NUMBER = NPERM
      RARITZ = .FALSE.
      UTOL = DMAX1(DBLE(FLOAT(N))*EPS,10.0D0**DBLE((-FLOAT(NFIG))))
      J = MAXJ
C
C ------------------------------------------------------------------
C
C ANY ITERATION OF THE ALGORITHM BEGINS HERE.
C
   30 DO 50 I=1,NBLOCK
         TEMP = DNRM2(N,P1(1,I),1)
         IF (TEMP.EQ.0D0) CALL DLARAN(N, P1(1,I))
   50 CONTINUE
      IF (NPERM.EQ.0) GO TO 70
      DO 60 I=1,NPERM
         TAU(I) = 1.0D0
         OTAU(I) = 0.0D0
   60 CONTINUE
   70 CALL DCOPY(N*NBLOCK, DZERO, 0, P0, 1)
      CALL DCOPY(NBLOCK*NBLOCK, DZERO, 0, BET, 1)
      CALL DCOPY(J*NBAND, DZERO, 0, T, 1)
      MTEMP = NVAL + 1
      DO 75 I = 1, MTEMP
         CALL DCOPY(J, DZERO, 0, S(1,I), 1)
   75 CONTINUE
      NGOOD = 0
      TMIN = 1.0D30
      TMAX = -1.0D30
      TEST = .TRUE.
      ENOUGH = .FALSE.
      BETMAX = 0.0D0
      J = 0
C
C ------------------------------------------------------------------
C
C THIS SECTION TAKES A SINGLE BLOCK LANCZOS STEP.
C
   80 J = J + NBLOCK
C
C THIS IS THE SELECTIVE ORTHOGONALIZATION.
C
      IF (NUMBER.EQ.0) GO TO 110
      DO 100 I=1,NUMBER
         IF (TAU(I).LT.EPSRT) GO TO 100
         TEST = .TRUE.
         TAU(I) = 0.0D0
         IF (OTAU(I).NE.0.0D0) OTAU(I) = 1.0D0
         DO 90 K=1,NBLOCK
            TEMP = -DDOT(N,VEC(1,I),1,P1(1,K),1)
            CALL DAXPY(N, TEMP, VEC(1,I), 1, P1(1,K), 1)
C
C THIS CHECKS FOR TOO GREAT A LOSS OF ORTHOGONALITY BETWEEN A
C NEW LANCZOS VECTOR AND A GOOD RITZ VECTOR.  THE ALGORITHM IS
C TERMINATED IF TOO MUCH ORTHOGONALITY IS LOST.
C
            IF (DABS(TEMP*BET(K,K)).GT.DBLE(FLOAT(N))*EPSRT*
     *        ANORM .AND. I.GT.NPERM) GO TO 380
   90    CONTINUE
  100 CONTINUE
C
C IF NECESSARY, THIS REORTHONORMALIZES P1 AND UPDATES BET.
C
  110 IF(.NOT. TEST) GO TO 160
      CALL DORTQR(N, N, NBLOCK, P1, ALP)
      TEST = .FALSE.
      IF(J .EQ. NBLOCK) GO TO 160
      DO 130 I = 1,NBLOCK
         IF(ALP(I,I) .GT. 0.0D0) GO TO 130
         M = J - 2*NBLOCK + I
         L = NBLOCK + 1
         DO 120 K = I,NBLOCK
            BET(I,K) = -BET(I,K)
            T(L,M) = -T(L,M)
            L = L - 1
            M = M + 1
  120    CONTINUE
  130 CONTINUE
C
C THIS IS THE LANCZOS STEP.
C
  160 CALL OP(N, NBLOCK, P1, P2)
      NOP = NOP + 1
      CALL IOVECT(N, NBLOCK, P1, J, 0)
C
C THIS COMPUTES P2=P2-P0*BET(TRANSPOSE)
C
      DO 180 I=1,NBLOCK
         DO 170 K=I,NBLOCK
            CALL DAXPY(N, -BET(I,K), P0(1,K), 1, P2(1,I), 1)
  170    CONTINUE
  180 CONTINUE
C
C THIS COMPUTES ALP AND P2=P2-P1*ALP.
C
      DO 200 I=1,NBLOCK
         DO 190 K=1,I
            II = I - K + 1
            ALP(II,K) = DDOT(N,P1(1,I),1,P2(1,K),1)
            CALL DAXPY(N, -ALP(II,K), P1(1,I), 1, P2(1,K), 1)
            IF (K.NE.I) CALL DAXPY(N, -ALP(II,K), P1(1,K),
     *        1, P2(1,I), 1)
  190   CONTINUE
  200 CONTINUE
C
C  REORTHOGONALIZATION OF THE SECOND BLOCK
C
      IF(J .NE. NBLOCK) GO TO 220
      DO 215 I=1,NBLOCK
         DO 210 K=1,I
            TEMP = DDOT(N,P1(1,I),1,P2(1,K),1)
            CALL DAXPY(N, -TEMP, P1(1,I), 1, P2(1,K), 1)
            IF (K.NE.I) CALL DAXPY(N, -TEMP, P1(1,K),
     *        1, P2(1,I), 1)
            II = I - K + 1
            ALP(II,K) = ALP(II,K) + TEMP            
  210   CONTINUE
  215 CONTINUE
C
C THIS ORTHONORMALIZES THE NEXT BLOCK
C
  220 CALL DORTQR(N, N, NBLOCK, P2, BET)
C
C THIS STORES ALP AND BET IN T.
C
      DO 250 I=1,NBLOCK
         M = J - NBLOCK + I
         DO 230 K=I,NBLOCK
            L = K - I + 1
            T(L,M) = ALP(L,I)
  230    CONTINUE
         DO 240 K=1,I
            L = NBLOCK - I + K + 1
            T(L,M) = BET(K,I)
  240    CONTINUE
  250 CONTINUE
C
C THIS NEGATES T IF SMALL IS FALSE.
C
      IF (SMALL) GO TO 280
      M = J - NBLOCK + 1
      DO 270 I=M,J
         DO 260 K=1,L
            T(K,I) = -T(K,I)
  260    CONTINUE
  270 CONTINUE
C
C THIS SHIFTS THE LANCZOS VECTORS
C
  280 CALL DCOPY(NBLOCK*N, P1, 1, P0, 1)
      CALL DCOPY(NBLOCK*N, P2, 1, P1, 1)
      CALL DLAGER(J, NBAND, J-NBLOCK+1, T, TMIN, TMAX)
      ANORM = DMAX1(RNORM, TMAX, -TMIN)
      IF (NUMBER.EQ.0) GO TO 305
C
C THIS COMPUTES THE EXTREME EIGENVALUES OF ALP.
C
      CALL DCOPY(NBLOCK, DZERO, 0, P2, 1)
      CALL DLAEIG(NBLOCK, NBLOCK, 1, 1, ALP, TARR, NBLOCK,
     1   P2, BOUND, ATEMP, D, VTEMP, EPS, TMIN, TMAX)
      ALPMIN = TARR(1)
      CALL DCOPY(NBLOCK, DZERO, 0, P2, 1)
      CALL DLAEIG(NBLOCK, NBLOCK, NBLOCK, NBLOCK, ALP, TARR,
     1  NBLOCK, P2, BOUND, ATEMP, D, VTEMP, EPS, TMIN, TMAX)
      ALPMAX = TARR(1)
C
C THIS COMPUTES ALP = BET(TRANSPOSE)*BET.
C
  305 DO 310 I = 1, NBLOCK
         DO 300 K = 1, I
            L = I - K + 1
            ALP(L,K) = DDOT(NBLOCK-I+1, BET(I,I), NBLOCK, BET(K,I),
     1        NBLOCK)
  300    CONTINUE
  310 CONTINUE
      IF(NUMBER .EQ. 0) GO TO 330
C
C THIS COMPUTES THE SMALLEST SINGULAR VALUE OF BET.
C
      CALL DCOPY(NBLOCK, DZERO, 0, P2, 1)
      CALL DLAEIG(NBLOCK, NBLOCK, 1, 1, ALP, TARR, NBLOCK,
     1  P2, BOUND, ATEMP, D, VTEMP, EPS, 0.0D0, ANORM*ANORM)
      BETMIN = DSQRT(TARR(1))
C
C THIS UPDATES TAU AND OTAU.
C
      DO 320 I=1,NUMBER
         TEMP = (TAU(I)*DMAX1(ALPMAX-VAL(I),VAL(I)-ALPMIN)
     *     +OTAU(I)*BETMAX+EPS*ANORM)/BETMIN
         IF (I.LE.NPERM) TEMP = TEMP + RES(I)/BETMIN
         OTAU(I) = TAU(I)
         TAU(I) = TEMP
  320 CONTINUE
C
C THIS COMPUTES THE LARGEST SINGULAR VALUE OF BET.
C
  330 CALL DCOPY(NBLOCK, DZERO, 0, P2, 1)
      CALL DLAEIG(NBLOCK, NBLOCK, NBLOCK, NBLOCK, ALP, TARR,
     1  NBLOCK, P2, BOUND, ATEMP, D, VTEMP, EPS, 0.0D0,
     2  ANORM*ANORM)
      BETMAX = DSQRT(TARR(1))
      IF (J.LE.2*NBLOCK) GO TO 80
C
C ------------------------------------------------------------------
C
C THIS SECTION COMPUTES AND EXAMINES THE SMALLEST NONGOOD AND
C LARGEST DESIRED EIGENVALUES OF T TO SEE IF A CLOSER LOOK
C IS JUSTIFIED.
C
      TOLG = EPSRT*ANORM
      TOLA = UTOL*RNORM
      IF(MAXJ-J .LT. NBLOCK .OR. (NOP .GE. MAXOP .AND.
     1  NLEFT .NE. 0)) GO TO 390
      GO TO 400

C
C ------------------------------------------------------------------
C
C THIS SECTION COMPUTES SOME EIGENVALUES AND EIGENVECTORS OF T TO
C SEE IF FURTHER ACTION IS INDICATED, ENTRY IS AT 380 OR 390 IF AN
C ITERATION (OR TERMINATION) IS KNOWN TO BE NEEDED, OTHERWISE ENTRY
C IS AT 400.
C
  380 J = J - NBLOCK
      IERR = -8
  390 IF (NLEFT.EQ.0) RETURN
      TEST = .TRUE.
  400 NTHETA = MIN0(J/2,NLEFT+1)
      CALL DLAEIG(J, NBAND, 1, NTHETA, T, VAL(NUMBER+1),
     1  MAXJ, S, BOUND, ATEMP, D, VTEMP, EPS, TMIN, TMAX)
      CALL DMVPC(NBLOCK, BET, MAXJ, J, S, NTHETA, ATEMP, VTEMP, D)
C
C THIS CHECKS FOR TERMINATION OF A CHECK RUN
C
      IF(NLEFT .NE. 0 .OR. J .LT. 6*NBLOCK) GO TO 410
      IF(VAL(NUMBER+1)-ATEMP(1) .GT. VAL(NPERM) - TOLA) GO TO 790 
C
C THIS UPDATES NLEFT BY EXAMINING THE COMPUTED EIGENVALUES OF T
C TO DETERMINE IF SOME PERMANENT VALUES ARE NO LONGER DESIRED.
C
 410  IF (NTHETA.LE.NLEFT) GO TO 470
      IF (NPERM.EQ.0) GO TO 430
      M = NUMBER + NLEFT + 1
      IF (VAL(M).GE.VAL(NPERM)) GO TO 430
      NPERM = NPERM - 1
      NGOOD = 0
      NUMBER = NPERM
      NLEFT = NLEFT + 1
      GO TO 400
C
C THIS UPDATES DELTA.
C
  430 M = NUMBER + NLEFT + 1
      DELTA = DMIN1(DELTA,VAL(M))
      ENOUGH = .TRUE.
      IF(NLEFT .EQ. 0) GO TO 80
      NTHETA = NLEFT
      VTEMP(NTHETA+1) = 1
C
C ------------------------------------------------------------------
C
C THIS SECTION EXAMINES THE COMPUTED EIGENPAIRS IN DETAIL.
C
C THIS CHECKS FOR ENOUGH ACCEPTABLE VALUES.
C
      IF (.NOT.(TEST .OR. ENOUGH)) GO TO 470
      DELTA = DMIN1(DELTA,ANORM)
      PNORM = DMAX1(RNORM,DMAX1(-VAL(NUMBER+1),DELTA))
      TOLA = UTOL*PNORM
      NSTART = 0
      DO 460 I=1,NTHETA
         M = NUMBER + I
         IF (DMIN1(ATEMP(I)*ATEMP(I)/(DELTA-VAL(M)),ATEMP(I))
     *     .GT.TOLA) GO TO 450
         IND(I) = -1
         GO TO 460
C
  450    ENOUGH = .FALSE.
         IF (.NOT.TEST) GO TO 470
         IND(I) = 1
         NSTART = NSTART + 1
  460 CONTINUE
C
C  COPY VALUES OF IND INTO VTEMP
C
      DO 465 I = 1,NTHETA
         VTEMP(I) = DBLE(FLOAT(IND(I)))
  465 CONTINUE
      GO TO 500
C
C THIS CHECKS FOR NEW GOOD VECTORS.
C
  470 NG = 0
      DO 490 I=1,NTHETA
         IF (VTEMP(I).GT.TOLG) GO TO 480
         NG = NG + 1
         VTEMP(I) = -1
         GO TO 490
C
  480    VTEMP(I) = 1
  490 CONTINUE
C
      IF (NG.LE.NGOOD) GO TO 80
      NSTART = NTHETA - NG
C
C ------------------------------------------------------------------
C
C THIS SECTION COMPUTES AND NORMALIZES THE INDICATED RITZ VECTORS.
C IF NEEDED (TEST = .TRUE.), NEW STARTING VECTORS ARE COMPUTED.
C
  500 TEST = TEST .AND. .NOT.ENOUGH
      NGOOD = NTHETA - NSTART
      NSTART = NSTART + 1
      NTHETA = NTHETA + 1
C
C THIS ALIGNS THE DESIRED (ACCEPTABLE OR GOOD) EIGENVALUES AND
C EIGENVECTORS OF T.  THE OTHER EIGENVECTORS ARE SAVED FOR
C FORMING STARTING VECTORS, IF NECESSARY.  IT ALSO SHIFTS THE
C EIGENVALUES TO OVERWRITE THE GOOD VALUES FROM THE PREVIOUS
C PAUSE.
C
      CALL DCOPY(NTHETA, VAL(NUMBER+1), 1, VAL(NPERM+1), 1)
      IF (NSTART.EQ.0) GO TO 580
      IF (NSTART.EQ.NTHETA) GO TO 530
      CALL DVSORT(NTHETA, VTEMP, ATEMP, 1, VAL(NPERM+1), MAXJ,
     *  J, S)
C
C THES ACCUMULATES THE J-VECTORS USED TO FORM THE STARTING
C VECTORS.
C
  530 IF (.NOT.TEST) NSTART = 0
      IF (.NOT.TEST) GO TO 580
C
C  FIND MINIMUM ATEMP VALUE TO AVOID POSSIBLE OVERFLOW
C
      TEMP = ATEMP(1)
      DO 535 I = 1, NSTART
         TEMP = DMIN1(TEMP, ATEMP(I))
  535 CONTINUE
      M = NGOOD + 1
      L = NGOOD + MIN0(NSTART,NBLOCK)
      DO 540 I=M,L
         CALL DSCAL(J, TEMP/ATEMP(I), S(1,I), 1)
  540 CONTINUE
      M = (NSTART-1)/NBLOCK
      IF (M.EQ.0) GO TO 570
      L = NGOOD + NBLOCK
      DO 560 I=1,M
         DO 550 K=1,NBLOCK
            L = L + 1
            IF (L.GT.NTHETA) GO TO 570
            I1 = NGOOD + K
            CALL DAXPY(J, TEMP/ATEMP(L), S(1,L), 1, S(1,I1), 1)
  550    CONTINUE
  560 CONTINUE
  570 NSTART = MIN0(NSTART,NBLOCK)
C
C THIS STORES THE RESIDUAL NORMS OF THE NEW PERMANENT VECTORS.
C
  580 IF (NGOOD.EQ.0 .OR. .NOT.(TEST .OR. ENOUGH)) GO TO 600
      DO 590 I=1,NGOOD
         M = NPERM + I
         RES(M) = ATEMP(I)
  590 CONTINUE
C
C THIS COMPUTES THE RITZ VECTORS BY SEQUENTIALLY RECALLING THE
C LANCZOS VECTORS.
C
  600 NUMBER = NPERM + NGOOD
      IF (TEST .OR. ENOUGH) CALL DCOPY(N*NBLOCK, DZERO, 0, P1, 1)
      IF (NGOOD.EQ.0) GO TO 620
      M = NPERM + 1
      DO 610 I=M,NUMBER
         CALL DCOPY(N, DZERO, 0, VEC(1,I), 1)
  610 CONTINUE
  620 DO 670 I=NBLOCK,J,NBLOCK
         CALL IOVECT(N, NBLOCK, P2, I, 1)
         DO 660 K=1,NBLOCK
            M = I - NBLOCK + K
            IF (NSTART.EQ.0) GO TO 640
            DO 630 L=1,NSTART
               I1 = NGOOD + L
               CALL DAXPY(N, S(M,I1), P2(1,K), 1, P1(1,L), 1)
  630       CONTINUE
  640       IF (NGOOD.EQ.0) GO TO 660
            DO 650 L=1,NGOOD
               I1 = L + NPERM
               CALL DAXPY(N, S(M,L), P2(1,K), 1, VEC(1,I1), 1)
  650       CONTINUE
  660    CONTINUE
  670 CONTINUE
      IF (TEST .OR. ENOUGH) GO TO 690
C
C THIS NORMALIZES THE RITZ VECTORS AND INITIALIZES THE
C TAU RECURRENCE.
C
      M = NPERM + 1
      DO 680 I=M,NUMBER
         TEMP = 1.0D0/DNRM2(N,VEC(1,I),1)
         CALL DSCAL(N, TEMP, VEC(1,I), 1)
         TAU(I) = 1.0D0
         OTAU(I) = 1.0D0
  680 CONTINUE
C
C  SHIFT S VECTORS TO ALIGN FOR LATER CALL TO DLAEIG
C
      CALL DCOPY(NTHETA, VAL(NPERM+1), 1, VTEMP, 1)
      CALL DVSORT(NTHETA, VTEMP, ATEMP, 0, TARR, MAXJ, J, S)
      GO TO 80
C
C ------------------------------------------------------------------
C
C THIS SECTION PREPARES TO ITERATE THE ALGORITHM BY SORTING THE
C PERMANENT VALUES, RESETTING SOME PARAMETERS, AND ORTHONORMALIZING
C THE PERMANENT VECTORS.
C
  690 IF (NGOOD.EQ.0 .AND. NOP.GE.MAXOP) GO TO 810
      IF (NGOOD.EQ.0) GO TO 30
C
C THIS ORTHONORMALIZES THE VECTORS
C
      CALL DORTQR(NMVEC, N, NPERM+NGOOD, VEC, S)
C
C THIS SORTS THE VALUES AND VECTORS.
C
      IF(NPERM .NE. 0) CALL DVSORT(NPERM+NGOOD, VAL, RES, 0, TEMP,
     *   NMVEC, N, VEC)
      NPERM = NPERM + NGOOD
      NLEFT = NLEFT - NGOOD
      RNORM = DMAX1(-VAL(1),VAL(NPERM))
C
C THIS DECIDES WHERE TO GO NEXT.
C
      IF (NOP.GE.MAXOP .AND. NLEFT.NE.0) GO TO 810
      IF (NLEFT.NE.0) GO TO 30
      IF (VAL(NVAL)-VAL(1).LT.TOLA) GO TO 790
C
C THIS DOES A CLUSTER TEST TO SEE IF A CHECK RUN IS NEEDED
C TO LOOK FOR UNDISCLOSED MULTIPLICITIES.
C
      M = NPERM - NBLOCK + 1
      IF (M.LE.0) RETURN
      DO 780 I=1,M
         L = I + NBLOCK - 1
         IF (VAL(L)-VAL(I).LT.TOLA) GO TO 30
  780 CONTINUE
C
C THIS DOES A CLUSTER TEST TO SEE IF A FINAL RAYLEIGH-RITZ
C PROCEDURE IS NEEDED.
C
  790 M = NPERM - NBLOCK
      IF (M.LE.0) RETURN
      DO 800 I=1,M
         L = I + NBLOCK
         IF (VAL(L)-VAL(I).GE.TOLA) GO TO 800
         RARITZ = .TRUE.
         RETURN
  800 CONTINUE
C
      RETURN
C
C ------------------------------------------------------------------
C
C THIS REPORTS THAT MAXOP WAS EXCEEDED.
C
  810 IERR = -2
      GO TO 790
C
      END
