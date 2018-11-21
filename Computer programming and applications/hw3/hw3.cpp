/*
   This code can be compiled and run ok.
    
  Given data1 and data2
  First separate map to MxN 
  then calculate Wbar、Q and E
  choose which area should set facility.
  Finally, use Greedy Algorithm and
  choose the area which can satisfied 
  more demand from the area we got.
   
   usage (how to run):
     ./hw3 (then input data1 or data2)

   input file:
      data1.txt data2.txt

   output file:
     none

   compile (how to compile):
     g++ -o hw3 hw3.cpp

   pseudocode:
     First input file and then store value to M,N,Q*,L,R
	   and W[],C[]
     then print them 
     Secaondly, Calculate Wbar and Q,select the area should chose
     Finally, use Greedy Algorithm to set other new facilities
     and use InsertionSort to Sort satisified area.


   coded by Yu-Che Wei, ID: H34064090, email:dessertn626@gmail.com
   date: 2018.04.10
*/
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <cmath>
#include <cstdlib>
using namespace std ;
 //insertionbSort函式
  void insertSort(int arr[], int size){
    //arr是要排序的陣列，size是陣列大小
  for(int next=1; next<size; next++){//next要移動的數的位置
    int insert = arr[next];//insert = 暫存要移動的數
    int move = next;//move = 最大移動次數

    while( (move>0) && (arr[move-1]>insert) ){
      //條件一:(move>0)避免移動到arr[-1]
      //條件二:左數大於右數的時候，將右數覆寫成左數
      arr[move] = arr[move-1];
      move--;
    }
    arr[move] = insert;
    //把要移動的數插入到正確的位置上
  }
 }
