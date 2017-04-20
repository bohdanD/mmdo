/*
 * lab6.c
 * 
 * Copyright 2017 bohdan <bohdan@bohdan-Lenovo-V580c>
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


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define m 3
#define n 5

void scanData(double **C, double *A, double *B){
	FILE *fc = fopen("C.txt", "r");
	FILE *fa = fopen("A.txt", "r");
	FILE *fb = fopen("B.txt", "r");
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			fscanf(fc, "%lf\t", &C[i][j]);
		}
		fscanf(fc, "\n");
		fscanf(fa, "%lf\t", &A[i]);
	}
	for(int i = 0; i < n; i++)
		fscanf(fb, "%lf\t", &B[i]);
	fclose(fc);
	fclose(fa);
	fclose(fb);
}

void printTable(double **C, double *A, double *B){
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			printf("%.2lf\t", C[i][j]);
		}
		printf("\n");	
	}
	printf("A: ");
	for(int i = 0; i < m; i++)
		printf("%.2lf\t", A[i]);
	printf("\nB : ");
	for(int i = 0; i < n; i++)
		printf("%.2lf\t", B[i]);
	printf("\n");
}

void checkOpen(double *A, double *B){
	double sumA = 0, sumB = 0;
	for(int i = 0; i < m; i++)
		sumA += A[i];
	for(int i = 0; i < n; i++)
		sumB += B[i];
	if(sumA == sumB) printf("Closed!");
	else printf("Open!");
}

void methodNorhWest(double **X, double *A, double *B){
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			X[i][j] = -1;
		}
	}
	int i =0;
	int j=0;
	while(i != m && j != n){
		if(A[i] < B[j]){
			X[i][j] = A[i];
			B[j] -= X[i][j];
			i++;
		}else if(A[i] > B[j]){
			X[i][j] = B[j];
			A[i] -= X[i][j];
			j++;
		}else if(A[i] == B[j]){
			X[i][j] =A[i];
			A[i] = 0;
			j++;
		}
	}
}

void printX(double **X){
	printf("\n");
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++)
			printf("%.2lf\t", X[i][j]);
		printf("\n");
	}
}

void printDelta(double** delta){
	printf("Delta:\n");
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			printf("%.2lf\t", delta[i][j]);
		}
		printf("\n");
	}
}

void findUV(double **C, double **X, double *U, double *V){
		for(int i=0; i<m; i++){
			U[i] = -1;
		}
		for(int i=0; i<n; i++){
			V[i] =-1;
		}
		U[0] = 0;
		V[0] = C[0][0];
		for(int j=0; j<n; j++){
			if(C[0][j]>=0){
				V[j] = C[0][j] - U[0];
			}
		}
		for(int i=1; i<m; i++){
			for(int j=0; j<n; j++){
				if(X[i][j]>=0){
					if(V[j] != -1){
						U[i] = C[i][j] - V[j];
					}
				}
			}
			
		}
}

void findDelta(double** C, double** delta, double* U, double* V){
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			delta[i][j] = U[i] + V[j] - C[i][j];
		}
	}
	
}

int isOptimal(double** delta){
	int flag=1;
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			if(delta[i][j]>0){
				flag = 0;
				break;
			}
		}
	}
	return flag;
}

void firstPart(double** X, double** delta){
	int** v = (int**) malloc(sizeof(int*) * m);
	for(int i=0; i<m; i++){
		v[i] = (int*) malloc(sizeof(int) * n);
	}
	
	double max = 0;
	int row = -1;
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			v[i][j] = 0; 
			if(delta[i][j] > max){
				max = delta[i][j];
				row = i;
			}
		}
	}
	for(int j=0; j<n; j++){
		v[row][j] = 1;
		if(X[row][j] >= 0){
			for(int i=0; i<m; i++){
				v[i][j] = 1;
				for(int col=0; col<n; col++){
					if(X[i][col] >= 0){
						v[i][col] = 1;
					}
				}
			}
		}
	}
	printf("vydil:\n");
	int flag = 1;
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			printf("%d\t", v[i][j]);
			if(X[i][j] >= 0 && v[i][j] != 1)
				flag = 0;
		}
		printf("\n");
	}
	printf("flag= %d\n", flag);
	for(int i=0; i<m; i++){
		free(v[i]);
	}
	free(v);
}


int main(int argc, char **argv)
{
	double **C;
	C = (double**)malloc(m * sizeof(double*));
	for(int i = 0; i < m; i++)
		C[i] = (double*)malloc(n * sizeof(double));
	double **X;
	X = (double**)malloc(m * sizeof(double*));
	for(int i = 0; i < m; i++)
		X[i] = (double*)malloc(n * sizeof(double));
	double **Z;
	Z = (double**)malloc(m * sizeof(double*));
	for(int i = 0; i < m; i++)
		Z[i] = (double*)malloc(n * sizeof(double));
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n; j++)
			Z[i][j] = -1;
	double **delta;
	delta = (double**)malloc(m * sizeof(double*));
	for(int i = 0; i < m; i++)
		delta[i] = (double*)malloc(n * sizeof(double));
	double* A = malloc(m * sizeof(double));
	double* B = malloc(n * sizeof(double));
	scanData(C, A, B);
	printTable(C, A, B);
	checkOpen(A, B);
	methodNorhWest(X, A, B);
	printf("X:\n");
	printX(X);
	double* U = (double*) malloc(m * sizeof(double));
	double* V = (double*) malloc(n * sizeof(double));
	findUV(C, X, U, V);
	findDelta(C, delta, U, V);
	printDelta(delta);
	firstPart(X, delta);
	//while(!isOptimal(delta)){
		
	//}
	
	for(int i = 0; i < m; i++){
		free(C[i]);
		free(X[i]);
		free(Z[i]);
		free(delta[i]);
	}
	free(C);
	free(X);
	free(Z);
	free(delta);
	free(A);
	free(B);
	return 0;
}

