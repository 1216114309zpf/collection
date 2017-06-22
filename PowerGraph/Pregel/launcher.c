#include<stdio.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<iostream>
#include<sys/wait.h>

using namespace std;

//to create a unique key
#define MSG_FILE "/etc/passwd"

struct msg_form {
     long mtype;
     char value[256];
};

int main()
{
     int vertex_num = 10;
     int msqid;
     key_t key;
     struct msg_form msg;

     //get key value
     if((key = ftok(MSG_FILE,'z')) < 0)
     {
           perror("ftok error");
           exit(1);
     }
     

     //create message queue
     if ((msqid = msgget(key, IPC_CREAT|0777)) == -1)
     {
           perror("msgget error");
           exit(1);
     }

     cout<<endl;
     printf("Shared message queue id is: %d.\n\n", msqid);
     cout<<"....................................."<<endl;
     cout<<"|     Vertex no    |     Pid       |"<<endl;
     cout<<"....................................."<<endl;
     //printf("launcher pid is: %d.\n", getpid());
     cout<<"|     launcher     |      "<<getpid()<<"      |"<<endl;

     //char* argv1[] = {"vertex","2","1.0","0","2","3","4",NULL};
     //char* argv2[] = {"vertex","3","1.0","1","0",NULL};
     //char* argv3[] = {"vertex","4","1.0","1","0",NULL};
     //char** argv[] = {argv1,argv2,argv3};
     //name,vertex_no,initial_val,neighbor_in,neighbor_out,...NULL
     char* argv0[] = {"vertex","2","1.0","1","1","4",NULL};
     char* argv1[] = {"vertex","3","1.0","1","1","4",NULL};
     char* argv2[] = {"vertex","4","1.0","2","4","2","3","5","6",NULL};
     char* argv3[] = {"vertex","5","1.0","1","1","6",NULL};
     char* argv4[] = {"vertex","6","1.0","5","3","7","8","9",NULL};
     char* argv5[] = {"vertex","7","1.0","1","1","6",NULL};
     char* argv6[] = {"vertex","8","1.0","1","1","6",NULL};
     char* argv7[] = {"vertex","9","1.0","3","3","6","10","11",NULL};
     char* argv8[] = {"vertex","10","1.0","2","2","9","11",NULL};
     char* argv9[] = {"vertex","11","1.0","2","2","9","10",NULL};
     char** argv[] = {argv0,argv1,argv2,argv3,argv4,argv5,argv6,argv7,argv8,argv9};

     for(int i=0; i<vertex_num; ++i){
          if( fork() == 0 ) {
               //execvp("/home/victor/Pregel/vertex",argv);
               execvp("/home/victor/Pregel/Pregel/vertex",argv[i]);
               fflush(stdout);
               exit( errno );
          }
     }
   
     bool flag=false;
     int iteration = 0;
     while(1) // when all vertex vote to halt, break;
     {
         ++iteration;
         for(int i=0;i<vertex_num;++i)
         {
              msg.mtype = i+2+10000;
              if(!flag)
                 sprintf(msg.value,"%f",1.0);
              else
                 sprintf(msg.value,"%f",0.5);
              msgsnd(msqid, &msg,256,0);             
         }


         if(flag)
         {
              //print result;
              break;
         }

         flag = true;
         
         for(int i=0;i<vertex_num;++i)
         {
             msgrcv(msqid, &msg, 256,1,0);
             if(atof(msg.value) > 0.75)
                 flag = false; 
         }
       
         if(iteration == 1){
              cout<<"....................................."<<endl<<endl<<endl;
              cout<<"........................................."<<endl;
              cout<<"|     Page no      |      Rank          |"<<endl;
              cout<<"........................................."<<endl;

         }
     }
     
    int returnVal;
    while(wait(&returnVal) > 0);
    cout<<"........................................."<<endl<<endl;
    cout<<"The final page rank is calculated through "<<iteration<<" iterations"<<endl<<endl;

    //delete message queue
    if(msgctl(msqid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  
    //exit(EXIT_SUCCESS);
    return 0; 
}
