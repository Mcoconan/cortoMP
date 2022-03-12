#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <time.h>
#define MAXCHAR 100
#define THREADS 4

int* read(int N, char* file_name){
    FILE *fp;
    char row[MAXCHAR];
    char *token;
    int arr[N];
    int counter = 0;

    fp = fopen(file_name,"r");

    while (feof(fp) != true)
    {
        fgets(row, MAXCHAR, fp);
        token = strtok(row, ",");
        while(token != NULL)
        {
            arr[counter] = strtol(token, NULL, 10);
            token = strtok(NULL, ",");
            counter++;
        }
    }

    return arr;
}

void write(int x[], int N, char* file_name){
  FILE *fpt;
  fpt = fopen(file_name, "w+");
  int j;
  for (j=0; j<N - 1; j++) 
    fprintf(fpt, "%d, ", x[j]);
  fprintf(fpt, "%d", x[N - 1]);
  fclose(fpt);
}



void par_qsort(int *data, int lo, int hi) 
{
  if(lo > hi) return;
  int l = lo;
  int h = hi;
  int p = data[(hi + lo)/2];

  while(l <= h){
    while((data[l] - p) < 0) l++;
    while((data[h] - p) > 0) h--;
    if(l<=h){
      int tmp = data[l];
      data[l] = data[h];
      data[h] = tmp;
      l++; h--;
    }
  }
  #pragma omp task firstprivate(data,lo,h)
  par_qsort(data, lo, h);
  
  #pragma omp task firstprivate(data,l,hi)
  par_qsort(data, l, hi);
}

int main(int argc, char* argv[]) {
		
 int N = 10;
    int x[N];
    int j;

    for (j=0; j<N; j++)
      x[j] = rand()%(N/2)+1;

    printf("Random Numbers: ");
    for (j=0; j<N; j++) 
        printf("%d ", x[j]);
    write(x, N, "array.csv");
    int* arr = read(N, "array.csv");
    
	omp_set_num_threads(THREADS);
    #pragma omp parallel
	{

	#pragma omp single nowait
	par_qsort(arr, 0, N-1);

	}
	

    
	printf("\nQuick Sorted Numbers: ");
    for (j=0; j<N; j++) 
        printf("%d ", x[j]);
    write(arr, N, "ordenados.csv");

    return 0;	
}
