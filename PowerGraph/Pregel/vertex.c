#include<stdio.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<iostream>
#include<iomanip>
//to create a unique key
#define MSG_FILE "/etc/passwd"

using namespace std;

void PregelPageRank(double &rank,double total_msg);

double Combiner(double message[],int count);

double absolute(double a,double b);

struct msg_form {
     long mtype;
     char value[256];
};

int main(int argc, char* argv[])
{
     int msqid;
     key_t key;
     struct msg_form msg;
     
  
     int neighbors_in = atoi(argv[3]);
     int neighbors_out = atoi(argv[4]);
     double rank = atof(argv[2]);
     int vertex_no = atoi(argv[1]);

     double *message_in = new double[neighbors_in];
     int *neighbors_out_num = new int[neighbors_out];
      
     for(int i=0; i<neighbors_out; ++i){
          neighbors_out_num[i] = atoi(argv[i+5]);
     }

     //cout<<vertex_no<<" "<<rank<<" " <<neighbors_in<<" "<<neighbors_out<<endl;


     //get key value
     if((key = ftok(MSG_FILE,'z')) < 0)
     {
           perror("ftok error");
           exit(1);
     }

     //printf("Message Queue - Server key is: %d.\n", key);

     //create message queue
     if ((msqid = msgget(key, IPC_CREAT|0777)) == -1)
     {
           perror("msgget error");
           exit(1);
     }

     // printf("My msqid is: %d.\n", msqid);
     //printf("My pid is: %d.\n", getpid());
     cout<<"|        "<<vertex_no-2<<"         |      "<<getpid()<<"      |"<<endl;
     
     //initial scatter 
     for(int i=0;i<neighbors_out;++i)
     {
            msg.mtype = neighbors_out_num[i];
            sprintf(msg.value,"%f",rank/neighbors_out);
            msgsnd(msqid,&msg,256,0);
     }
   

     while(1) 
     {
           //receive a message from the launch then begin a new iteration
           msgrcv(msqid, &msg,256,vertex_no+10000,0); 

           if(atof(msg.value) < 0.75)
           {
                cout<<"|        "<<vertex_no-2<<"         |      "<<std::left<<setw(8)<<rank<<"      |"<<endl;
                fflush(stdout);
                break;
           }
       
           //get all the incoming message 
           for(int i=0;i<neighbors_in;++i)
           {
                msgrcv(msqid, &msg, 256,vertex_no, 0);
                message_in[i] = atof(msg.value);
                //cout<<"message_in[i] is "<<message_in[i]<<endl;
           }

           double total_msg = Combiner(message_in, neighbors_in);
           double new_rank=0.0;
           PregelPageRank(new_rank,total_msg);
           double difference = absolute(rank,new_rank);
           rank = new_rank;
           //cout<<"Rank for vertex "<<vertex_no<<"now is "<<rank<<endl;
           fflush(stdout);
         
           for(int i=0;i<neighbors_out;++i)
           {
                msg.mtype = neighbors_out_num[i];
                sprintf(msg.value,"%f",rank/neighbors_out);
                msgsnd(msqid,&msg,256,0);
           }

           msg.mtype = 1;
           if(difference < 0.01)
               sprintf(msg.value,"%f",0.5);  //vote to halt;
           else
               sprintf(msg.value,"%f",1.0); //vote to continue;

           msgsnd(msqid,&msg,256,0);
           
     }
     delete message_in,neighbors_out_num;//added
     return 0;
}


void PregelPageRank(double &rank,double total_msg)
{
     rank = 0.15 + 0.85*total_msg;
}

double Combiner(double message[],int count){
     double total_msg = 0.0;
     for(int i=0; i<count; ++i)
          total_msg += message[i];
     return total_msg;
}
 
double absolute(double a,double b)
{
     if (a>b)
        return a-b;
     else
        return b-a;
}
