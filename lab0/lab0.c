

#include <stdio.h>
#include <math.h>

#define size 4
#define rows 2
#define cols 3

void writeVec(double array[], char filename[])
{
	FILE *finput = fopen(filename, "w");
	for(int i=0; i<size; i++)
	{
		fprintf(finput, "%lf\n", array[i]);
		printf("%lf\n", array[i]);
	}
	printf("Дані записано в %s", filename);
	fclose(finput);
}

double* readVec(char filename[])
{
	FILE *foutput = fopen(filename, "r");
	static double array[size];
	rewind(foutput);
	for(int i=0; i<size; i++)
	{
		fscanf(foutput, "%lf", &array[i]);
		printf("%lf\n", array[i]);
	}
	return array;
}
	

void writeMatr(double array[rows][cols], char filename[], int row, int column)
{
	FILE * finput = fopen(filename, "w");
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<column; j++)
		{
			fprintf(finput, "%lf\n", array[i][j]);
			printf("%lf\n", array[i][j]);
		}
		
	}
	printf("Дані записано в %s", filename);
	fclose(finput);
}

void readMatr(double array[rows][cols], char filename[])
{
	FILE *foutput = fopen(filename, "r");
	rewind(foutput);
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			fscanf(foutput, "%lf", &array[i][j]);
			printf("%lf\n", array[i][j]);
		}
		
	}
}


int main()
{
	
	double array[] = {1.0d, 2, 4, 3};
	writeVec(array, "input.txt");
	
	double x[rows][cols] = {{1, 2, 3},{4, 5, 6}};
	writeMatr(x, "finput.txt", 2, 3);
	
	readVec("input.txt");
	double y[rows][cols];
	readMatr(y, "finput.txt");
	return 0;
}
