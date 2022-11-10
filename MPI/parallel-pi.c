#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#define NUMSTEPS 1000000

double rectangle(double startpt, int rec_count, double width)
{
	int i;
	double x, local_sum, estimate = 0.0;
	x = 0.5*width + startpt;

	for (i= 0; i <= rec_count-1; i++){
		x += width;
		estimate += 4.0/(1.0+x*x);
	}
	local_sum = width*estimate;
	return local_sum;
}
int main() {
	int comm_sz, my_rank;
	struct timespec start, end;
	
	double pi = 0.0;
	double startpt, width;
	double local_sum;
	int source, local_numsteps;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	
	clock_gettime(CLOCK_MONOTONIC, &start);
	width = 1.0 / (double)NUMSTEPS;
	local_numsteps = NUMSTEPS/comm_sz;
	startpt = my_rank*local_numsteps*width;
	local_sum = rectangle(startpt, local_numsteps, width);
	if (my_rank != 0)
	{
		MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else 
	{
		pi = local_sum;
		//printf("local_sum: %0.20f\n", local_sum);
		for (source = 1; source < comm_sz; source++)
		{
			MPI_Recv(&local_sum, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//printf("local_sum: %0.20f\n", local_sum);
			pi += local_sum;
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &end);
	u_int64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	if (my_rank == 0)
	{
		printf("PI is %.20f\n",pi);
		printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
	}
	MPI_Finalize();
	return 0;
}
