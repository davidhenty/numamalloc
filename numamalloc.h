void *numamalloc(size_t itemsize, size_t nitem,
		 MPI_Comm *nodecomm, MPI_Win *nodewin);
void *numafree(void *mem);
