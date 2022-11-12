#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#define N 2048
#define BLOCK_SIZE 512
#define FactorIntToDouble 1.1;

double firstMatrix [N] [N] = {0.0};
double secondMatrix [N] [N] = {0.0};
double matrixMultiResult [N] [N] = {0.0};



void matrixMulti() {
	int i, j, k = 0;
	int i2, j2, k2 = 0;
	for (i2 = 0 ; i2 < N; i2+=BLOCK_SIZE) {
		for (j2 = 0; j2 < N; j2+=BLOCK_SIZE) {
			for (k2 = 0; k2 < N; k2+=BLOCK_SIZE) {
				//inside each block
				for (i = i2; i < i2+BLOCK_SIZE; i++) {
					for (j = j2; j < j2+BLOCK_SIZE; j++) {
						double resultValue = 0;
						for (k = k2; k < k2+BLOCK_SIZE; k++) {
							resultValue += firstMatrix[i][k] * secondMatrix[k][j];
						}
						matrixMultiResult[i][j] = resultValue;
					}
				}

			}
		}
	}
}

void matrixInit()
{
    for(int row = 0 ; row < N ; row++ ) {
        for(int col = 0 ; col < N ;col++){
            srand(row+col);
            firstMatrix [row] [col] = ( rand() % 10 ) * FactorIntToDouble;
            secondMatrix [row] [col] = ( rand() % 10 ) * FactorIntToDouble;
        }
    }
}
int main()
{
	matrixInit();

	double time_spent = 0.0;
	clock_t begin = clock();

	matrixMulti();

	clock_t end = clock();
	printf("Time elpased is %ld", end-begin);

	return 0;
}



