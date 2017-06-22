#include<stdio.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<iostream>
#include<sys/wait.h>
#include<iomanip>
#include<sys/sem.h>

using namespace std;

struct shared_use_st
{
   double rank_value;
   int out_degree;
   int readcount;
};



// union used to initiliaze semctl
union semun
{
        int              val; /*for SETVAL*/
        struct semid_ds *buf;
        unsigned short  *array;
};

//initialize semaphore 
int init_sem(int sem_id, int value, int count)
{
        union semun tmp;
        tmp.val = value;
        for(int i=0;i<count;++i)
        {
            if(semctl(sem_id, i, SETVAL, tmp) == -1)
            {
                perror("Init Semaphore Error");
                return -1;
            }
        }
        return 0;
}


//delete semaphore set 
int del_sem(int sem_id)
{
        union semun tmp;
        if(semctl(sem_id, 0, IPC_RMID, tmp) == -1)
        {
                perror("Delete Semaphore Error");
                return -1;
        }
        return 0;
}


int main()
{
   key_t key;
   int sem_write_id;
   int sem_read_id;
   int vertex_count = 10;
   int shmid;
   void *shm = NULL;
   struct shared_use_st *shared = NULL;
   //int *out_degree_array = new int[vertex_count];
   //out_degree_array[0] = 2;
   //out_degree_array[1] = 0;
   //out_degree_array[2] = 0;

 
   int *out_degree_array = new int[vertex_count];
   out_degree_array[0]=1;
   out_degree_array[1]=1;
   out_degree_array[2]=4;
   out_degree_array[3]=1;
   out_degree_array[4]=3;
   out_degree_array[5]=1;
   out_degree_array[6]=1;
   out_degree_array[7]=3;
   out_degree_array[8]=2;
   out_degree_array[9]=2;
   //get key
   if((key = ftok(".", 'z')) < 0)
   {
       perror("ftok error");
       exit(1);
   }
  
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
   //printf("\nMemory attached at %X\n",shm);
  
   //set shared memory
   shared = (struct shared_use_st*)shm;

   cout<<endl;
   printf("Shared memory id is: %d.\n\n", shmid);
   cout<<"....................................."<<endl;
   cout<<"|     Vertex no    |     Pid        |"<<endl;
   cout<<"....................................."<<endl;
   cout<<"|     launcher     |      "<<getpid()<<"      |"<<endl;
   
   //initialize shared memory
   for(int i=0;i<vertex_count;++i)
   {
        shared = (struct shared_use_st*)shm + sizeof(struct shared_use_st)*i;
        shared->rank_value = 1.0;
        shared->out_degree = out_degree_array[i];
        shared->readcount = 0;
   }

   // get write key value
   if((key = ftok(".", 'p')) < 0)
   {
        perror("ftok error");
	exit(1);
   }

   //create semaphore write set 
   if((sem_write_id = semget(key,vertex_count , IPC_CREAT|0666)) == -1)
   {
	perror("semget error");
	exit(1);
   }

   //get read key value;
   if((key = ftok(".", 'f')) < 0)
   {
        perror("ftok error");
        exit(1);
   }


   //create semaphore read set
   if((sem_read_id = semget(key,vertex_count , IPC_CREAT|0666)) == -1)
   {
        perror("semget error");
        exit(1);
   }

   init_sem(sem_write_id,1,vertex_count);
   init_sem(sem_read_id,1,vertex_count);
   
   //name,vertex_count,vertex_no,degree_in.....NULL;
   // char* argv1[] = {"vertex","3","0","0",NULL};
   //char* argv2[] = {"vertex","3","1","1","0",NULL};
   //char* argv3[] = {"vertex","3","2","1","0",NULL};
   //char** argv[] = {argv1,argv2,argv3};
  
    char* argv0[] = {"vertex","10","0","1","2",NULL};
    char* argv1[] = {"vertex","10","1","1","2",NULL};
    char* argv2[] = {"vertex","10","2","2","0","1",NULL};
    char* argv3[] = {"vertex","10","3","1","2",NULL};
    char* argv4[] = {"vertex","10","4","5","2","3","5","6","7",NULL};
    char* argv5[] = {"vertex","10","5","1","4",NULL};
    char* argv6[] = {"vertex","10","6","1","4",NULL};
    char* argv7[] = {"vertex","10","7","3","4","8","9",NULL};
    char* argv8[] = {"vertex","10","8","2","7","9",NULL};
    char* argv9[] = {"vertex","10","9","2","7","8",NULL};
    char** argv[] = {argv0,argv1,argv2,argv3,argv4,argv5,argv6,argv7,argv8,argv9};

   //launch all the vertex
   for(int i=0;i<vertex_count;++i)
   {
        if(fork() == 0)
        {
             execvp("/home/victor/Graphlab/Asyncl/vertex",argv[i]);
             fflush(stdout);
             exit(errno);
        }
   }
   
   int return_val;
   while(wait(&return_val) > 0);

   cout<<"....................................."<<endl<<endl<<endl;
   cout<<"........................................."<<endl;
   cout<<"|     Page no      |      Rank          |"<<endl;
   cout<<"........................................."<<endl;

   for(int i=0;i<vertex_count;++i)
   {
        shared = (struct shared_use_st*)shm + sizeof(struct shared_use_st)*i;
        //cout<<"The final rank for vertex "<<i<<" is "<<shared->rank_value<<endl;
        cout<<"|        "<<i<<"         |      "<<std::left<<setw(8)<<shared->rank_value<<"      |"<<endl;
        fflush(stdout);
   }

   cout<<"........................................."<<endl<<endl;

   //detach shared memoryA
   if(shmdt(shm) == -1)
   {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
   }

   //delete shared memory
   if(shmctl(shmid,IPC_RMID,0) == -1)
   {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
   }

   //delete semaphore set
   del_sem(sem_write_id);
   del_sem(sem_read_id);

   return 0;
}
