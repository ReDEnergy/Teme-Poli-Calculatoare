/**
 * Matrix Market usage
 */

 #include "mmio.h"

double *E;		// Matrix Elements
int *I;			// row vector for reading
int *J;			// column vector for reading
int nr_elem;	// number of nonzero elements	

/**
 * Read a matrix from MatrixMarket using mmio.c
 */
void readMatrix(const char *filename) {
	FILE *f = fopen(filename, "r");

	if (f == NULL) {
		printf("Error while oppening file: %s\n", filename);
		exit(1);
	}
	mm_read_unsymmetric_sparse(filename, &N, &N, &nr_elem, &E, &I, &J);
}

void setMatrix() {
	int i;
	for (i=0; i<nr_elem; i++) {
		setMatrixValueAt(I[i], J[i], E[i]);
		setMatrixValueAt(J[i], I[i], E[i]);
	}
}

readMatrix(argv[1]);
getDsymvOperands(N);
setMatrix();



/**
 * Call the optimized implementation of DSYMV function
 * Y:= alpha * A * X + beta * Y
 */
double* computeOptimezedDsymv(double * const Y) {

	// Accumulator
	register double acm;

	double *YY = (double*) calloc(N, sizeof(double));
	memcpy(YY, Y, N * sizeof(double));

	double *ptrIdxYY = YY;				/* pointer to YY */
	double *ptrIdxX;					/* pointer to array X */
	double *ptrIdxA = A;				/* pointer to matrix A */
	double *ptrEndA = A + N * N;		/* pointer to last elem from A */
	double *ptrEndX = X + N;			/* pointer to last elem from X */
	double *ptrEndYY = ptrIdxYY + N;	/* pointer to last elem from Y */

	// Y = beta * Y using pointers
	for (; ptrIdxYY != ptrEndYY; ptrIdxYY++)
		*ptrIdxYY *= beta;

	// Reset pointer
	ptrIdxYY = YY;

	while (ptrIdxA != ptrEndA) {
		acm = 0;
		for(ptrIdxX = X; ptrIdxX != ptrEndX; ptrIdxX++) {
			acm += alpha * (*ptrIdxA) * (*ptrIdxX);
			ptrIdxA++;
		}
		(*ptrIdxYY) += acm;
		ptrIdxYY++;
	}
	return YY;
}

/*
if (argv[2]) {
	printResult(YSIMPLE, argv[2], "SIMPLE");
	printResult(YBLAS, argv[2], "ATLAS");
	printResult(YOPTIM, argv[2], "OPTIM");
}
*/