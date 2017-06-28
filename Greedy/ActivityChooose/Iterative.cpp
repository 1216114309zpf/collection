#include<iostream>
#include<vector>

using namespace std;


int main()
{
   int start[] = {0,1,3,0,5,3,5,6,8,8,2,12};
   int finish[] = {0,4,5,6,7,9,9,10,11,12,14,16};//sorted already
   vector<int> result;

   int current=1;
   int chosen=0;
   while(current<=11){
      if(start[current] >= finish[chosen]){
          result.push_back(current);
          chosen=current;
      } 
      ++current;
   }
   cout<<"Chosen activities are :";
   for(int i=0;i<result.size();++i)
      cout<<result[i]<<" ";
   cout<<endl;
   
   return 0;
}

