#include<iostream>

using namespace std;

int CutRod(int priceTable[],int length);

int main(){
   int tableSize;
   cout<<"Input size of price table:"<<endl;
   cin>>tableSize;
   
   int *priceTable=new int[tableSize];
   cout<<"Input price orderly:"<<endl;
   for(int i=0;i<tableSize;++i)
      cin>>priceTable[i];
 
   int length=0;
   cout<<"Input length of the rod:"<<endl;
   while(cin>>length){
      if(length<=tableSize)
         break;
      else
         cout<<"Length can not be bigger than size of price table, input again:"<<endl;
   }
  
   int revenue=CutRod(priceTable,length);
   
   cout<<"The maximal revenue is "<<revenue<<endl;
   return 0;
}

int CutRod(int priceTable[],int length){
   int revenue=-1;

   if(length==0)
      return 0;

   for(int i=1;i<=length;++i){
      int tmp=CutRod(priceTable,length-i)+priceTable[i-1];
      if(tmp>revenue)
          revenue=tmp;
   }

   return revenue;
}
