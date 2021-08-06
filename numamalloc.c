#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

static MPI_Win  nodewin  = MPI_WIN_NULL;
static MPI_Comm nodecomm = MPI_COMM_NULL;

void *numamalloc(size_t itemsize, size_t nitem)
{
  int i, rank, nodesize, noderank;
  MPI_Aint winsize;
  int nlocal, disp;
  void *x, *x0;
  char *s;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Create node-local communicator if needed

  if (nodewin != MPI_WIN_NULL)
    {
      printf("numamalloc: ERROR, double call\n");
      return NULL;
    }
  else
    {
      MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, rank,
			  MPI_INFO_NULL, &nodecomm);

      MPI_Comm_size(nodecomm, &nodesize);
      MPI_Comm_rank(nodecomm, &noderank);
    }

  // Allocate a shared array, contiguous across processes
  // Work out how much to allocate locally

  nlocal = (nitem+nodesize-1)/nodesize;

  // Maybe we need to truncate a bit?

  if (nlocal*(noderank+1) > nitem)
    {
      nlocal = nitem - nlocal*noderank;
      if (nlocal < 0) nlocal = 0;
    }

  //  printf("rank %d, noderank %d allocating %d items of size %d\n",
  //          rank, noderank, nlocal, itemsize);

  winsize = nlocal*itemsize;
  disp    = itemsize;

  MPI_Win_allocate_shared(winsize, disp,
              MPI_INFO_NULL, nodecomm, &x, &nodewin);

  // Perform first touch locally to ensure correct allocation

  s = (char *) x;

  for (i=0; i < nlocal*itemsize; i++)
    {
      s[i] = '\0';
    }

  // Return the same pointer to each process, i.e. from rank 0

  MPI_Win_shared_query(nodewin, 0, &winsize, &disp, &x0);
  return x0;
}

void *numafree(void *mem)
{
  if (nodewin == MPI_WIN_NULL)
    {
      printf("numamalloc: ERROR, double call\n");
      return NULL;
    }

  MPI_Win_free(&nodewin);
  MPI_Comm_free(&nodecomm);
}
