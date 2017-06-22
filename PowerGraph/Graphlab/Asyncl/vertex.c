#include<stdio.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<iostream>
#include<sys/wait.h>
#include<sys/sem.h>

using namespace std;

double absolute(double a,double b);

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


// P operation:
//	若信号量值为1，获取资源并将信号量值-1 
//	若信号量值为0，进程挂起等待
int sem_p(int sem_id,int sem_num)
{
	struct sembuf sbuf;
	sbuf.sem_num = sem_num; /*序号*/
	sbuf.sem_op = -1; /*P操作*/
	sbuf.sem_flg = SEM_UNDO;

	if(semop(sem_id, &sbuf, 1) == -1)
	{
		perror("P operation Error");
		return -1;
	}
	return 0;
}

// V operation：
//	释放资源并将信号量值+1
//	如果有进程正在挂起等待，则唤醒它们
int sem_v(int sem_id,int sem_num)
{
	struct sembuf sbuf;
	sbuf.sem_num = sem_num; /*序号*/
	sbuf.sem_op = 1;  /*V操作*/
	sbuf.sem_flg = SEM_UNDO;

	if(semop(sem_id, &sbuf, 1) == -1)
	{
		perror("V operation Error");
		return -1;
	}
	return 0;
}



int main(int argc,char* argv[])
{
   key_t key;
   int shmid;
   int sem_write_id;
   int sem_read_id;
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


   double rank=1.0;
   double delta = 0.01;
   double difference = 1.0;

   //cout<<"iteration begins"<<endl;
   //while(difference > delta)
   for(int k=0;k<10000;++k)
   {
        double new_rank = 0.15;
        for(int i=0;i<degree_in;++i){
            shared = (struct shared_use_st*)shm + sizeof(struct shared_use_st)*degree_in_no[i];
            double rank_value = 0.0;

            sem_p(sem_read_id,degree_in_no[i]);//get read lock
            cout<<"get read lock"<<endl;
            shared->readcount++;
            if(shared->readcount == 1)
               sem_p(sem_write_id,degree_in_no[i]);//get write lock
            sem_v(sem_read_id,degree_in_no[i]);//release read lock

            rank_value = shared->rank_value;//perform reading

            sem_p(sem_read_id,degree_in_no[i]);//get read lock
            shared->readcount--;
            if(shared->readcount == 0)
               sem_v(sem_write_id,degree_in_no[i]);//release write lock
            sem_v(sem_read_id,degree_in_no[i]);//release read lock
          
            new_rank += (rank_value / shared->out_degree)*0.85;
        }
        shared = (struct shared_use_st*)shm + sizeof(struct shared_use_st)*vertex_no;

        sem_p(sem_write_id,vertex_no);//get write lock
        shared->rank_value = new_rank;
        sem_v(sem_write_id,vertex_no);//release write lock

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

