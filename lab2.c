#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "omp.h"

struct coordinate{
	int r,c;
};

int* makeSquareMatrix(int size){
	//return calloc((size * size) * sizeof(int));
	return calloc(size*size,sizeof(int));
}

int coordinateToIndex( struct coordinate* coordinate, int size){ //coverts 2D coordinate into 1D array index
	return (coordinate->r * size) + coordinate->c;
}

void initialize(int* array, int size){ //intitialize array to have random values
	for(int i = 0; i< (size * size);i++){
		int maxValue = 10; //random numbers from 0 to maxValue
		array[i] = rand()%maxValue;
	}
}

void print(int* array, int size){
	for(int i = 0; i<size*size;i++){
		if( (i % size) == 0){
			printf("\n \t %d",array[i]);
		}else{
			printf(" \t %d",array[i]);
		}
	}
}

void swap(int* array, int size,struct coordinate* rhs,struct coordinate* lhs){
	int rhsIndex = coordinateToIndex(rhs,size);
	int lhsIdex = coordinateToIndex(lhs,size);
	int temp = array[rhsIndex];
	array[rhsIndex] = array[lhsIdex];
	array[lhsIdex] = temp;
}


void transpose(int* array, int size, int threads){ //use 'threads' number of threads to transpose matrix
	//omp_set_num_threads(2);
	#pragma omp parallel for num_threads(threads)
	for(int i = 0 ; i< size;i++){
		for(int j = i;j<size;j++){
			struct coordinate rhs; //swap coordinate on opposite sides of the diagonal
			rhs.r = i; rhs.c = j;
			struct coordinate lhs;
			lhs.r = j; lhs.c = i;
			swap(array,size,&rhs,&lhs);
		}
	}
}



int main(){
	clock_t t;
	srand((unsigned) time(&t));
	
	/*
	int size = 5;
	
	int* array = makeSquareMatrix(size);
	initialize(array,size)
	print(array,size);
	
	printf("\n------------ Transpose ---------\n ");
	int threads = 10;
	transpose(array,size,threads);
	print(array,size);
	
	
	free(array);
	*/
	
	int sizes[] = {128,1024,8192};
	int numSizes = 3;
	
	for(int s = 0;s < numSizes;s++){
		
		double time1, timeTaken;
		
		int* array = makeSquareMatrix(sizes[s]);
		initialize(array,sizes[s]);
		int threads = 4;
		for(int i = 0; i < 6; i++){
			//clock_t begin = clock();
			time1 = omp_get_wtime();
			transpose(array,sizes[s],threads);
			timeTaken = omp_get_wtime() - time1;
			printf("\n size = %d \t threads = %d \t timeElaped = %f \n",sizes[s],threads,timeTaken);
			threads = threads*2;
		}
		
		free(array);
	}
	return 0;
}