#include<iostream>

using namespace std;

int CutRod(int priceTable[],int revenueTable[],int cutPosition[],int length);
void PrintCutRodSolution(int cutPosition[],int length);

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
   int *cutPosition=new int[length+1];  //where to cut the first piece
   for(int i=0;i<=length;++i)
      revenueTable[i]=-1;
   cutPosition[0]=0;

   int revenue=CutRod(priceTable,revenueTable,cutPosition,length);
   
   cout<<"The maximal revenue is "<<revenue<<endl;
   PrintCutRodSolution(cutPosition,length);

   return 0;
}

int CutRod(int priceTable[],int revenueTable[],int cutPosition[],int length){
   revenueTable[0]=0;
   for(int i=1;i<=length;++i){
       int revenue=-1;
       for(int j=1;j<=i;++j){
          int tmp=priceTable[j-1]+revenueTable[i-j];
          if(tmp>revenue){
             revenue=tmp;
             cutPosition[i]=j;
          }
       }
       revenueTable[i] = revenue;
   }
   return revenueTable[length];
}


void PrintCutRodSolution(int cutPosition[],int length){
    cout<<"The cut solution is "<<endl;
    while(length>0){
       cout<<cutPosition[length]<<" ";
       length-=cutPosition[length];
    }
    cout<<endl;
}
