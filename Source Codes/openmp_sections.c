#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#define SIZE 1000000

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
#pragma omp parallel sections
{
#pragma omp section
		merge_sort(x,p,q);
#pragma omp section
		merge_sort(x,q+1,r);
}
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
	double start,end,max,min;
	int tnum,data[SIZE];
#pragma omp parallel
{
	tnum=omp_get_num_threads();
}
	double alg_start[tnum],alg_end[tnum];
	printf("No. of threads = %d\n",tnum);
	generate_list(data,SIZE);
	//printf("List before sorting\n");
	//print_list(data,SIZE);
	start = omp_get_wtime();
#pragma omp parallel
{
	alg_start[omp_get_thread_num()] = omp_get_wtime();
	merge_sort(data,0,SIZE-1);
	alg_end[omp_get_thread_num()] = omp_get_wtime();
}
	end = omp_get_wtime();
	max = find_max(alg_end,tnum);
	min = find_min(alg_start,tnum);
	printf("Stage 1 total runtime = %f\n",end-start);
	printf("Stage 1 alg runtime = %f\n",max-min);
	printf("Stage 1 overhead = %f\n",(end-start)-(max-min));
	//printf("List after sorting\n");
	//print_list(data,SIZE);
	generate_list(data,SIZE);
	start = omp_get_wtime();
#pragma omp parallel
{
	alg_start[omp_get_thread_num()] = omp_get_wtime();
	merge_sort(data,0,SIZE-1);
	alg_end[omp_get_thread_num()] = omp_get_wtime();
}
	end = omp_get_wtime();
	max = find_max(alg_end,tnum);
	min = find_min(alg_start,tnum);
	printf("Stage 2 total runtime = %f\n",end-start);
	printf("Stage 2 alg runtime = %f\n",max-min);
	printf("Stage 2 overhead = %f\n",(end-start)-(max-min));
	return 0;
}
