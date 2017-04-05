/*
 * lab7.c
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
#define eps 0.0000000001


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
	if(sumA == sumB) printf("Closed!\n");
	else printf("Open!");
}


void findC1(double** C){
	for(int j=0; j<n; j++){
		double min = 999999;
		for(int i=0; i<m; i++){
			if(min > C[i][j])
				min = C[i][j];
		}
		for(int i=0; i<m; i++){
			C[i][j] -= min;
		}
	}
	
}

void findC0(double** C){
	for(int i=0; i<m; i++){
		double min = 999999;
		for(int j=0; j<n; j++){
			if(min > C[i][j])
				min = C[i][j];
		}
		for(int j=0; j<n; j++){
			C[i][j] -= min;
		}
	}
	
}

void methodNorhWest(double **X, double** C, double *A, double *B){
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			X[i][j] = -1;
		}
	}
	int i =0;
	int j=0;
	while(i != m && j != n){
		if(fabs(C[i][j]) < eps){
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
		}else{
			i++;
		}

	}
}

double nevjazka(double** X, double* A, double* B){
	double* bi = malloc(m * sizeof(double));
	double* bj = malloc(n * sizeof(double));
	
	for(int i=0; i<m; i++){
		double sum = 0;
		for(int j=0; j<n; j++){
			sum += X[i][j];
		}
		bi[i] = A[i] - sum;
	}
	for(int j=0; j<n; j++){
		double sum = 0;
		for(int i=0; i<m; i++){
			sum += X[i][j];
		}
		bj[j] = B[j] - sum;
	}
	
	double sumBi = 0;
	double sumBj = 0;
	for(int j=0; j<n; j++)
		sumBj += bj[j];
	for(int i=0; i<m; i++)
		sumBi += bi[i];
	return sumBi + sumBj;
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
	findC1(C);
	printTable(C, A, B);
	findC0(C);
	printTable(C, A, B);
	methodNorhWest(X, C, A, B);
	printTable(X, A, B);
	printf("Nevjazka = %lf \n", nevjazka(X, A, B));
	return 0;
}

