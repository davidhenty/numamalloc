#include <stdlib.h>
#include <stdio.h>
#include <numaif.h>
#include <mpi.h>

#include "numamalloc.h"
#include "utilities.h"

int getnumanode(void *ptr);
  
int main(int argc, char **argv)
{
  int disp_unit, rank, size;
  
  int *x, *y;
  
  int n, i, i0, i1, irank;

  n = 8*1024*1024;
  // n = 5;

  MPI_Init(NULL, NULL);  

  // Find size and rank

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  printlocation();

  x = (int *) numamalloc(sizeof(int), n);
  y = (int *) malloc(sizeof(int)*n);

  // Initialise

  for (i=0; i < n; i++)
    {
      x[i] = i;
      y[i] = i;
    }

  for (irank=0; irank < size; irank++)
    {
      i0 = irank*((n+size-1)/size);
      i1 = i0 + ((n+size-1)/size) - 1;
      
      if (i1 > n-1) i1 = n-1;
	  
      printf("rank %d: x[%d] = %d on numa node %d\n",
	      rank, i0, x[i0], getnumanode(&x[i0]));
      printf("rank %d: x[%d] = %d on numa node %d\n",
	      rank, i1, x[i1], getnumanode(&x[i1]));
      printf("rank %d: y[%d] = %d on numa node %d\n",
	      rank, i0, y[i0], getnumanode(&y[i0]));
      printf("rank %d: y[%d] = %d on numa node %d\n",
	      rank, i1, y[i1], getnumanode(&y[i1]));
    }

  numafree(x);

  // Finish

  MPI_Finalize();
}

int getnumanode(void *ptr)
{
  int numanode = -1;
  get_mempolicy(&numanode, NULL, 0, (void*)ptr, MPOL_F_NODE | MPOL_F_ADDR);
  return numanode;
}
