#include<iostream>
#include<vector>
#include<string>

using namespace std;

int computeLCS(int **,int,int,string,string);

int main()
{
  string X,Y;
  cout<<"Input string X and Y: "<<endl;
  cin>>X>>Y;

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
}
