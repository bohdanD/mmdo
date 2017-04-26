


#include "stdio.h"
#include "malloc.h"

double *c, **a;
int oN, oM, n, m;
double M_value;

void balans(int *znak);
double delta(int j);
int* doNb();

int main(){
	oM = 4;
	oN = 5;
	int *znak;
	n = oN;
	m = oM;
//----------READ--------------------------
	FILE *in = fopen("c.txt", "r");
	c = (double*) malloc((n + 1)*sizeof(double));
	for(int i = 1; i <= n; i++)
		fscanf(in, "%lf ", &c[i]);
	fclose(in);
	in = fopen("a.txt", "r");
	a = (double**) malloc((m + 1)*sizeof(double*));
	for(int i = 0; i <= m; i++)
		a[i] = (double*) malloc((n + 1)*sizeof(double));
	for(int i = 1; i <= m; i++)
		for(int j = 0; j <= n; j++)
			fscanf(in, "%lf ", &a[i][j]);
	fclose(in);
	in = fopen("znak.txt", "r");
	znak = (int*) malloc((m + 1)*sizeof(int));
	for(int i = 1; i <= m; i++)
		fscanf(in, "%d ", &znak[i]);
	fclose(in);
	//----------------BALANS--------------------------
	balans(znak);
	int *Nb = doNb();
//------------------------------------------------
	
	
	
	free(Nb);
	free(c);
	free(znak);
	for(int i = 0; i <= m; i++)
		free(a[i]);
	free(a);
	return 0;
}

void newA(double **a, int k, int p){
	//p = p -1;
	double **tmp = (double**) malloc((m + 1)*sizeof(double*));
	for(int i = 0; i <= m; i++)
		tmp[i] = (double*) malloc((n + 1)*sizeof(double));
	for(int i = 1; i <= m; i++)
		for(int j = 0; j <= n; j++)
			if(i != k)
				tmp[i][j] = (a[k][p] * a[i][j] - a[i][p] * a[k][j]) / a[k][p];
			else
				tmp[i][j] = a[i][j] / a[k][p];
	for(int i = 1; i <= m; i++)
		for(int j = 0; j <= n; j++)
			a[i][j] = tmp[i][j];
}

double M(double *c){
	double res = c[1];
	for(int i = 2; i <= n; i++)
		if(res < c[i])
			res = c[i];
	return -10.0 * res;
}

void addColumn(int i, double data, double cData){
	double* mem = (double*) realloc(c, (n + 2)*sizeof(double));
	if(mem == NULL)
		printf("error\n");
	else
		c = mem;
	c[n + 1] = cData;
	for(int j = 1; j <= m;j++){
		mem = (double*) realloc(a[j], (n + 2)*sizeof(double));
		if(mem == NULL)
			printf("error\n");
		else
			a[j] = mem;
		a[j][n + 1] = (i == j) ? data : 0;
	}
	n++;
}

void balans(int *znak){
	M_value = M(c);
	for(int i = 1; i <= m; i++)
		if(znak[i] == 1)
			addColumn(i, 1, 0);
		else if(znak[i] == 3){
			for(int j = 0; j <= n; j++)
				a[i][j] *= -1.0;
			addColumn(i, 1, 0);
		}
		else
			addColumn(i, 1, M_value);
}

double delta(int j){
	double res = c[j];
	for(int i = 1; i <= m; i++)
		res -= c[i] * a[i][j];
	return res;
}

int * doNb(){
	int *nb = (int *) malloc((m + 1)*sizeof(int));
	int s, pos;
	for(int j = 1; j <= n; j++){
		s = 0;
		for(int i = 1; i <= m; i++){
			if(a[i][j] == 0)
				continue;
			else if(a[i][j] == 1){
				s = 1;
				pos = i;
			} else {
				s = 0;
				break;
			}
		}
		if(s == 1)
			nb[pos] = j;
	}
	return nb;
}

void DoDvoistyy(double** a, int* Nb){
	
}





