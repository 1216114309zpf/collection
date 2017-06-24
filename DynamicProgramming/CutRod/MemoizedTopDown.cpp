#include<iostream>

using namespace std;

int MemoizedCutRod(int priceTable[],int revenueTable[],int length);

int main(){
   int tableSize;
   cout<<"Input size of price table:"<<endl;
   cin>>tableSize;
   
   int *priceTable=new int[tableSize];
   cout<<"Input price orderly:"<<endl;
   for(int i=0;i<tableSize;++i){
      cin>>priceTable[i];
   }
 
   int length=0;
   cout<<"Input length of the rod:"<<endl;
   while(cin>>length){
      if(length<=tableSize)
         break;
      else
         cout<<"Length can not be bigger than size of price table, input again:"<<endl;
   }
  
   int *revenueTable=new int[length+1];
   for(int i=0;i<=length;++i)
      revenueTable[i]=-1;
   int revenue=MemoizedCutRod(priceTable,revenueTable,length);
  
   
   cout<<"The maximal revenue is "<<revenue<<endl;
   return 0;
}

int MemoizedCutRod(int priceTable[],int revenueTable[],int length){
   int revenue=-1;

   if(revenueTable[length]>0)
      return revenueTable[length];

   if(length==0)
      revenue=0;

   for(int i=1;i<=length;++i){
      int tmp=MemoizedCutRod(priceTable,revenueTable,length-i)+priceTable[i-1];
      if(tmp>revenue)
          revenue=tmp;
   }

   revenueTable[length]=revenue;

   return revenue;
}
