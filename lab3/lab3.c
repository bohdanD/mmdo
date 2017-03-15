/*
 * lab3.c
 * 
 * Copyright 2017 fei-34dorosh <fei-34dorosh@class>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "stdio.h"

#include "malloc.h"

#include "math.h"

double *c, **a;

int originalN, originalM;

void addColumn(int i, double data, double cData, int *n, int m);

int* doNb(int n, int m);

void balans(int *znaky, int *n, int m);

double* doCb(double *c, int *n_b, int n);

double** inverse(double **A, int n);

double** matMulmat(double **a, double **b, int n, int m, int p);

double* vecMulmat(double *a, double **b, int m, int n);

double* matMulvec(double **a, double *b, int m, int n);

int argmin(double *a, int n);

double M(double *c, int *n);

int main(){

	originalM = 4;

	originalN = 5;

	int *znak, n = originalN, m = originalM;

//----------------------READ---------

	FILE *in = fopen("c.txt", "r");

	c = (double*) malloc((n + 1)*sizeof(double));

	for(int i = 1; i <= n; i++)

	fscanf(in, "%lf ", &c[i]);

	fclose(in);

	FILE *in2 = fopen("a.txt", "r");

	a = (double**) malloc((m + 1)*sizeof(double*));

	for(int i = 0; i <= m; i++)

	a[i] = (double*) malloc((n + 1)*sizeof(double));

	for(int i = 1; i <= m; i++)

		for(int j = 0; j <= n;j++)
	
			fscanf(in2, "%lf ", &a[i][j]);

	fclose(in2);

	FILE *in3 = fopen("znak.txt", "r");

	znak = (int*) malloc((m + 1)*sizeof(int));

	for(int i = 1; i <= m; i++)

		fscanf(in3, "%d ", &znak[i]);

	fclose(in3);

//--------------------balans------------------------

	for(int i = 1; i <= m; i++)

		if(a[i][0] < 0)

		for(int j = 0; j <= n; j++)

			a[i][j] *= -1;

	balans(znak, &n, m);

//---------------DO NB CB Binvers------------------

	int *Nb = doNb(n, m);

	double *Cb = doCb(c, Nb, m);

	double **B = (double **) malloc((m + 1)*sizeof(double*));

	for(int i = 1; i <= m; i++)

		B[i] = (double*) malloc((m + 1)*sizeof(double));

	for(int i = 1; i <= m; i++){

		for(int j = 1; j <= m; j++){

			B[j][i] = a[j][Nb[i]];

			printf("%.2lf\t", B[i][j]);

		}

		printf("\n");

	}	

	double **Binv = inverse(B, m);

	double *u, *delta, *Vec;

//---------------------------------------------------

	while(1){

	u = vecMulmat(Cb, Binv, m, m);
	
	delta = vecMulmat(u, a, m, n);

	printf("Delta:");

	for (int i = 1; i <= n; i++){

		delta[i] -= c[i];

		printf("%.2lf\t", delta[i]);

	}

	printf("\n");

	int isNoGood = 0;

	for(int i = 1; i <= n; i++)

		if(delta[i] < 0) isNoGood++;

	Vec = (double*)malloc((m + 1)*sizeof(double));

	for(int i = 1; i <= m; i++)

		Vec[i] = a[i][0];

	double *alpha0 = matMulvec(Binv, Vec, m, m);

	if(!isNoGood){

		printf("IS OPTIMAL\nAlpha: ");

	for(int i = 1; i <= m; i++){

		printf("%.2lf\t", alpha0[i]);

	}

	printf("\nNb: ");

	for(int i = 1; i <= m; i++){

		printf("%d\t", Nb[i]);

	}

	break;

	}

	int p = argmin(delta, n);

	for(int i = 1; i <= m; i++)

		Vec[i] = a[i][p];

	double *alphaP = matMulvec(Binv, Vec, m, m);

	free(Vec);

	int kk = 0;

	for(int i = 1; i <= m; i++)

		if(alphaP[i] > 0)

			kk++;

	if(!kk){

		printf("No result\n");

		break;

	}

	int k;

	double minValue = 999999;

	for(int i = 1; i <= m; i++)

		if(alphaP[i] > 0)

			if(alpha0[i] / alphaP[i] < minValue){

				k = i;

				minValue = alpha0[i] / alphaP[i];

			}

	Nb[k] = p;

	Cb[k] = c[p];

	for(int i = 1; i <= m; i++)

		if(i == k)

			continue;

		else

			for(int j = 1; j <= m; j++)

				Binv[i][j] = Binv[i][j] - Binv[k][j] * alphaP[i] / alphaP[k];

	for(int j = 1; j <= m; j++)

		Binv[k][j] /= alphaP[k];

	for(int i = 1; i <= m; i++){

	for(int j = 1; j <= m; j++){

		printf("%.2lf\t", Binv[i][j]);

	}

	printf("\n");

}

	printf("p=%d k=%d\n", p, k);

	printf("\n");

	free(alphaP);	

	free(alpha0);

}

	free(delta);

	free(u);

	for(int i = 1; i <= m; i++){

	free(B[i]);

	free(Binv[i]);

	}

	free(B);

	free(Binv);

	free(Cb);

	free(Nb);

	free(c);

	free(znak);

	for(int i = 0;i <= m; i++)

		free(a[i]);

	free(a);

	return 0;

}

int* doNb(int n, int m){

	int *nb = (int *) malloc((m + 1)*sizeof(int));

	int st, pos;

	for(int j = 1;j <= n; j++){

		st = 0;

	for(int i = 1;i <= m; i++){

		if(a[i][j] == 0)

			continue;

	else if(a[i][j] == 1){

	st = 1;

	pos = i;

	} else {

	st = 0;

	break;

	}

	}

	if(st == 1)

	nb[pos] = j;

	}

	return nb;

	}

	void addColumn(int i, double data, double cData, int *n, int m){

	double * mem= (double*) realloc(c, (*n + 2)*sizeof(double));

	if(mem == NULL)

	printf("error\n");

	else

	c = mem;

	c[*n + 1] = cData;

	for(int j = 1;j <= m; j++){

	mem = (double*) realloc(a[j], (*n + 2)*sizeof(double));

	if(mem == NULL)

	printf("error\n");

	else

	a[j] = mem;

	a[j][*n + 1] = (i == j) ? data : 0;

	}

	(*n)++;

	}

	double M(double *c, int *n){

	double res = c[1];

	for(int i = 2; i <= *n; i++)

	if(res < c[i])

	res = c[i];

	return -10.0 * res;

	}

	void balans(int *znak, int *n, int m){

	double Mzn = M(c, n);

	for(int i = 1; i <= m;i++)

	if(znak[i] == 1){

	addColumn(i, 1, 0, n, m);

	znak[i] = 0;

	} else if(znak[i]==3)

	addColumn(i, 1, Mzn, n, m);

	else {

	addColumn(i, 1, Mzn, n, m);

	znak[i] = 0;

	}

	for(int i = 1; i <= m; i++)

	if(znak[i] == 3)

	addColumn(i, -1, 0, n, m);

	}

	double* doCb(double *c, int *nb, int n){

	double *cb = (double*) malloc((n + 1)*sizeof(double));

	for(int i = 1; i <= n; i++)

	cb[i] = c[nb[i]];

	return cb;

	}

	void LUP(double **A, double **C, double **P, int n) {

	for(int i = 1; i <= n; i++)

	for(int j = 1; j <= n; j++){

	C[i][j] = A[i][j];

	P[i][j] = (i == j) ? 1 : 0;

	}

	for(int i = 1; i <= n; i++){

	double pivotValue = 0;

	int pivot = -1;

	for(int row = i; row <= n; row++)

	if(fabs(C[row][i]) > pivotValue){

	pivotValue = fabs(C[row][i]);

	pivot = row;

	}

	if(pivotValue == 0){

	printf("bad matrix");

	return;

	}

	double prom;

	for(int j = 1; j <= n; j++){

	prom = C[pivot][j];

	C[pivot][j] = C[i][j];

	C[i][j] = prom;

	prom = P[pivot][j];

	P[pivot][j] = P[i][j];

	P[i][j] = prom;

	}

	for(int j = i + 1; j <= n; j++){

	C[j][i] /= C[i][i];

	for(int k = i + 1; k <= n; k++)

	C[j][k] -= C[j][i] * C[i][k];

	}

	}

	}

	double** matMulmat(double **a, double **b, int n, int m, int p){

	double **res = (double**)malloc((n + 1)*sizeof(double*));

	for(int i = 1; i <= n; i++)

	res[i] = (double*)malloc((p + 1)*sizeof(double));

	double sum;

	for(int i = 1; i <= n; i++)

	for(int j = 1; j <= p; j++){

	sum = 0;

	for(int k = 1; k <= m; k++)

	sum += a[i][k] * b[k][j];

	res[i][j] = sum;

	}

	return res;

	}

	double* vecMulmat(double *a, double **b, int m, int n){

	double *res = (double*)malloc((n + 1)*sizeof(double*));

	double sum;

	for(int i = 1; i <= n; i++){

	sum = 0;

	for(int j = 1; j <= m; j++)

	sum += a[j] * b[j][i];

	res[i] = sum;

	}

	return res;

	}

	double* matMulvec(double **a, double *b, int m, int n){

	int i, j;

	double *res = (double*)malloc((m+1)*sizeof(double*));

	double sum;

	for(i=1;i<=m;++i){

	sum=0;

	for(j=1;j<=n;++j)

	sum+=a[i][j]*b[j];

	res[i]=sum;

	}

	return res;

	}

	double** inverse(double **A, int n) {

	double** X = (double**)malloc((n + 1)*sizeof(double*));

	for(int i = 1; i <= n; i++)

	X[i] = (double*)malloc((n + 1)*sizeof(double*));

	double** P = (double**)malloc((n + 1)*sizeof(double*));

	for (int i = 1; i <= n; i++)

	P[i] = (double*)malloc((n + 1)*sizeof(double*));

	double** C = (double**)malloc((n + 1)*sizeof(double*));

	for (int i = 1; i <= n; i++)

	C[i] = (double*)malloc((n + 1)*sizeof(double*));

	LUP(A, C, P, n);

	for(int k = n; k > 0; k--) {

	X[k][k] = 1;

	for(int j = n; j > k; j--)

	X[k][k] -= C[k][j] * X[j][k];

	X[k][k] /= C[k][k];

	for(int i = k - 1; i > 0; i--){

	for(int j = n; j > i; j--){

	X[i][k] -= C[i][j] * X[j][k];

	X[k][i] -= C[j][i] * X[k][j];

	}

	X[i][k] /= C[i][i];

	}

	}

	double** res = matMulmat(X, P, n, n, n);

	for(int i = 1; i <= n; i++){

	free(C[i]);

	free(P[i]);

	free(X[i]);

	}

	free(C);

	free(P);

	free(X);

	return res;

	}

	int argmin(double *a, int n){

	int res = 1;

	double resVal = a[res];

	for(int i = 2; i <= n; i++)

	if(a[i] < resVal){

	res = i;

	resVal = a[i];

	}

	return res;

	} 
