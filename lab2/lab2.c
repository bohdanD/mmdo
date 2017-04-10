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
#include <malloc.h>
//#define m 2
//#define n 6
//#define isMaxTask 1
int m, n;
int isMaxTask;
int *Nb;
double **A, *delta, *Cb, *C;


void readA(){
	FILE *f = fopen("A.txt", "r");
	//for(int i = 0; i < m; i++){
	//	for(int j = 0; j < n + 1; j++){
	//		fscanf(f, "%lf\t", &A[i][j]);
	//	}
	//	fscanf(f, "\n");
	//}
	m = 0;
	n = 0;
	char buff[250];
	double a[100];
	while(!feof(f))
		if(fgets(buff, 250, (FILE* )f))
			m++;
	fclose(f);
	f = fopen("A.txt", "r");
	while(!feof(f)){
		if(fscanf(f, "%lf", &a[n]))
			n++;
	}
	n = n / m - 1;
	A = (double**) malloc(m * sizeof(double*));
	for (int i = 0; i < m; i++)
	{
		A[i] = (double*) malloc((n+1) * sizeof(double));
	}
	int k = 0;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n + 1; j++)
		{
			A[i][j] = a[k];
			k++;
		}
	}
	
	fclose(f);
	f = fopen("sign.txt", "r");
	fscanf(f, "%d", &isMaxTask);
	fclose(f);
	Nb = (int*) malloc(m * sizeof(int));
	for(int i=0; i<m; i++){
		Nb[i] = n - m + i;
	}
		
}

void readC(){
	FILE *f = fopen("C.txt", "r");
	C = (double*) malloc((n+1) * sizeof(double));
	for(int i = 0; i < n + 1; i++)
		fscanf(f, "%lf\t", &C[i]);
	fclose(f);
}

void readCb(){
	FILE *f = fopen("Cb.txt", "r");
	Cb = (double*) malloc(m * sizeof(double));
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

void changeBasis(double **A, int k, int p){
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

void getDelta(double *delta, double *Cb, double *C, double **A, int *Nb){
	for(int i = 1; i < n + 1; i++){
	double sum = 0;
		for(int j = 0; j < m; j++){
			sum += Cb[j] * A[j][i];
		}
		delta[i-1] = sum - C[i-1];
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
	double max = 999;
	for(int i = 0; i < n; i++){
		if(isMax == 1){
			if(delta[i] < max){
				max = delta[i];
				p = i;
			}
			/*if(fabs(delta[i]) >= max){
				max = fabs(delta[i]);
				p = i;
			}*/
		}else{
			if(delta[i] > max){
				max = fabs(delta[i]);
				p = i;
			}
		}
	}
	return p+1;
}

int getK(double **A, double *delta, int p){
	int k = 0;
	double min = 9999;
	double *teta = (double *) malloc(sizeof(double) * m);
	for(int i = 0; i < m; i++){
		//if(fabs(A[i][p]) > 0.00000001){
			teta[i] = A[i][0] / A[i][p];
			//if(teta[i] >= 0)
			if(teta[i] < min){
				min = teta[i];
				k = i;
			}
		//}
		
	}
	free(teta);
	return k;
}

int isNoResult(double **A, double *delta){
	for(int i = 0; i < n; i++){
		if(delta[i] < 0){
			int k = 0;
			for(int j = 0; j < m; j++){
				if(A[j][i] < 0)
					k++;
			}
			if(k == (m))
				return 1;
		}
	}
	return 0;
}

void printTable(double **A){
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

void printCbNb(double *cb, int *nb){
	printf("Cb : ");
	for(int i = 0; i < m; i++)
		printf("%.2lf   ", cb[i]);
	printf("\n");
	printf("Nb : ");
	for(int i = 0; i < m; i++)
		printf("%d   ", nb[i]);
	printf("\n");
}

int main(int argc, char **argv)
{
	delta = (double *) malloc(n * sizeof(double));
	//readA(A);
	//printf("m=%d\tn=%d\t", m, n);
	//double /*A[m][n+1], */delta[n + 1];
	//double Cb[m], C[n + 1];
	//int Nb[m];
	int k, p;
	readA();
	readC();
	readCb();
	//readNb(Nb);
	printTable(A);
	printCbNb(Cb, Nb);
	getDelta(delta, Cb, C, A, Nb);
	printDelta(delta);
	if(isNoResult(A, delta) == 1)
		return 0;
	while(checkDelta(delta, isMaxTask) == 0){
		p = getP(delta, isMaxTask);
		k = getK(A, delta, p);
		printf("p= %d, k= %d\n", p, k);
		Nb[k] = p;
		//printf("%d\n", Nb[0]);
		changeBasis(A, k, p);
		printTable(A);
		Cb[k] = C[Nb[k] - 1];
		printCbNb(Cb, Nb);
		getDelta(delta, Cb, C, A, Nb);
		printDelta(delta);
		if(isNoResult(A, delta) == 1)
			return 0;
	}
	
	//printTable(A);
	for (int i = 0; i < m; i++)
	{
		free(A[i]);
	}
	free(A);
	free(C);
	free(Cb);
	//free(Nb);
	//free(delta);
	return 0;
}
