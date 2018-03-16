#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "omp.h"

#define NUM_THREADSF 4
#define NUM_THREADSE 8
#define NUM_THREADSS 16
#define NUM_THREADSSF 64
#define NUM_THREADSO 128

const int matrixDimensionsOne=128;
const int matrixDimensionsTwo=1024;
const int matrixDimensionsThree=8192;

//threads take parameters passed in structs so i had to create a struct that holds the matrix, and the index information
struct parameters{
    int **matrixtotranspose;
    int rbegin;
    int rend;
}; 

void Logicfortranspose(int **mattranspose){
    int temp;
    for (int i = 0; i < matrixDimensionsOne - 1; i++) {
        for (int j = i + 1; j < matrixDimensionsOne; j++) {
            temp = mattranspose[i][j];
            mattranspose[i][j] = mattranspose[j][i];
            mattranspose[j][i] = temp;
        }
    }
}

void print(int size, int **M){
	for(int i = 0; i<size*size;i++){
		for(int j=0; j<size; j++){
			printf("%d \t", M[i][j]);
			}
		printf("\n");
	}
}

void *transpose(void *args){
    struct parameters *parameters;
    parameters = (struct parameters *) args;
    int holder;

    for (int i = parameters->rbegin; i < parameters->rend; i++) {
        for (int j = i + 1; j < matrixDimensionsOne; j++) {
            int temp = parameters->matrixtotranspose[i][j];
            parameters->matrixtotranspose[i][j] = parameters->matrixtotranspose[j][i];
            parameters->matrixtotranspose[j][i] = temp;
        }
    }
    pthread_exit(NULL);
}

