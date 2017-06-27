#include<iostream>

using namespace std;
void PrintSolution(int **help,int left,int right);
int ComputeCost(int **array,int**help,int*matrix,int line,int col);

int main(){
   int matrix[7] = {30,35,15,5,10,20,25}; //two adjacent numbers represent a matrix
   int size=7;
   int **array=new int*[6];
   int **help=new int*[6];

   for(int i=0;i<6;++i){
       array[i]=new int[6];
       help[i]=new int[6];
   }

   for(int i=0;i<6;++i)
      for(int j=0;j<6;++j)
         array[i][j]=-1;
  
   ComputeCost(array,help,matrix,0,5);
   cout<<"The minimal cost is "<<array[0][5]<<endl;
   PrintSolution(help,0,5);
   cout<<endl;
}

void PrintSolution(int **help,int left,int right)
{
   if(left == right)
     cout<<"A"<<left+1;
   else{
     cout<<"(";
     PrintSolution(help,left,help[left][right]);
     PrintSolution(help,help[left][right]+1,right);
     cout<<")";
   }
}


int ComputeCost(int **array,int **help,int *matrix,int line,int col){
   if(line==col){
      array[line][col]=0;
   }
   
   if(array[line][col] >=0)
      return array[line][col];

   else{
      array[line][col]=-1;
         for(int k=line;k<col;++k){
            int tmp=ComputeCost(array,help,matrix,line,k) + ComputeCost(array,help,matrix,k+1,col) + matrix[line] * matrix[k+1] * matrix[col+1];
            if(array[line][col]==-1){
                array[line][col]=tmp;
                help[line][col]=line;
            }
            else{
               if(tmp<array[line][col]){
                  array[line][col] = tmp;
                  help[line][col] = k;
                }
            }
         }
   
   }
  
   return array[line][col];
        
}
