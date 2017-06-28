#include<iostream>
#include<vector>

using namespace std;

void RecursiveActivitySelector(int* start,int *finish,vector<int> &result,int begin,int end);

int main()
{
   int start[] = {0,1,3,0,5,3,5,6,8,8,2,12};
   int finish[] = {0,4,5,6,7,9,9,10,11,12,14,16};
   vector<int> result;

   RecursiveActivitySelector(start,finish,result,0,11);

   cout<<"Chosen activities are :";
   for(int i=0;i<result.size();++i)
      cout<<result[i]<<" ";
   cout<<endl;
   
   return 0;
}

void RecursiveActivitySelector(int* start,int *finish,vector<int> &result,int begin,int end)
{
   int current=begin+1;
   while(current<=end and start[current]<finish[begin])
       ++current;

   if(current<=end){
       result.push_back(current);
       RecursiveActivitySelector(start,finish,result,current,end);
   }
}