int main(){

    int **Matix;
    Matix = malloc(matrixDimensionsOne * sizeof(int *));//creating the memory space for the matrix
    int **Matix2;
    Matix2 = malloc(matrixDimensionsTwo * sizeof(int *));//creating the memory space for the matrix
    int **Matix3;
    Matix3 = malloc(matrixDimensionsThree * sizeof(int *));
	for (int i = 0; i < matrixDimensionsOne; i++){
        	Matix[i] = (int *) malloc(matrixDimensionsOne * sizeof(int));
        	}
     for (int i = 0; i < matrixDimensionsTwo; i++){
        	Matix2[i] = (int *) malloc(matrixDimensionsTwo * sizeof(int));
        	}
     for (int i = 0; i < matrixDimensionsThree; i++){
        	Matix3[i] = (int *) malloc(matrixDimensionsThree * sizeof(int));
        	}
	//
		int count=0;
		int count2=0;
		int count3=0;
    		for (int i = 0; i < matrixDimensionsOne; i++) {
        		for (int j = 0; j < matrixDimensionsOne; j++) {
            			Matix[i][j] = count;
            			count++;
        		}
    		}
    		
    		for (int i = 0; i < matrixDimensionsTwo; i++) {
        		for (int j = 0; j < matrixDimensionsTwo; j++) {
            			Matix2[i][j] = count2;
            			count2++;
        		}
    		}
    		
    		for (int i = 0; i < matrixDimensionsThree; i++) {
        		for (int j = 0; j < matrixDimensionsThree; j++) {
            			Matix3[i][j] = count3;
            			count3++;
        		}
    		}
	//should change stuff here
    double matrixSize = matrixDimensionsOne;//Should add for the other matrix sizes
    double matrixSizetwo = matrixDimensionsTwo;
    double matrixSizethree = matrixDimensionsThree;

    struct parameters *Forthread;
    double time1,time2,time3,time4,time5, timeTaken1,timeTaken2,timeTaken3, timeTaken4,timeTaken5 ;
    time1 = omp_get_wtime();
    pthread_t threadsF[NUM_THREADSF];
     
    for (int i = 0; i < NUM_THREADSF; i++) {
        Forthread = malloc(sizeof(struct temp *));
        Forthread->matrixtotranspose = Matix;
        Forthread->rbegin = (matrixSize/NUM_THREADSF) * i;
        Forthread->rend = (matrixSize /NUM_THREADSF) * (i + 1);
        pthread_create(&threadsF[i], NULL, transpose, (void *)Forthread);
    }

    for (int i = 0; i <NUM_THREADSF; i++) {
        pthread_join(threadsF[i], NULL);
    }

    timeTaken1 = omp_get_wtime() - time1;
    time1 = omp_get_wtime();
    Logicfortranspose(Matix);
    timeTaken1 = omp_get_wtime() - time1;
    
//    For 8 pthreads and matrix size 128
	
	struct parameters *Forthread8;
	time2 = omp_get_wtime();
	pthread_t threadsE[NUM_THREADSE];
	for (int i = 0; i < NUM_THREADSE; i++) {
        Forthread8 = malloc(sizeof(struct temp *));
        Forthread8->matrixtotranspose = Matix;
        Forthread8->rbegin = (matrixSize/NUM_THREADSE) * i;
        Forthread8->rend = (matrixSize/NUM_THREADSE) * (i + 1);
        pthread_create(&threadsE[i], NULL, transpose, (void *)Forthread8);
	
    }

    for (int i = 0; i <NUM_THREADSE; i++) {
        pthread_join(threadsE[i], NULL);
    }

    timeTaken2 = omp_get_wtime() - time2;
    time2 = omp_get_wtime();
    Logicfortranspose(Matix);
    timeTaken2 = omp_get_wtime() - time2;
    
// For 16 pthreads and the matrix size 128
	struct parameters *Forthread16;
	time3 = omp_get_wtime();
	pthread_t threadsS[NUM_THREADSS];
	for (int i = 0; i < NUM_THREADSS; i++) {
        Forthread16 = malloc(sizeof(struct temp *));
        Forthread16->matrixtotranspose = Matix;
        Forthread16->rbegin = (matrixSize/NUM_THREADSS) * i;
        Forthread16->rend = (matrixSize/NUM_THREADSS) * (i + 1);
        pthread_create(&threadsS[i], NULL, transpose, (void *)Forthread16);
	
    }

    for (int i = 0; i <NUM_THREADSS; i++) {
        pthread_join(threadsS[i], NULL);
    }

   
    timeTaken3 = omp_get_wtime() - time3;
    
    time3 = omp_get_wtime();
    Logicfortranspose(Matix);
    timeTaken3 = omp_get_wtime() - time3;
    
// For 64 pthreads and the matrix size 128
	struct parameters *Forthread64;
	time4 = omp_get_wtime();
	pthread_t threadsSF[NUM_THREADSSF];
	for (int i = 0; i < NUM_THREADSSF; i++) {
        Forthread64 = malloc(sizeof(struct temp *));
        Forthread64->matrixtotranspose = Matix;
        Forthread64->rbegin = (matrixSize/NUM_THREADSSF) * i;
        Forthread64->rend = (matrixSize/NUM_THREADSSF) * (i + 1);
        pthread_create(&threadsSF[i], NULL, transpose, (void *)Forthread64);
	
    }

    for (int i = 0; i <NUM_THREADSSF; i++) {
        pthread_join(threadsSF[i], NULL);
    }

   
    timeTaken4 = omp_get_wtime() - time4;
    
    time4 = omp_get_wtime();
    Logicfortranspose(Matix);
    timeTaken4 = omp_get_wtime() - time4;
    
// For 128 pthreads and the matrix size 128
	struct parameters *Forthread128;
	time5 = omp_get_wtime();
	pthread_t threadsO[NUM_THREADSO];
	for (int i = 0; i < NUM_THREADSO; i++) {
        Forthread128 = malloc(sizeof(struct temp *));
        Forthread128->matrixtotranspose = Matix;
        Forthread128->rbegin = (matrixSize/NUM_THREADSO) * i;
        Forthread128->rend = (matrixSize/NUM_THREADSO) * (i + 1);
        pthread_create(&threadsO[i], NULL, transpose, (void *)Forthread128);
	
    }

    for (int i = 0; i <NUM_THREADSO; i++) {
        pthread_join(threadsO[i], NULL);
    }

    timeTaken5 = omp_get_wtime() - time5;
    time5 = omp_get_wtime();
    Logicfortranspose(Matix);
    timeTaken5 = omp_get_wtime() - time5;
    
//Changing the size of the matrix to 1024
	struct parameters *ForthreadM2;	
	double time12,time22,time32,time42,time52, timeTaken12,timeTaken22,timeTaken32, timeTaken42,timeTaken52;
	time12 = omp_get_wtime();
    pthread_t threadsFM2[NUM_THREADSF];
     
    for (int i = 0; i < NUM_THREADSF; i++) {
        ForthreadM2 = malloc(sizeof(struct temp *));
        ForthreadM2->matrixtotranspose = Matix2;
        ForthreadM2->rbegin = (matrixSizetwo/NUM_THREADSF) * i;
        ForthreadM2->rend = (matrixSizetwo /NUM_THREADSF) * (i + 1);
        pthread_create(&threadsFM2[i], NULL, transpose, (void *)ForthreadM2);
    }

    for (int i = 0; i <NUM_THREADSF; i++) {
        pthread_join(threadsFM2[i], NULL);
    }
//printf("\nThe matrix size is changing to 1024\n");

    timeTaken12 = omp_get_wtime() - time12;
    time12 = omp_get_wtime();
    Logicfortranspose(Matix2);
    timeTaken12 = omp_get_wtime() - time12;
    

//    For 8 pthreads and matrix size 1024
	
	struct parameters *Forthread8M2;
	time22 = omp_get_wtime();
	pthread_t threadsEM2[NUM_THREADSE];
	for (int i = 0; i < NUM_THREADSE; i++) {
        Forthread8M2 = malloc(sizeof(struct temp *));
        Forthread8M2->matrixtotranspose = Matix2;
        Forthread8M2->rbegin = (matrixSizetwo/NUM_THREADSE) * i;
        Forthread8M2->rend = (matrixSizetwo/NUM_THREADSE) * (i + 1);
        pthread_create(&threadsEM2[i], NULL, transpose, (void *)Forthread8M2);
	
    }

    for (int i = 0; i <NUM_THREADSE; i++) {
        pthread_join(threadsEM2[i], NULL);
    }

    timeTaken22 = omp_get_wtime() - time22;
    time22 = omp_get_wtime();
    Logicfortranspose(Matix2);
    timeTaken22 = omp_get_wtime() - time22;
    
// For 16 pthreads and the matrix size 1024
	struct parameters *Forthread16M2;
	time32 = omp_get_wtime();
	pthread_t threadsSM2[NUM_THREADSS];
	for (int i = 0; i < NUM_THREADSS; i++) {
        Forthread16M2 = malloc(sizeof(struct temp *));
        Forthread16M2->matrixtotranspose = Matix2;
        Forthread16M2->rbegin = (matrixSizetwo/NUM_THREADSS) * i;
        Forthread16M2->rend = (matrixSizetwo/NUM_THREADSS) * (i + 1);
        pthread_create(&threadsSM2[i], NULL, transpose, (void *)Forthread16M2);
	
    }

    for (int i = 0; i <NUM_THREADSS; i++) {
        pthread_join(threadsSM2[i], NULL);
    }

    timeTaken32 = omp_get_wtime() - time32;
    time32 = omp_get_wtime();
    Logicfortranspose(Matix2);
    timeTaken32 = omp_get_wtime() - time32;
    
// For 64 pthreads and the matrix size 1024
	struct parameters *Forthread64M2;
	time42 = omp_get_wtime();
	pthread_t threadsSFM2[NUM_THREADSSF];
	for (int i = 0; i < NUM_THREADSSF; i++) {
        Forthread64M2 = malloc(sizeof(struct temp *));
        Forthread64M2->matrixtotranspose = Matix2;
        Forthread64M2->rbegin = (matrixSizetwo/NUM_THREADSSF) * i;
        Forthread64M2->rend = (matrixSizetwo/NUM_THREADSSF) * (i + 1);
        pthread_create(&threadsSFM2[i], NULL, transpose, (void *)Forthread64M2);
	
    }

    for (int i = 0; i <NUM_THREADSSF; i++) {
        pthread_join(threadsSFM2[i], NULL);
    }

    timeTaken42 = omp_get_wtime() - time42;
    time42 = omp_get_wtime();
    Logicfortranspose(Matix2);
    timeTaken42 = omp_get_wtime() - time42;
    
// For 128 pthreads and the matrix size 1024
	struct parameters *Forthread128M2;
	time52 = omp_get_wtime();
	pthread_t threadsOM2[NUM_THREADSO];
	for (int i = 0; i < NUM_THREADSO; i++) {
        Forthread128M2 = malloc(sizeof(struct temp *));
        Forthread128M2->matrixtotranspose = Matix2;
        Forthread128M2->rbegin = (matrixSizetwo/NUM_THREADSO) * i;
        Forthread128M2->rend = (matrixSizetwo/NUM_THREADSO) * (i + 1);
        pthread_create(&threadsOM2[i], NULL, transpose, (void *)Forthread128M2);
	
    }

    for (int i = 0; i <NUM_THREADSO; i++) {
        pthread_join(threadsOM2[i], NULL);
    }

    timeTaken52 = omp_get_wtime() - time52;
    time52 = omp_get_wtime();
    Logicfortranspose(Matix2);
    timeTaken52 = omp_get_wtime() - time52;
    

//Changing the size of the matrix to 8192
	struct parameters *ForthreadM3;	
	double time13,time23,time33,time43,time53, timeTaken13,timeTaken23,timeTaken33, timeTaken43,timeTaken53;
	time13 = omp_get_wtime();
        pthread_t threadsFM3[NUM_THREADSF];
     
    for (int i = 0; i < NUM_THREADSF; i++) {
        ForthreadM3 = malloc(sizeof(struct temp *));
        ForthreadM3->matrixtotranspose = Matix3;
        ForthreadM3->rbegin = (matrixSizethree/NUM_THREADSF) * i;
        ForthreadM3->rend = (matrixSizethree /NUM_THREADSF) * (i + 1);
        pthread_create(&threadsFM3[i], NULL, transpose, (void *)ForthreadM3);
    }

    for (int i = 0; i <NUM_THREADSF; i++) {
        pthread_join(threadsFM3[i], NULL);
    }
    //printf("\nThe matrix size is changing to 8192\n");

    time13 = omp_get_wtime();
    Logicfortranspose(Matix3);
    timeTaken13 = omp_get_wtime() - time13;
    
//    For 8 pthreads and matrix size 8192
	
	struct parameters *Forthread8M3;
	time23 = omp_get_wtime();
	pthread_t threadsEM3[NUM_THREADSE];
	for (int i = 0; i < NUM_THREADSE; i++) {
        Forthread8M3 = malloc(sizeof(struct temp *));
        Forthread8M3->matrixtotranspose = Matix3;
        Forthread8M3->rbegin = (matrixSizethree/NUM_THREADSE) * i;
        Forthread8M3->rend = (matrixSizethree/NUM_THREADSE) * (i + 1);
        pthread_create(&threadsEM3[i], NULL, transpose, (void *)Forthread8M3);
	
    }

    for (int i = 0; i <NUM_THREADSE; i++) {
        pthread_join(threadsEM3[i], NULL);
    }

    
    timeTaken23 = omp_get_wtime() - time23;
    time23 = omp_get_wtime();
    Logicfortranspose(Matix3);
    timeTaken23 = omp_get_wtime() - time23;
    

// For 16 pthreads and the matrix size 8192
	struct parameters *Forthread16M3;
	time33 = omp_get_wtime();
	pthread_t threadsSM3[NUM_THREADSS];
	for (int i = 0; i < NUM_THREADSS; i++) {
        Forthread16M3 = malloc(sizeof(struct temp *));
        Forthread16M3->matrixtotranspose = Matix3;
        Forthread16M3->rbegin = (matrixSizethree/NUM_THREADSS) * i;
        Forthread16M3->rend = (matrixSizethree/NUM_THREADSS) * (i + 1);
        pthread_create(&threadsSM3[i], NULL, transpose, (void *)Forthread16M3);
	
    }

    for (int i = 0; i <NUM_THREADSS; i++) {
        pthread_join(threadsSM3[i], NULL);
    }

    timeTaken33 = omp_get_wtime() - time33;
    time33 = omp_get_wtime();
    Logicfortranspose(Matix3);
    timeTaken33 = omp_get_wtime() - time33;
    
// For 64 pthreads and the matrix size 8192
	struct parameters *Forthread64M3;
	time43 = omp_get_wtime();
	pthread_t threadsSFM3[NUM_THREADSSF];
	for (int i = 0; i < NUM_THREADSSF; i++) {
        Forthread64M3 = malloc(sizeof(struct temp *));
        Forthread64M3->matrixtotranspose = Matix3;
        Forthread64M3->rbegin = (matrixSizethree/NUM_THREADSSF) * i;
        Forthread64M3->rend = (matrixSizethree/NUM_THREADSSF) * (i + 1);
        pthread_create(&threadsSFM3[i], NULL, transpose, (void *)Forthread64M3);
	
    }

    for (int i = 0; i <NUM_THREADSSF; i++) {
        pthread_join(threadsSFM3[i], NULL);
    }

    
    timeTaken43 = omp_get_wtime() - time43;
    time43 = omp_get_wtime();
    Logicfortranspose(Matix3);
    timeTaken43 = omp_get_wtime() - time43;
    
// For 128 pthreads and the matrix size 8192
	struct parameters *Forthread128M3;
	time53 = omp_get_wtime();
	pthread_t threadsOM3[NUM_THREADSO];
	for (int i = 0; i < NUM_THREADSO; i++) {
        Forthread128M3 = malloc(sizeof(struct temp *));
        Forthread128M3->matrixtotranspose = Matix3;
        Forthread128M3->rbegin = (matrixSizethree/NUM_THREADSO) * i;
        Forthread128M3->rend = (matrixSizethree/NUM_THREADSO) * (i + 1);
        pthread_create(&threadsOM3[i], NULL, transpose, (void *)Forthread128M3);
	
    }

    for (int i = 0; i <NUM_THREADSO; i++) {
        pthread_join(threadsOM3[i], NULL);
    }

    timeTaken53 = omp_get_wtime() - time53;
    time53 = omp_get_wtime();
    Logicfortranspose(Matix3);
    timeTaken53 = omp_get_wtime() - time53;
   
	printf("----------Printing out expected table-----------------------\n");
	printf("Pthreads	Matrix size 128 	Matrix size 1024 	Matrix size 8192\n");
	printf("%d		%fs		%fs		%fs\%",NUM_THREADSF, timeTaken1,timeTaken12,timeTaken13);
	printf("\n%d		%fs		%fs		%fs\%",NUM_THREADSE, timeTaken2,timeTaken22,timeTaken23);
	printf("\n%d		%fs		%fs		%fs\%",NUM_THREADSS, timeTaken3,timeTaken32,timeTaken33);
	printf("\n%d		%fs		%fs		%fs\%",NUM_THREADSSF, timeTaken4,timeTaken42,timeTaken43);
	printf("\n%d		%fs		%fs		%fs\n\%",NUM_THREADSO, timeTaken5,timeTaken52,timeTaken53);
    //printf("\n%fHz     fsec%   %f     %f      %f\%%", f, period,t1,t2,duty);     	

    return 0;
}

