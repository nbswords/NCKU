/*
   This code can be compiled and run ok.

   這個作業將會模擬病毒生長、擴散、原生並且模擬投藥後的結果。
   首先透過讀檔設定M*N個網格, 在沒有投藥的情況下每個網格都會有生長、擴散、原生。
   每一期都要印出生長Q1 擴散Q2 原生Q3 與最後Q1,Q2,Q3相加而成的Q0網格
   接下來每一期都讓使用者輸入將要投藥的次數,並逐一輸入每次要投的縱向座標和橫向座標、藥劑撲殺率、藥劑影響範圍
   而在網格有投藥的情況下,生長和擴散將會停止,而原生則是不受投藥影響。
   最後輸出每一期投藥後的Q1 Q2 Q3 與最後的Q0=(Q1+Q2+Q3)*殘存率r

   usage (how to run):
    input n_M投藥次數
    並且輸入每個n_M中要投藥的縱向座標 row、橫向座標 col、藥劑撲殺率 X、藥劑影響範圍 R

   input file:
     data1.txt

   output file:
     none

   compile (how to compile):
     g++ -o hw6 hw6.cpp

   pseudocode:
   First use readFile function to read the data and define some dynamic array.
   Then print as request.
   Second,
   Print Original Q0 with color,
   Calculate Q1 and print.
   Calculate Q2 and print.
   Calculate Q3 and print.
   Calculate Q0=Q1+Q2+Q3 and print Q0 with color.
   Finally,
   in every t
   let users input n_M and row col X R
   then 
   in every t    
   Calculate Q1 and print.
   Calculate Q2 and print.
   Calculate Q3 and print.
   Calculate Q0=(Q1+Q2+Q3)*r and print Q0 with color.

   coded by Che-Wei Yu, ID: H34064090, email: dessertn626@gmail.com
   date: 2018.6.12
*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <random>
#include <cstdlib>
#include <cmath>
using namespace std ;
struct Zone{ //此為各網格之 struct
int a; //儲存該網格之縱軸座標，即由上而下第 a 列, a=0,…,M
int b; //儲存該網格之橫軸座標，即由左而右第 b 行, b=0,…,N
int id;//儲存其編號，id=0,…,MN-1
float r=1.; //儲存該網格本期之殘存率，初始化為 1，若該網格有投藥，則將有 X 比例被撲殺，亦即殘留了 1-X 比例。
            //倘若另有一網格 k’其所投放的藥效範圍會波及本網格，則會將網格
            //k’對本網格的殘餘量（每多一單位則殺傷效果減半）拿來乘以本網格原先的殘餘量，以此類推。
bool isM; //儲存某網格於本期期初是否被投藥，是則為 1，否則為 0
};
struct Virus{
int F; //原生病毒出現之強度(或可理解為數量)
int p; //原生病毒該強度之發生機率(%)
};
//讀檔+初始化動態陣列
void readFile(char *filename,int &M,int &N,int &T,float &s,float &q,int &n_Vir,
Zone *&V,Virus *&Vir,float *&Q0,float *&Q1,float *&Q2,float *&Q3){
  fstream file;
  while(1){
   cout<<"Enter fileName: ";
   cin>>filename;
   file.open(filename);
    if(file.is_open()){
      break;
    }
    cout<<"The file does not exist!! "<<endl;
  }
  file>>M>>N>>T>>s>>q>>n_Vir;
  V=new Zone [M*N];
  Vir=new Virus [n_Vir];
  Q0=new float [M*N](); //()是為了將Q0,Q1,Q2,Q3初始化為0
  Q1=new float [M*N]();
  Q2=new float [M*N]();
  Q3=new float [M*N]();
  for(int i=0;i<n_Vir;i++){ file>>Vir[i].F>>Vir[i].p; }
  for(int j=0;j<M*N;j++){
    file>>Q0[j];
    V[j].b=j%N+1;
    V[j].a=((j*V[j].b+1)/N)+1;
    V[j].id=j;
    V[j].r=1;
    V[j].isM=0;
  }
}

//印出有顏色的Q0
void print_colored_2Darray(float *Q0,int &M,int &N){
  cout<<"Q0[k,0]:"<<endl;
  cout<<"V"<<setw(7);
  for(int i=0;i<N;i++){cout<<setw(7)<<i;}
  cout<<endl;
for(int i=0;i<M;i++){
  cout<<i<<setw(7);
    for(int j=0;j<N;j++){
       if(Q0[i*N+j]>=0 && Q0[i*N+j]<33)
         cout<<"\x1b[;32;1m"<<setw(7)<<fixed<<setprecision(1)<<Q0[i*N+j]<<"\x1b[0m"<<flush;
       else if(Q0[i*N+j]>=33 && Q0[i*N+j]<66)
         cout<<"\x1b[;33;1m"<<setw(7)<<fixed<<setprecision(1)<<Q0[i*N+j]<<"\x1b[0m"<<flush;
       else if(Q0[i*N+j]>=66 && Q0[i*N+j]<100)
         cout<<"\x1b[;31;1m"<<setw(7)<<fixed<<setprecision(1)<<Q0[i*N+j]<<"\x1b[0m"<<flush;
       else
         cout<<"\x1b[;35;1m"<<setw(7)<<fixed<<setprecision(1)<<Q0[i*N+j]<<"\x1b[0m"<<flush;
    }
    cout<<endl; //印完一行(N個)Q0就換行
 }
}
//計算成長量
void Growth(float *&Q0,float *&Q1,int &M,int &N,float &s,bool &Throw,int &row,int &col,float &X,int &R){
    //如果有投藥
  if(Throw==true){
    for(int i=0;i<M*N;i++){ 
      if( (row*N+col) == i){
         s=0;//投藥後該網格的s=0
         Q1[row*N+col]=Q0[i]*(1+s/100);
      }
      else
      Q1[i]=Q0[i]*(1+s/100); 
    }
  }
    //如果沒投藥
  if(Throw==false){
    for(int i=0;i<M*N;i++)
    Q1[i]=Q0[i]*(1+s/100);
  }
}
//計算擴散量
void spread(float *&Q0,float *&Q2,int &M,int &N,float &q,bool &Throw,int &row,int &col,float &X,int &R){
  //如果有投藥
  if(Throw==true){
    int TempX,TempY;//X,Y坐標
    for(int i=0;i<M*N;i++){
      if(Q0[i]>0){
        TempY=i%N; //Y坐標
        TempX=(i-TempY)/N;//X坐標    

         if(i==row*N+col){
            q=0;//有投藥的網格 q=0
            Q2[row*N+col]+=Q0[i]*(q/100);
         }
         else{
            //上  (x-1,y)不變  而編號=X*N+Y
            if(TempX>0)
             Q2[(TempX-1)*N+TempY]+=Q0[i]*(q/100);
            //下  (x+1,y)不變
            if(TempX<(M-1)) 
             Q2[(TempX+1)*N+TempY]+=Q0[i]*(q/100);
             //左  (x不變,y-1)
            if(TempY>0) 
             Q2[TempX*N+TempY-1]+=Q0[i]*(q/100);  
             //右  (x不變,y+1)
            if(TempY<(N-1)) 
             Q2[TempX*N+TempY+1]+=Q0[i]*(q/100);
         }
      } 
    }
  }
  //如果沒投藥
  if(Throw==false){
    int TempX,TempY;//X,Y坐標
    for(int i=0;i<M*N;i++){
      if(Q0[i]>0){
        TempY=i%N; //Y坐標
        TempX=(i-TempY)/N;//X坐標
            //上  (x-1,y)不變  而編號=X*N+Y
            if(TempX>0)
             Q2[(TempX-1)*N+TempY]+=Q0[i]*(q/100);
            //下  (x+1,y)不變
            if(TempX<(M-1)) 
             Q2[(TempX+1)*N+TempY]+=Q0[i]*(q/100);
             //左  (x不變,y-1)
            if(TempY>0) 
             Q2[TempX*N+TempY-1]+=Q0[i]*(q/100);  
             //右  (x不變,y+1)
            if(TempY<(N-1)) 
             Q2[TempX*N+TempY+1]+=Q0[i]*(q/100);
      } 
    }
  }
}
//計算原生量
void Autogenerate(float *&Q3,int &M,int &N,int &n_Vir,Virus *&Vir){
  srand((unsigned)time(NULL)); //Random seed
  int Rand;  //隨機亂數
  int count; //計算區間
  for(int i=0;i<M*N;i++){
    Rand = rand()%100; //0~99中挑1個
    count=0; //計算區間
    for(int j=0;j<n_Vir;j++){ 
      if(Rand>=count && Rand<(count+Vir[j].p)){
         Q3[i]=Vir[j].F;
         break;
      }
      else
         count=count+Vir[j].p;
    }
  }
}
//印出單色的Q1、Q2或Q3  
//記得呼叫函式前要cout<<"Q1orQ2orQ3[k,0]:"<<endl;
void print_mono_2Darray(float *Q,int &M,int &N){ //此處 Q1 亦可適用 Q2,Q3
  cout<<"V"<<setw(7);
  for(int i=0;i<N;i++){cout<<setw(7)<<i;}
  cout<<endl;
for(int i=0;i<M;i++){
  cout<<i<<setw(7);
    for(int j=0;j<N;j++){
         cout<<setw(7)<<fixed<<setprecision(1)<<Q[i*N+j];
    }
    cout<<endl; //印完一行(N個)Q0就換行
 }
}
//殘存量
void Residual(Zone *&V,int &M,int &N,int &row,int &col,float &X,int &R){
  if(R==0){
    for(int i=0;i<M*N;i++){
       if((row*N+col) == i)
          V[i].r=100-X;
       else
          V[i].r=100;
    }   
  }

  else{
     for(int i=0;i<M*N;i++){
        if((row*N+col) == i)
           V[i].r=100-X;
        else{
           if(( abs( V[i].a-row )+abs( V[i].b-col ) ) <R ){
              for(int j=R;j==0;j--)
                    V[j].r=100 - X/pow(2,R);
           }
       }
     }
  }

}

int main() {
int   M, //區域縱軸座標範圍(1,2,…,M)
      N, //區域橫軸座標範圍(1,2,…,N)
      n_Vir, //原生病毒出現之種類個數，譬如圖二(b)之 n_Vir=3 代表 3 種病毒強度，分別為 60,40,0 機率 10%,20%,70%
      T; //模擬之總期數
Zone *V; //動態宣告 MN 長度之網格 struct 陣列;V[k],k=0,1,…,MN-1
Virus *Vir; //動態宣告 n_Vir 長度之原生病毒陣列;
float *Q0,//Q0[k],k=0,…,MN-1 存網格 k 於本期期末總病毒數量 Q0=(Q1+Q2+Q3)*r
      *Q1,//Q1[k]存 k 之上期 Q0 於本期成長所衍生的總病毒數量 Q1=Q0*(1+s)
      *Q2,//Q2[k]存 k 之相鄰網格上期 Q0 於本期擴散移入之病毒總量 Q2+=鄰格 Q0*q
      *Q3,//Q3[k]儲存網格 k 經過本期而新隨機出現的原生病毒數量，為某個強度 F
        s, //給定的病毒每期成長比率(%)
        q; //給定的病毒每期擴散比率(%)
int n_M, //記錄當期投藥次數，若為 0 則代表不投藥
    row, //記錄當下投藥之網格縱軸座標
    col, //記錄當下投藥之網格橫軸座標
    R; //記錄當下投藥之影響範圍，以曼哈頓距離表示，若為 0 則代表僅影響那一格
float  X; //記錄當下投藥之撲殺率(%)
bool Throw;//用來傳入函式  判斷是否有投藥 ---(其實就是老師規定的isM 但Zone放在陣列V太麻煩)
char filename[50];//記錄輸入之測試檔名
readFile(filename,M,N,T,s,q,n_Vir,V,Vir,Q0,Q1,Q2,Q3);
cout<<"M="<<M<<", N="<<N<<", T="<<T<<", s="<<s<<"%, q="<<q<<"%, n_Vir="<<n_Vir<<"; F={";
    for(int i=0;i<n_Vir;i++){
      cout<<Vir[i].F<<",";
    }
cout<<"}"<<", p={";
    for(int j=0;j<n_Vir;j++){
      cout<<Vir[j].p<<",";
    }
cout<<"};"<<endl;
//印出原始Q0
print_colored_2Darray(Q0,M,N);
//-----------End of Part1 ----------------------------------
for(int t=0;t<T;t++){
    cout<<"#################################################"<<endl;//隔開每一期
    cout<<"Please input n_M"<<endl;
    cin>>n_M;       
    if(n_M==0){break;}
    Throw=true; //表示有投藥
    for(int n=0;n<n_M;n++){
    cout<<"Please input row,col,X,R you want to throw drug"<<endl;
    cin>>row>>col>>X>>R;

    //計算&印出成長量
    Growth(Q0,Q1,M,N,s,Throw,row,col,X,R);
    cout<<"Q1[k,0]:"<<endl;
    print_mono_2Darray(Q1,M,N);
    //計算&印出成擴散量
    spread(Q0,Q2,M,N,q,Throw,row,col,X,R);
    cout<<"Q2[k,0]:"<<endl;
    print_mono_2Darray(Q2,M,N);
    //計算&印出原生量
    Autogenerate(Q3,M,N,n_Vir,Vir);
    cout<<"Q3[k,0]:"<<endl;
    print_mono_2Darray(Q3,M,N);
    //Q0=(Q1+Q2+Q3)*r 並且印出彩色的new Q0
    for(int i=0;i<M*N;i++){Q0[i]=(Q1[i]+Q2[i]+Q3[i])*V[i].r;}
    print_colored_2Darray(Q0,M,N);
  }
}
Throw=false; //表示沒投藥
//計算&印出成長量
Growth(Q0,Q1,M,N,s,Throw,row,col,X,R);
cout<<"Q1[k,0]:"<<endl;
print_mono_2Darray(Q1,M,N);
//計算&印出成擴散量
spread(Q0,Q2,M,N,q,Throw,row,col,X,R);
cout<<"Q2[k,0]:"<<endl;
print_mono_2Darray(Q2,M,N);
//計算&印出原生量
Autogenerate(Q3,M,N,n_Vir,Vir);
cout<<"Q3[k,0]:"<<endl;
print_mono_2Darray(Q3,M,N);
//Q0=Q1+Q2+Q3 並且印出彩色的new Q0
for(int i=0;i<M*N;i++){Q0[i]=Q1[i]+Q2[i]+Q3[i];}
print_colored_2Darray(Q0,M,N);

return 0 ;  
}