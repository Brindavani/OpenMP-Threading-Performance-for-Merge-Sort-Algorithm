#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <time.h>
#include <sys/time.h>
#define SIZE 100000

void generate_list(int *x,int n){
	int i=0,j,t;
	for(i=0;i<n;i++)
		x[i]=i;
	for(i=0;i<n;i++){
		j=rand()%n;
		t=x[i];
		x[i]=x[j];
		x[j]=t;
	}
}

void merge(int *x,int p,int q,int r){
	int i,j,k,n1=q-p+1, n2=r-q, L[n1], R[n2];
	for(i=0;i<n1;i++)
		L[i]=x[p+i];
	for(j=0;j<n2;j++)
		R[j]=x[q+j+1];
	L[n1]=R[n2]=SIZE;
	i=j=0;
	for(k=p;k<r;k++){
		if(L[i]<=R[j])
			x[k]=L[i++];
		else
			x[k]=R[j++];
	}
	while(i<n1)
		x[k++]=L[i++];
	while(j<n2)
		x[k++]=R[j++];
}

void merge_sort(int *x,int p,int r){
	if(p<r){
		int q=(p+r)/2;
#pragma omp task firstprivate(x,p,q)
		merge_sort(x,p,q);
#pragma omp task firstprivate(x,q,r)
		merge_sort(x,q+1,r);
#pragma omp taskwait
		merge(x,p,q,r);
	}
}

/*void print_list(int *x,int n){
	int i;
	for(i=0;i<n;i++)
		printf("%d\t",x[i]);
	printf("\n");
}*/

double find_max(double arr[],int size){
	int i;
	double max=-1;
	for(i=0;i<size;i++)
		if(arr[i]>max)
			max=arr[i];
	return(max);
}

double find_min(double arr[],int size){
	int i;
	double min=arr[0];
	for(i=1;i<size;i++)
		if(arr[i]<min)
			min=arr[i];
	return(min);
}

int main(){
	struct timeval start,end;
	int tnum,data[SIZE];
	double run_time;
#pragma omp parallel
{
	tnum=omp_get_num_threads();
}
	printf("No. of threads = %d\n",tnum);
	generate_list(data,SIZE);
	//printf("List before sorting\n");
	//print_list(data,SIZE);
	gettimeofday(&start,NULL);
#pragma omp single
{
	merge_sort(data,0,SIZE-1);
}
	merge_sort(data,0,SIZE-1);
	gettimeofday(&end,NULL);
	run_time = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);
	printf("Stage 1 alg runtime in seconds = %lf\n",run_time);
	//printf("List after sorting\n");
	//print_list(data,SIZE);
	generate_list(data,SIZE);
	gettimeofday(&start,NULL);
#pragma omp single
{
	merge_sort(data,0,SIZE-1);
}
	gettimeofday(&end,NULL);
	run_time = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);
	printf("Stage 2 alg runtime in seconds = %lf\n",run_time);
	return 0;
}
