#include<iostream>
#include<vector>
#include<string>

using namespace std;

void PrintSolution(int **,int i,int j,string X,string Y);
int main()
{
  string X,Y;
  cout<<"Input string X and Y: "<<endl;
  //cin>>X>>Y;
  X="ABCBDAB";
  Y="BDCABA";

  int sizeX=X.size()+1;
  int sizeY=Y.size()+1;


  int **array=new int*[sizeX];
  for(int i=0;i<sizeX;++i){
     array[i]=new int[sizeY];
  }

  for(int i=0;i<sizeX;++i){
     array[i][0]=0;
  }

  for(int j=0;j<sizeY;++j){
     array[0][j]=0;
  }

  for(int i=1;i<sizeX;++i){
     for(int j=1;j<sizeY;++j){
        if(X[i-1]==Y[j-1]){
           array[i][j]=array[i-1][j-1]+1;
        }
        else if(array[i][j-1]>array[i-1][j]){
           array[i][j]=array[i][j-1];
        }
        else{
           array[i][j]=array[i-1][j];
        }
     }
   }

   cout<<"The longest common subsequence of X and Y is: "<<array[sizeX-1][sizeY-1]<<endl;
   PrintSolution(array,sizeX-1,sizeY-1,X,Y);
   return 0;
}

void PrintSolution(int **array,int i,int j,string X, string Y){
     if(i==0 or j==0)
        return;
     if(X[i-1]==Y[j-1]){
           PrintSolution(array,i-1,j-1,X,Y);
           cout<<X[i-1]<<" ";
        }
        else if(array[i][j-1]>array[i-1][j]){
           PrintSolution(array,i,j-1,X,Y);
        }
        else{
           PrintSolution(array,i-1,j,X,Y);
        }
}
