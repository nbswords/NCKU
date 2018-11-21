/*
   This code can be compiled and run ok.
   
   modify 1,2,3 part and calculate the real minimum cost
   
   usage (how to run):
     input ./hw2_4
     and then input data1.txt or data2.txt
   
   input file:
     data1.txt
     data2.txt

   output file:
     none

   compile (how to compile):
     g++ -o hw2_4 hw2_4.cpp

   pseudocode:
     declare some new array we will need
       initalize i integer = 1 ,j integer = 0
       keep i+1 and j+1 until j=T
          intitialize f integer = 0,k integer = 0,d integer = 0
          keep f+1 and k+2 and d+10 until f=standard deviation of D
            Store every (original s - 2*D_sig)+k in s[f] (k表示等差2) 
            Store every original Q - 50 + d in Q[j]  (d表示等差10)

            and use s[] substitue s 
                    Q[] substitue Q
            finally 
            store first TC in temp
            if temp<MinTC
               then TC become MinTC
               then BC become MinBC
               then IC become MinIC
               then PC become MinPC
               then OC become MinOC
             else
               maintain same TC,BC,PC,OC

    the others maintain same with Part1,2,3 
 -------------------------------------------------------
      initalize i integer = 1 ,j integer = 0
       keep i+1 and j+1 until j=T
        initialize V integer = 2
       keep V+2 until V=6
        initialize k integer =0,d integer =0 ,q integer =1
        keep k+1 d+2 q+2 until k=7
          Store every 1+d in R[k]
          Store every q*average of D in S[k]

          
          and use R[] substitue R 
              S[] substitue S

            finally 
            store first TC in temp
            if temp<MinTC
               then TC become MinTC
               then BC become MinBC
               then IC become MinIC
               then PC become MinPC
               then OC become MinOC
             else
               maintain same TC,BC,PC,OC

    the others maintain same with Part1,2,3 

   coded by Che-Wei YU, ID: H34064090, email: dessertn626@gmail.com
   date: 2018.03.19
*/
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <cmath>
using namespace std;
int main(){

   int T, //規劃期間為 T 天
       H, //單位持有成本
       B, //單位缺貨成本
       A, //單次訂購成本
       C, //單位進貨成本
       d=0,//Qt的等差
       so, //原本的再訂購點 s original
       Qo, //原本的經濟訂購批量 Q original
       Ro, //原本的訂購週期
       Vo, //原本的預期期初庫存量
       temp,
       MinTC=0,
       MinBC=0,
       MinPC=0,
       MinOC=0,
       MinIC=0;


   int D[32]={0}, //儲存第 i(=1,…,31)天的預測需求量，先初始化為 0
       I[32]={0}, //儲存第 i(=0,…,31)天的期末存貨量，先初始化為 0
       L[32]={0}, //儲存第 i(=0,…,31)天的期末缺貨量，先初始化為 0
       Y[32]={0}, //儲存第 i(=0,…,30)天的訂購決定，先初始化為 0
       O[32]={0}, //儲存第 i(=1,…,31)天的期初進貨量，先初始化為 0
       s[13]={0}, //將再訂購點改為陣列
       Q[11]={0}, //將經濟訂購批量改為陣列
       R[8]={0},//將訂購週期改為陣列
       S[8]={0};//將預期期初庫存量改為陣列
       char fileName[50];//儲存檔案名稱的陣列     
       fstream file;//宣告fstream物件
       int TI=0,//總I[i] i從1到T
           TL=0,//總L[i] i從1到T
           TY=0,//總Y[i] i從0到T-1
           TO=0,//總O[i] i從1到T
           IC=0,//存貨成本
           BC=0,//缺貨成本
           OC=0,//訂購成本
           PC=0,//進貨成本
           TC=0;//總成本
  //開啟檔案
  cout << "Enter FileName: ";
  cin >> fileName;
file.open(fileName, ios::in);//以讀取模式開啟檔案
  
file>>T;
for(int i=1;i<=T;i++){
    if(file>>D[i]);   
}  
file>>H>>B>>A>>C>>Vo>>Ro>>I[0]>>L[0];

file.close();

int Dtotal=0;//儲存D的總量   

 for(int i=1;i<=T;i++){
    Dtotal+=D[i];
 }
int D_bar=0;//D的平均
 D_bar=Dtotal/T;

int D_sig=0;//D的標準差sigma
int sspow=0;//sum of squares(ss)離均差的平方 
            //離均差:每筆資料和平均數的差
  for(int i=1;i<=T;i++){ //先算出離均差平方的總合
       sspow+=(pow((D[i]-D_bar),2));
  }
  
  D_sig=sqrt(sspow/(T-1));//再把離均差平方的總合除以T-1再開根號

  so=D_bar+D_sig;//計算再訂購點

  Qo=sqrt(2*A*Dtotal/H);//計算經濟訂購批量 

  //-----------------------(St,Qt)-------------------------
 
 for(int i=1,j=0;j<=T;i++,j++){
    for(int f=0,k=0;f<=D_sig;f++,k+=2,d+=10){//測試D_sig+1組st 
      s[f]=(so-2*D_sig)+k;//st等差是2
      Q[j]=Qo-50+d;//Qt等差是10

        if((I[i-1]+O[i])<D[i]){//如果缺貨
           Y[i]=1;
           L[i]=D[i]-I[i-1]-O[i];//缺貨量
           I[i]=0;
          if(I[j]<=s[f]){//(ST,QT)訂購策略
            O[j+1]=Q[j];
            I[i]=O[j+1]+I[i-1]-D[i];
            L[i]=0; 
           }
           int LD=0;//缺貨天數 lack days
           LD++;
           B*=LD;//缺貨成本  
           A*=LD;//訂購成本  
        }
        else{ //不缺貨的話
           Y[i]=0;
           I[i]=I[i-1]+O[i]-D[i];
           L[i]=0;  
        }
        if((I[i-1]+O[i])>D[i]){//滯貨的話
            int DD=0; //滯貨天數 demurrage days
              DD++;
              H*=DD;//持有成本             
        }
        TI+=I[j];//總I
        TL+=L[j];//總L
        TO+=O[j];//總O

       TI-=45; //減掉第0天的I
       for(int j=0;j<T;j++)
       TY+=Y[j];//總Y    

      IC=H*TI;//算出IC
      BC=B*TL;//算出BC
      OC=A*TY;//算出OC
      PC=C*TO;//算出PC     
      
      TC=IC+BC+PC+OC;//算出TC
      temp=TC;
       if(temp<MinTC){
         MinTC=TC; 
         MinIC=IC;
         MinBC=BC;
         MinPC=PC;
         MinOC=OC;
       }
       else{
         MinTC=TC; 
         MinIC=IC;
         MinBC=BC;
         MinPC=PC;
         MinOC=OC;
      }     
     }   
   }
    cout<<"Total cost: "<<MinTC<<" = "<<MinIC<<"(IC)"<<" + "<<MinBC<<"(BC)"<<
      " + "<<MinOC<<"(OC)"<<" + "<<MinPC<<"(PC)"<<endl; 

//-----------------------(R,S)-------------------------
       
       //重新初始化

           Y[32]={0}, 
           O[32]={0}; 
           TI=0,//總I[i] i從1到T
           TL=0,//總L[i] i從1到T
           TY=0,//總Y[i] i從0到T-1
           TO=0,//總O[i] i從1到T
           MinTC=0; 
           MinIC=0;
           MinBC=0;
           MinPC=0;
           MinOC=0;
           IC=0,//存貨成本
           BC=0,//缺貨成本
           OC=0,//訂購成本
           PC=0,//進貨成本
           TC=0;//總成本

      D[0]=0;
      I[0]=45;

 for(int i=1,j=0;j<=T;i++,j++){  
    for(int V=2;V<=6;V+=2){
     for(int k=0,d=0,q=1;k<8;k++,d+=2,q+=2){ 
        R[k]=1+d;
        S[k]=q*D_bar;

          if((j==V)||(j==V+R[k])||(j==V+2*R[k])||(j==V+3*R[k])||(j==V+4*R[k])){//第V日、V+nR日進貨(n=1~4)              
                Y[j]=1;
                O[i-1]=S[k]-I[i-2];
                I[i-1]=O[i-1]+I[i-2]-D[i-1]; 
                L[j]=0;                        
                int OD=0;//訂購天數
                OD++;
                A*=OD;//訂購成本       
          }          
           //非訂購日
           Y[i]=0;
           I[i]=I[i-1]+O[i]-D[i];     
         
         if((I[i-1]+O[i])<D[i]){//如果缺貨
           L[j+1]=D[j+1]-I[j]-O[j+1];//缺貨量
           I[i]=0;
           int LD=0;//缺貨天數 lack days
           LD++;
           B*=LD;//缺貨成本  
         }
         if((I[i-1]+O[i])>D[i]){//滯貨的話
            int DD=0; //滯貨天數 demurrage days
              DD++;
              H*=DD;//持有成本             
         }
        TI+=I[j];//總I
        TL+=L[j];//總L
        TO+=O[j];//總O
      }
    }
  }     
       TI-=45; //減掉第0天的I
      
       for(int j=0;j<T;j++)
       TY+=Y[j];//總Y
       
       IC=H*TI;//算出IC
       BC=B*TL;//算出BC
       OC=A*TY;//算出OC
       PC=C*TO;//算出PC    
 
      TC=IC+BC+PC+OC;//算出TC

      temp=TC; //暫時儲存TC
       if(temp<MinTC){
         MinTC=TC; 
         MinIC=IC;
         MinBC=BC;
         MinPC=PC;
         MinOC=OC;
       }
       else
         MinTC=TC; 
         MinIC=IC;
         MinBC=BC;
         MinPC=PC;
         MinOC=OC;

      cout<<"Total cost: "<<MinTC<<" = "<<MinIC<<"(IC)"<<" + "<<MinBC<<"(BC)"<<
      " + "<<MinOC<<"(OC)"<<" + "<<MinPC<<"(PC)";
  //--- end --- PART 3: Implement (R,S) strategy -----------
    return 0 ;
 }   
     