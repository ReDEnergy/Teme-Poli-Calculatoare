/*
 * Ivanica Gabriel 333CA
 * Tema2 ASC
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#include "cblas.h"

int N;			// number of rows/columns
double alpha;	// scalar
double beta;	// scalar
double *A;		// Symetric Matrix
double *X;		// X vector
double *Y;		// Y vector

const long long EPS = 16 * 1024 * 1024;
const unsigned long long COMPLEMENT2 = 0x8000000000000000;

/* Start timer */
clock_t startTime() {
	return clock();
}

/** 
 * Get time elapsed between reference and curent time
 */
 int getElapsedTime(clock_t reference) {
	clock_t diff = clock() - reference;
	int msec = diff * 1000 / CLOCKS_PER_SEC;
	return msec;
}

/**
 * Allocate memory for matrix , and X and Y arrays
 */
void allocMemory(int size) {
	X = (double*) calloc(size, sizeof(double));
	Y = (double*) calloc(size, sizeof(double));
	A = (double*) calloc(size * size, sizeof(double));
}

/**
 * Free memory
 */
void clearMemory() {
	free(A);
	free(X);
	free(Y);
}

/**
 * Generate a new random double value
 */
inline double getRand() {
	double val;
	val = (double) rand() / RAND_MAX;
	val = (double) LONG_MIN  + val * ((double)LONG_MAX - LONG_MIN);
	return (double) val;
}

/**
 * Get the value from the matrix at row-col pozition
 */
inline double getMatrixValueAt(int row, int column) {
	return A[row * N + column];
}

/**
 * Set the value in the matrix at row-col pozition
 */
inline void setMatrixValueAt(int row, int column, double value) {
	A[row * N + column] = value;
}

void genMatrix(int size) {
	int i, j;
	for (i=0; i<size; i++) {
		for (j=0; j<=i; j++) {
			double val = getRand();
			setMatrixValueAt(i, j, val);
			setMatrixValueAt(j, i, val);
		}
	}
}

/**
 * Generate the operands needed for BLAS DSYMV computing
 * Y:= alpha * A * X + beta * Y
 */
void getDsymvOperands(int size) {
	int i;

	allocMemory(size);

	for (i=0; i<size; i++) {
		X[i] = getRand();
		Y[i] = getRand();
	}

	genMatrix(N);

	alpha = getRand();
	beta  = getRand();
}

/**
 * Call the atlas DSYMV function from cblas.h
 * Y:= alpha * A * X + beta * Y
 */
double* atlasDsymv(const double* const Y) {

	double *YY = (double*) calloc(N, sizeof(double));
	memcpy(YY, Y, N * sizeof(double));

	cblas_dsymv(CblasRowMajor, CblasUpper, N, alpha, A, N, X, 1, beta, YY, 1);
	
	return YY;
}

/**
 * Call the simple DSYMV function
 * Y:= alpha * A * X + beta * Y
 */
double* computeSimpleDsymv(const double * const Y) {
	int i, j;
	double acm;
	double *YY = (double*) calloc(N, sizeof(double));

	for (i=0; i<N; i++) {
		acm = 0;
		for(j=0; j<N; j++)
			acm += alpha * getMatrixValueAt(i, j) * X[j];

		YY[i] = acm + beta * Y[i];
	}

	return YY;
}

/**
 * Call the optimized implementation of DSYMV function
 * Y:= alpha * A * X + beta * Y
 */
double* computeOptimezedDsymv(double * const Y) {
	int i, j;
	// Accumulator
	register double acm;

	// Copy X and Y
	double *XX = (double*) malloc(N * sizeof(double));
	double *YY = (double*) malloc(N * sizeof(double));
	memcpy(XX, X, N * sizeof(double));
	memcpy(YY, Y, N * sizeof(double));

	double *ptrIdxA = A;		/* pointer to matrix A */
	double *ptrIdxXX = XX;		/* pointer to array X */
	double *ptrIdxYY = YY;		/* pointer to array YY */
	double *ptrEndXX = XX + N;	/* pointer to last elem from X */

	// YY = beta * YY using pointers
	// XX = alfa * XX using pointers
	for (; ptrIdxXX != ptrEndXX; ptrIdxXX++) {
		*ptrIdxYY *= beta;
		*ptrIdxXX *= alpha;
		ptrIdxYY++;
	}

	// Access elements from matrix A using pointers
	// Access just the UPPER part of the matrix
	for (i=0; i<N; i++) {
		ptrIdxA += i;						/* jump to A[i][i] */
		acm = *ptrIdxA * XX[i];				/* add the elem from A[i][i] */
		ptrIdxA +=1;						/* jump to next element */
		for (j=i+1; j<N; j++) {
			YY[j] += *ptrIdxA * XX[i];		/* Y[j] += alfa * A[i][j] * X[i] */
			acm += *ptrIdxA * XX[j];		/* row * column starting from j */
			ptrIdxA++;						/* jump to next element from A */
		}
		YY[i] +=acm;						/* save the value */
	}

	free(XX);
	return YY;
}

/**
 * Compare 2 double values
 * return 0 if values differ less than EPS
 * return 1 if values differ more than EPS
 */
inline int compareDoubleEPS(double A, double B) {

	long long lngA, lngB, diff;

	lngA = *(long long*)&A;
	if (lngA < 0)
		lngA = COMPLEMENT2 - lngA;

	lngB = *(long long*)&B;
	if (lngB < 0)
		lngB = COMPLEMENT2 - lngB;

	diff = llabs(lngA - lngB);
	if (diff <= EPS)
		return 0;

	return 1;
}

/**
 * Compare 2 arrays of double values
 */
void compareResults(double *YA, double *YB) {
	int i;
	for (i=0; i<N; i++) {
		if (compareDoubleEPS(YA[i], YB[i])) {
			printf("Values differ at: %d\n", i);
			return;
		}
	}
	// printf("Results are EPS equal\n");
}

/**
 * Debug function for printing Y result
 */

void printResult(const double * const rez, char *cpu, char* test) {
	int i;
	char filename[50];
	sprintf(filename, "rez_%s_%s.txt", cpu, test);
	FILE *F = fopen(filename, "w");

	for (i=0; i<N; i++)
		fprintf(F, "%.20lg\n", rez[i]);

	fclose(F);
}


void printTaskInfo(char* test, char *arhitecture, int time) {
	printf("%s\t%s\t%d\t%d\n", test, arhitecture, N, time);
}

int main(int argc, char* argv[]) {

	int i;
	double *YBLAS;
	double *YSIMPLE;
	double *YOPTIM;
	long int job_time;

	clock_t start;
	if (argc < 2)
		printf("Usage: ./bin matrix_size\n");

	srand(time(NULL));

	N = atoi(argv[1]);

	for (i=1; i<9; i++) {
		N = 5000 * i;
		getDsymvOperands(N);

		start = startTime();
		YBLAS = atlasDsymv(Y);
		job_time = getElapsedTime(start);
		printTaskInfo("ATLAS", argv[2], job_time);

		start = startTime();
		YSIMPLE = computeSimpleDsymv(Y);
		job_time = getElapsedTime(start);
		printTaskInfo("SIMPLE", argv[2], job_time);

		start = startTime();
		YOPTIM = computeOptimezedDsymv(Y);
		job_time = getElapsedTime(start);
		printTaskInfo("OPTIM", argv[2], job_time);

		compareResults(YBLAS, YSIMPLE);
		compareResults(YBLAS, YOPTIM);

		free(YBLAS);
		free(YSIMPLE);
		free(YOPTIM);
		clearMemory();
	}

	return 0;
}
