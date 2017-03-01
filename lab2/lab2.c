/*
 * lab2.c
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


#include <math.h>
#include <stdio.h>
#define m 2
#define n 6
#define isMaxTask 1

void readA(double (*A)[n + 1]){
	FILE *f = fopen("A.txt", "r");
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n + 1; j++){
			fscanf(f, "%lf\t", &A[i][j]);
		}
		fscanf(f, "\n");
	}
	fclose(f);
}

void readC(double *C){
	FILE *f = fopen("C.txt", "r");
	for(int i = 0; i < n + 1; i++)
		fscanf(f, "%lf\t", &C[i]);
	fclose(f);
}

void readCb(double *Cb){
	FILE *f = fopen("Cb.txt", "r");
	for(int i = 0; i < m; i++)
		fscanf(f, "%lf\t", &Cb[i]);
	fclose(f);
}

void readNb(int  *Nb){
	FILE *f = fopen("Nb.txt", "r");
	for(int i = 0; i < m; i++)
		fscanf(f, "%d\t", &Nb[i]);
	fclose(f);
}

void changeBasis(double (*A)[n + 1], int k, int p){
	double kp = A[k][p];
	double temp[m][n + 1];
	for(int i = 0; i < m; i++){
		if(i != k){
			for(int j = 0; j < n + 1; j++)
				temp[i][j] = A[i][j] - (A[k][j] * A[i][p]) / kp;
		}else{
			for(int j = 0; j < n + 1; j++)
				temp[i][j] = A[i][j] / kp;
		}
	}
	for(int i = 0; i < m; i++)
		for(int j = 0; j < n + 1; j++)
			A[i][j] = temp[i][j];
}

void getDelta(double *delta, double *Cb, double *C, double (*A)[n + 1]){
	for(int i = 0; i < n + 1; i++){
	double sum = 0;
		for(int j = 0; j < m; j++){
			sum += Cb[j] * A[j][i];
		}
		delta[i] = sum - C[i];
	}
}

int checkDelta(double *delta, int isMax){
	int isOpt = 1;
	for(int i = 0; i < n; i++){
		if(isMax == 1){
			if(delta[i] < 0){
				isOpt = 0;
				break;
			}
		}else{
			if(delta[i] > 0){
				isOpt = 0;
				break;
			}
		}
	}
	return isOpt;
}

int getP(double *delta, int isMax){
	int p = 0;
	double max = 0;
	for(int i = 0; i < n; i++){
		if(isMax == 1){
			if(fabs(delta[i]) >= max){
				max = fabs(delta[i]);
				p = i;
			}
		}else{
			if(delta[i] >= max){
				max = fabs(delta[i]);
				p = i;
			}
		}
	}
	return p;
}

int getK(double (*A)[n + 1], double *delta){
	int k = 0;
	double min = 9999;
	double teta[m];
	for(int i = 0; i < m; i++){
		teta[i] = A[i][n] / A[i][getP(delta, isMaxTask)];
		if(teta[i] >= 0)
		if(teta[i] <= min){
			min = teta[i];
			k = i;
		}
	}
	return k;
}

int isNoResult(double (*A)[n + 1], double *delta){
	for(int i = 0; i < n; i++){
		if(delta[i] < 0){
			int k = 0;
			for(int j = 0; j < m; j++){
				if(A[j][i] < 0)
					k++;
			}
			if(k == (m + 1))
				return 1;
		}
	}
	return 0;
}

void printTable(double (*A)[n + 1]){
	printf("------------------------------------------------------\n");
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n + 1; j++){
			printf("%.2lf\t", A[i][j]);
		}
		printf("\n");
	}
}

void printDelta(double *delta){
	printf("Delta : ");
	for(int i = 0; i < n + 1; i++)
		printf("%.2lf\t", delta[i]);
	printf("\n");
}

void printCbNb(double *Cb, int *Nb){
	printf("Cb : ");
	for(int i = 0; i < m; i++)
		printf("%.2lf   ", Cb[i]);
	printf("\n");
	printf("Nb : ");
	for(int i = 0; i < m; i++)
		printf("%d   ", Nb[i]);
	printf("\n");
}

int main(int argc, char **argv)
{
	double A[m][n+1], delta[n + 1];
	double Cb[m], C[n + 1];
	int Nb[m];
	int k, p;
	readA(A);
	readC(C);
	readCb(Cb);
	readNb(Nb);
	printTable(A);
	printCbNb(Cb, Nb);
	getDelta(delta, Cb, C, A);
	printDelta(delta);
	if(isNoResult(A, delta) == 1)
		return 0;
	while(checkDelta(delta, isMaxTask) == 0){
		p = getP(delta, isMaxTask);
		k = getK(A, delta);
		Nb[k] = p;
		changeBasis(A, k, p);
		printTable(A);
		Cb[k] = C[Nb[k]];
		printCbNb(Cb, Nb);
		getDelta(delta, Cb, C, A);
		printDelta(delta);
		if(isNoResult(A, delta) == 1)
			return 0;
	}
	return 0;
}