int main() {
  struct Zone{ //此為各網格之 struct
        int a; //儲存某網格之縱軸座標，即由上而下第 a 列, a=1,…,M
        int b; //儲存某網格之橫軸座標，即由左而右第 b 行, b=1,…,N
        double W, //儲存某網格之自身需求量
               TW,//儲存某網格之暫時需求量             
               C, //儲存某網格之設施建置成本
               TC,//儲存某網格之暫時設施建置成本
               Wbar=0, //儲存某網格可涵蓋之鄰近 R 範圍內之需求量總和         
                Q; //儲存某網格之設施建置評估值 = Wbar/C
        bool isQ, //儲存某網格是否合格得以建置新設施，合格則為 1，否則為 0
             isF, //儲存某網格是否被挑選來建置新設施，是則為 1，否則為 0
             isB; //儲存某網格是否已被指派其專屬之設施，是則為 1，否則為 0
  };
  int M, //區域縱軸座標範圍(1,2,…,M)
      N, //區域橫軸座標範圍(1,2,…,N)
      U, //合格之候選網格個數，計算完各網格之設施建置評估值才可得到
      L, //欲選擇設立之設施個數
      R; //設施涵蓋距離範圍，使用曼哈頓距離量測
  double Qstar; //給定的設施建置評估值
  string fileName;//檔案名稱   
  fstream file;//宣告fstream物件
  Zone *V; //動態宣告 MN 長度之網格 struct 陣列;V[k],k=0,1,…,MN-1
  int *VF; //動態宣告 MN 長度之設施編號陣列;VF[l],l=0,1,…,MN-1
  bool **E; //動態宣告U MN × 長度的二維布林陣列 E[U][MN]，用於記錄是否
            //候選點 l=0,…,U-1 是否與網格 k=0,..,MN-1 有專屬負責關係
  int arr[1000]={0};//用存滿足的網格

  for(int i=0;i<1000;i++)arr[i]=-1;      
   while(1){
       cout<<"Enter fileName: ";
       cin>>fileName;
       file.open(fileName,ios::in);
     if(file.is_open()){
       break;
     }
      cout<<"The file does not exist!! "<<endl;
   }
   file>>M>>N>>Qstar>>L>>R;
   cout<<"M="<<M<<"; "<<"N="<<N<<"; "<<"Q*="<<Qstar<<"; "
   <<"L="<<L<<"; "<<"R="<<R<<";"<<endl;

     V=new Zone [M*N];//動態宣告V[]
     VF=new int [M*N];//動態宣告VF[]

   //將W,C先行存入暫時陣列TW,TC
     for(int i=0;i<(M*N);i++)
       file>>V[i].TW;
     for(int j=0;j<(M*N);j++)
       file>>V[j].TC;
    file.close();

    //印出W[]
      cout<<setw(7)<<"W";
   for(int k=1;k<=N;k++){
      cout<<setw(7)<<k;
   }
      cout<<endl;
   int f=0;   
   for(int m=1;m<=M;m++){
       cout<<setw(7)<<m;
       for(int n=0;n<N;n++){
            V[n].W=V[f].TW;          
            cout<<setw(7)<<V[n].W;
            f++;
     }
      cout<<endl;//印完一行(N個)W就換行
   }
    cout<<endl;//隔開W跟C
  
   //印出C[]
      cout<<setw(7)<<"C";
   for(int k=1;k<=N;k++){
      cout<<setw(7)<<k;
   }
   int g=0;
   cout<<setw(7)<<endl;
   for(int m=1;m<=M;m++){
         cout<<setw(7)<<m;
         for(int n=0;n<N;n++){
             V[n].C=V[g].TC;
             cout<<setw(7)<<V[n].C;
                g++;
       }
         cout<<endl;//印完一行(N個)C就換行
   }

   int k=0; //編號k=0,1,...MN-1
   //算座標
    for(int i=1;i<=M;i++){
      for(int j=1;j<=N;j++){
        V[k].b=k%N+1;//橫軸座標V[0].b=1, V[1].b=2...V[M].b=M
        V[k].a=(k-V[k].b+1)/N+1;//縱軸座標V[1].a=1, V[2].a=2..V[N].a=N
        k++;
    }  
   }  

   int temp; //存曼哈頓距離
   //算出Wbar
   for (int i=0;i<M*N;i++){
       V[i].Wbar=V[i].TW;
        for (int j=0;j<M*N;j++){
           temp=abs(V[j].a-V[i].a)+abs(V[j].b-V[i].b);
           if ((i!=j)&&(temp<=R))
                   V[i].Wbar+=V[j].TW;
        }
    }

     //算出Q
    for(int i=0;i<N*M;i++){
      V[i].Q=V[i].Wbar/V[i].TC;
    }
   cout<<endl; 

  //分出合格網格有哪些並且將編號存入VF[],將合格網格數量存入U
  //並且令V[].isQ為真  否則另為假
  int vfindex=0;
   for(int i=0;i<M*N;i++){
     if(V[i].Q>=Qstar){
       VF[vfindex]=i;
       vfindex++;
       V[i].isQ=true; 
     }
     else{
       U=vfindex;
       V[i].isQ=false;
     }
   } 
    cout<<endl; 

    //印出U和V
    cout<<setw(3)<<"U="<<U<<", "<<"VF: ";
    for(int i=0;i<U;i++){
      cout<<VF[i]<<" ";
    }
    cout<<endl;
    cout<<endl;

    //印出Wbar
     cout<<setw(7)<<"W_bar";
   for(int k=1;k<=N;k++){
      cout<<setw(7)<<k;
   }
      cout<<endl;
   int z=0;   
   for(int m=1;m<=M;m++){
       cout<<setw(7)<<m;
       for(int n=0;n<N;n++){          
         cout<<setw(7)<<fixed<<setprecision(1)<<V[z].Wbar;
            z++;
     }
      cout<<endl;//印完一行(N個)Wbar就換行
   }
     cout<<endl;

    //印出Q  
   cout<<setw(7)<<"Q";
   for(int k=1;k<=N;k++){
      cout<<setw(7)<<k;
   }
      cout<<endl;
   
   int s=0;
   for(int m=1;m<=M;m++){
       cout<<setw(7)<<m;
       for(int n=0;n<N;n++){          
           cout<<setw(7)<<fixed<<setprecision(2)<<V[s].Q;
            s++;
     }
      cout<<endl;//印完一行(N個)Q就換行
   } 

      E=new bool *[U] ;//動態宣告E[U][MN]
     for(int i=0;i<U;i++)
      E[i]=new bool [M*N];

     //判定是否VF[]與其他網格的距離<=R
     //若是則E[][]=true 否則=false
     //並且令V[].isF=true  否則=false
     for(int i=0;i<U;i++){
      for(int w=0;w<M*N;w++){
        if((abs(V[VF[i]].a-V[w].a)+abs(V[VF[i]].b-V[w].b))<=R){
          E[i][w]=true;
          V[w].isF=true;
        }
        else{
          E[i][w]=false;
          V[w].isF=false;
        }
      }
     }
      cout<<endl;
     //印出E[][]
     cout<<setw(7)<<"E";
     for(int l=0;l<M*N;l++){
        cout<<setw(3)<<l;
     }
       cout<<setw(7)<<endl;

     for(int m=0;m<U;m++){
       cout<<setw(7)<<m;
         for(int w=0;w<M*N;w++){
         cout<<setw(3)<<E[m][w];
         }
         cout<<endl;
      }
      cout<<endl;

    //貪婪演算法
    //step1
    double WbarMax=0,TSDemand=0;//TSDemand=Total Satisfied Demands
    int lMax=0;
    for(int h=1;h<=L;h++){
      cout<<"Iteration "<<h<<": select Facility ";
      for(int l=0;l<U;l++){//從U中挑出Wbar最大的
                 if(V[VF[l]].Wbar>WbarMax){
                   WbarMax=V[VF[l]].Wbar;
                   lMax=l;
                 }
      }
     for(int i=0;i<U;i++)
    {
      V[VF[i]].Wbar=0;
    }
           TSDemand+=WbarMax;
           V[VF[lMax]].isF=true;
           V[VF[lMax]].isB=true;
    //reset value
      for(int u=0;u<U;u++){
          for(int i=0;i<M*N;i++){
             if(V[i].isB!=1 && E[u][i]==0 ){
              V[i].W=0;
              V[i].isB=true;
              E[VF[lMax]][i]=true;
             }
         }
      }

     //step2 
    //算Wbar
   int temp2;
   for (int i=0;i<M*N;i++){
       V[i].Wbar=V[i].TW;
        for (int j=0;j<M*N;j++){
           temp2=abs(V[j].a-V[i].a)+abs(V[j].b-V[i].b);
           if ((i!=j)&&(temp2<=R))
                   V[i].Wbar+=V[j].TW;
        }
    }
    
   //算E
     for(int i=0;i<U;i++){
      for(int w=0;w<M*N;w++){
        if((abs(V[VF[i]].a-V[w].a)+abs(V[VF[i]].b-V[w].b))<=R){
          E[i][w]=true;
        }
        else{
          E[i][w]=false;
        }
      }
    }
      cout<<VF[lMax]<<", satisfied  "<<setprecision(1)<<WbarMax<<"  demands for zone: ";

             for(int i=0;i<M*N;i++){
                  if((abs(V[VF[lMax]].a-V[VF[i]].a)+abs(V[VF[lMax]].b-V[VF[i]].b))<=R && V[VF[i]].isF==0){
                          V[VF[i]].isB=true;
                          arr[i]=VF[i];
                             for(int k=0;k<U;k++){
                             E[VF[k]][i]=false;
                             }
                  } 
             }
             //使用InsertionSort從小到大排列
                insertSort(arr,1000);
                   for(int b=0;b<1000;b++){
                        if(arr[b]==-1)continue;
                          cout<<arr[b]<<",";
                     }
  }
   cout<<"Total satisfied demands:  "<<TSDemand<<endl; 
    return 0 ;  
  }


