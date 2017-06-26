#include<iostream>
#include<vector>
#include<string>

using namespace std;

int computeLCS(int **,int,int,string,string);
void PrintSolution(int **,int,int,string,string);

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

   computeLCS(array,sizeX-1,sizeY-1,X,Y);
   cout<<"The longest common subsequence of X and Y is: "<<array[sizeX-1][sizeY-1]<<endl;
   PrintSolution(array,sizeX-1,sizeY-1,X,Y);
}


int computeLCS(int **array,int i,int j,string X,string Y){
    if(i==0 or j==0){
        array[i][j] = 0;
        return 0;
    }

    if(X[i-1]==Y[j-1]){
           array[i][j]=computeLCS(array,i-1,j-1,X,Y)+1;
        }
        else if(computeLCS(array,i,j-1,X,Y)>computeLCS(array,i-1,j,X,Y)){
           array[i][j]=computeLCS(array,i,j-1,X,Y);
        }
        else{
           array[i][j]=computeLCS(array,i-1,j,X,Y);
        }
    return array[i][j];
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

