#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cassert>
#include <time.h>
#include <mpi.h>
#include<vector>

using namespace std;

static int ndegree;
static int size;

void parseCommandLine(int argc, char** const argv);

int main(int argc, char* argv[])
{
    srand(time(NULL));
    MPI_Init(&argc, &argv);
    int nprocs,me;
	
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
	char *sbuf_, *rbuf_;
    MPI_Request *sreq_, *rreq_;
	parseCommandLine(argc, argv);
	
	sbuf_ = new char[size];
    rbuf_ = new char[size];
	
	sreq_ = new MPI_Request[ndegree];
    rreq_ = new MPI_Request[ndegree];

    //std::vector<int> targets_, sources_;
	  std::vector<vector<int>> targets_,sources_;
    for(int i=0;i<nprocs;i++)
    {
		int t=0;
                std::vector<int>temp;
		while(t!=ndegree){
		int r = rand() % nprocs;
		if(r!=i){
                   temp.push_back(r);
		t++;
		 }
		}
	        sources_.push_back(temp);

                t=0;
                std::vector<int>temp1;
                while(t!=ndegree){
                int r = rand() % nprocs;
                if(r!=i){
                   temp1.push_back(r);
                t++;
                 }
                }

                targets_.push_back(temp1);
		
    }
    
    //debugging
    /*for(int i=0;i<nprocs;i++)
    {
        printf("%d : ",i);
        for(int j=0;j<ndegree;j++)
        {
             printf("%d ",sources_[i][j]);
        }
        printf("\n");
     }*/
	
	if(me%2==0)
	{
		for (int p = 0; p < ndegree; p++)
	    {
            int t = rand() % ( 90 - 65 + 1 ) + 65;
            sbuf_[p]=t;
			printf("[Process %d] I send the value %c to %d.\n", me, sbuf_[p],targets_[me][p]);
			MPI_Isend(sbuf_,size, MPI_CHAR, targets_[me][p], 100, MPI_COMM_WORLD, sreq_ + p);
	    }
		MPI_Waitall(ndegree, sreq_, MPI_STATUSES_IGNORE);
	}
	else
	{
		
		for (int p = 0; p < ndegree; p++)
	    {
            int t = rand() % ( 90 - 65 + 1 ) + 65;
            rbuf_[p]=t;
			MPI_Irecv(rbuf_, size, MPI_CHAR, sources_[me][p], 100, MPI_COMM_WORLD, rreq_ + p);
			printf("[Process %d] The MPI_Irecv completed. I received the value %c from %d.\n",me,rbuf_[p],sources_[me][p]);
	    }
        MPI_Waitall(ndegree, rreq_, MPI_STATUSES_IGNORE);
	}
	
    // MPI_Finalize();      		
}

void parseCommandLine(int argc, char** const argv)
{
  int ret;
  optind = 1;

  while ((ret = getopt(argc, argv, "d:s:")) != -1) {
    switch (ret) {
    case 'd':
      ndegree = atoi(optarg);
      break;
     case 's':
      size = atoi(optarg);
      break;
	default:
      assert(0 && "Should not reach here!!");
      break;
    }
  }
}
