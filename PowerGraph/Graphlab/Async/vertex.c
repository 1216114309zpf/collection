#include<stdio.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<iostream>
#include<sys/wait.h>

using namespace std;

double absolute(double a,double b);

struct shared_use_st
{
   double rank_value;
   int out_degree;
};

int main(int argc,char* argv[])
{
   key_t key;
   int shmid;
   void *shm = NULL;
   int vertex_count = atoi(argv[1]);
   int vertex_no = atoi(argv[2]);
   int degree_in = atoi(argv[3]);
   int *degree_in_no = new int[degree_in];
   for(int i=0;i<degree_in;++i)
        degree_in_no[i] = atoi(argv[i+4]);
   
   struct shared_use_st *shared = NULL;

   //get key
   if((key = ftok(".", 'z')) < 0)
   {
       perror("ftok error");
       exit(1);
   }

   cout<<"|        "<<vertex_no<<"         |      "<<getpid()<<"      |"<<endl;
  
    //create shared memory
   shmid = shmget(key, sizeof(struct shared_use_st)*vertex_count,0666|IPC_CREAT);
   if(shmid == -1)
   {
       fprintf(stderr,"shmget failed\n");
       exit(EXIT_FAILURE);
   }
  
   //attach shared memory to current process
   shm = shmat(shmid,0,0);
   if(shm == (void*)-1)
   {
       fprintf(stderr,"shmat failed\n");
       exit(EXIT_FAILURE);
   }

   //set shared memory
   shared = (struct shared_use_st*)shm;

   double rank=1.0;
   double delta = 0.01;
   double difference = 1.0;

   while(difference > delta)
   //for(int k=0;k<1000;++k)
   {
        double new_rank = 0.15;
        for(int i=0;i<degree_in;++i){
            shared = (struct shared_use_st*)shm + sizeof(struct shared_use_st)*degree_in_no[i];
            new_rank += (shared->rank_value / shared->out_degree)*0.85;
        }
        shared = (struct shared_use_st*)shm + sizeof(struct shared_use_st)*vertex_no;
        shared->rank_value = new_rank;
        difference = absolute(rank,new_rank);
        rank = new_rank; 
   }
  
     
}

double absolute(double a,double b)
{
     if (a>b)
        return a-b;
     else
        return b-a;
}

