/*
   This code can be compiled and run ok.
   
   Input two file which called data1.txt and data2.txt
   and then use the data in the file to calculate Total Demand 
   ,Average Demand and standard deviation of Demand
   print the result and the data
   
   Then implement two ordering strategy
   first is (St,Qt) strategy:
   when inventory <= St , trigger a order,whice order quantity equal to Qt
   and then calculate the total cost ,backlog cost,ordering cost
   and purchasing cost,finally print all process and cost

   second is (R,S) strategy:
   Trigger first order at V day and at every V+nR day 
   trigger once order.After every once order make 
   inventory equal to S.
   and then calculate the total cost ,backlog cost,ordering cost
   and purchasing cost ,finally print all process and cost


   usage (how to run):
     input ./hw2_123
     and then input data1.txt or data2.txt

   input file:
     data1.txt
     data2.txt

   output file:
     none

   compile (how to compile):
     g++ -o hw2_123 hw2_123.cpp

   pseudocode:
     
   initialize and declare most of variable and array will need.
   print enter filename
   store filename and open the file
   then store the data in the file in T,D(a series of number)
   ,H,B,A,C,V,R,I[0],L[0]
   
   then close the file

   calculate total D and store in Dtotal 
   calculate average of D and store in D_bar
   D_bar:plus all D and then divided by T
   calculate standard deviation of D:
   first calculate Sigma ((D-D_bar)^2)/(T-1)
   and add square root on it

   print all data and total D ,average of D and standard deviation of D
   typesetting according to request

   實作(St,QT)-----------------------------------------
   
   print St,Qt as type setting

   initialize i to 1 which avoid to make index of array <0
   initialize j to 0 which represent t
   keep i+1 and j+1 to j=T
   
     if lack of inventory 
        let Y[i]=1  決定訂貨
        let L[i]=D[i]-I[i-1]-O[i] 計算今天缺貨量
        let I[i]=0; 缺貨表示已無存貨(I)

       if I[j]<=s (實做St,Qt策略)
          let O[j+1]=Q  令明天的訂貨量=Q
          let I[i]=O[j+1]+I[i-1]-D[i]
          let L[i]=0  
     else (表示不缺貨)
          let Y[i]=0  不需訂貨
          let I[i]=I[i-1]+O[i]-D[i] 今天的存貨=昨天的存貨+今天的訂貨-今天的需求量
          let L[i]=0 不缺貨則表示L=0

     if demurrage (如果滯貨的話)
          declare DD to 0  表滯貨天數 demurrage days
          DD+1 滯貨了一天就+1
          H*=DD  滯貨幾天乘以持有成本             
            
    let TI equal to TI+I[j]  計算總存貨量(TI)
    let TL equal to TL+L[j]  計算總缺貨量(TL)
    let TO equal to TO+O[j]  計算總訂貨量(TO)
        
   print t,D,O,I,L and of cource
   typesetting according to request
   
	
   let TI minus 45 (45 表示第0天的存貨 因為木已成舟所以不算成本)
   (L[0]本來就都是0所以不用減)
   
   initialize j to 0 
   j keep +1 to j=T-1 
   Store all TY+Y[j] to TY

   let IC=H*TI  算出IC
   let BC=B*TL  算出BC
   let OC=A*TY  算出OC
   let PC=C*TO  算出PC  

   let TC=IC+BC+PC+OC 算出TC

   print IC BC OC PC individually and TC
   
   實作(R,S)-----------------------------------------
   
   重新初始化TC,BC,OC,PC,TI,TL,TY,TO 以及 Y[i]和O[i]
   
   print R,S as type setting

   initialize i to 1 which avoid to make index of array <0
   initialize j to 0 which represent t
   keep i+1 and j+1 to j=T

   if j=V,V+nR (n=1,2,3,4) 如果是進貨日
       let Y[j]=1 表決定進貨
       let O[i-1]=S-I[i-2] 今天的進貨量=S-昨天的存貨量
       let I[i-1]=O[i-1]+I[i-2]-D[i-1] 今天的存貨量=今天的進貨量+昨天的存貨量-今天的需求量 
       let L[j]=0 今天不會缺貨                        
       declare OD integer equal to 0 計算進貨天數
       let OD+1 
       將A*OD存入A (計算進貨成本)
   如果不是訂購日
       let Y[i]=0 表決定不進貨
       let I[i]=I[i-1]+O[i]-D[i] 今天的存貨量=昨天的存貨量+今天的進貨量-今天的需求量
   如果缺貨 if(I[i-1]+O[i])<D[i]
       let L[j+1]=D[j+1]-I[j]-O[j+1]//缺貨量
       let I[i]=0 缺貨的話表示存貨是0
       declare LD integer equal to 0 計算缺貨天數
       let LD+1
       將B*LD存入B(計算缺貨成本)  
    如果滯貨 if(I[i-1]+O[i])>D[i] 
      declare DD integer equal to 0 計算滯貨天數
      DD+1
      將H*DD存入H(計算持有成本)  

    let TI equal to TI+I[j]  計算總存貨量(TI)
    let TL equal to TL+L[j]  計算總缺貨量(TL)
    let TO equal to TO+O[j]  計算總訂貨量(TO)
        
   print t,D,O,I,L and of cource
   typesetting according to request
   
   let TI minus 45 (45 表示第0天的存貨 因為木已成舟所以不算成本)
   (L[0]本來就都是0所以不用減)
   
   initialize j to 0 
   j keep +1 to j=T-1 
   Store all TY+Y[j] to TY

   let IC=H*TI  算出IC
   let BC=B*TL  算出BC
   let OC=A*TY  算出OC
   let PC=C*TO  算出PC  

   let TC=IC+BC+PC+OC 算出TC

   print IC BC OC PC individually and TC
  

   coded by Che-Wei YU, ID: H34064090, email: dessertn626@gmail.com
   date: 2018.03.18
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
       s, //再訂購點
       Q, //經濟訂購批量
       V, //初始進貨日
       R, //訂購週期
       S; //預期期初庫存量
   int D[32]={0}, //儲存第 i(=1,…,31)天的預測需求量，先初始化為 0
       I[32]={0}, //儲存第 i(=0,…,31)天的期末存貨量，先初始化為 0
       L[32]={0}, //儲存第 i(=0,…,31)天的期末缺貨量，先初始化為 0
       Y[32]={0}, //儲存第 i(=0,…,30)天的訂購決定，先初始化為 0
       O[32]={0}; //儲存第 i(=1,…,31)天的期初進貨量，先初始化為 0
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
  
if(file>>T){    
   cout<<"T= "<<T<<", ";
}
for(int i=1;i<=T;i++){
    if(file>>D[i]);   
}  
  cout<<"D= ";
for(int j=1;j<=T;j++){
   cout<<D[j]<<" ";
}

  cout<<endl;

if(file>>H){    
     cout<<"H="<<H<<", ";
    }
if(file>>B){    
     cout<<"B="<<B<<", ";
    }
if(file>>A){    
     cout<<"A="<<A<<", ";
    }
if(file>>C){    
     cout<<"C="<<C<<", ";
    }
if(file>>V){    
     cout<<"V="<<V<<", ";
    }
if(file>>R){    
     cout<<"R="<<R<<", ";
    }
if(file>>I[0]){    
     cout<<"I[0]="<<I[0]<<", ";
    }
if(file>>L[0]){    
     cout<<"L[0]="<<L[0]<<", ";
    }

file.close();
cout<<endl;

int Dtotal=0;//儲存D的總量   

 for(int i=1;i<=T;i++){
    Dtotal+=D[i];
 }
    cout<<"D="<<Dtotal<<" ";

int D_bar=0;//D的平均
 
  D_bar=Dtotal/T;
  cout<<"D_bar="<<D_bar<<" ";

int D_sig=0;//D的標準差sigma
int sspow=0;//sum of squares(ss)離均差的平方 
            //離均差:每筆資料和平均數的差
 
  for(int i=1;i<=T;i++){ //先算出離均差平方的總合
       sspow+=(pow((D[i]-D_bar),2));
  }
  
  D_sig=sqrt(sspow/(T-1));//再把離均差平方的總合除以T-1再開根號
 
  cout<<"stdev="<<D_sig<<"; ";

  s=D_bar+D_sig;//計算再訂購點
  Q=sqrt(2*A*Dtotal/H);//計算經濟訂購批量  
  cout<<"(s,Q)=("<<s<<","<<Q<<"); ";
  
  S=7*D_bar;//計算預期期初存貨量
  
  cout<<"(R,S)=("<<R<<","<<S<<"); "<<endl<<endl;
  //--- end --- PART 1: Declaration and read file -----------
  //---begin--- PART 2: Implement (St,Qt) strategy -----------
  cout<<"(s="<<s<<",Q="<<Q<<") model:"<<endl;
  cout<<setw(4)<<"t"<<setw(4)<<"D"<<setw(4)<<"O"<<setw(4)<<"I"<<setw(4)<<"L"<<endl;
  cout<<"-----------------------------"<<endl;
for(int i=1,j=0;j<=T;i++,j++){
        if((I[i-1]+O[i])<D[i]){//如果缺貨
           Y[i]=1;
           L[i]=D[i]-I[i-1]-O[i];//缺貨量
           I[i]=0;
          if(I[j]<=s){//(ST,QT)訂購策略
            O[j+1]=Q;
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
       
        cout<<setw(4)<<j<<setw(4)<<D[j]<<setw(4)
        <<O[j]<<setw(4)<<I[j]<<setw(4)<<L[j]<<endl;
  }

       TI-=45; //減掉第0天的I
       for(int j=0;j<T;j++)
       TY+=Y[j];//總Y
         
       IC=H*TI;//算出IC
       BC=B*TL;//算出BC
       OC=A*TY;//算出OC
       PC=C*TO;//算出PC     

      cout<<endl;
      cout<<"=============================="<<endl;
 
      TC=IC+BC+PC+OC;//算出TC
      
      cout<<"Total cost: "<<TC<<" = "<<IC<<"(IC)"<<" + "<<BC<<"(BC)"<<
      " + "<<OC<<"(OC)"<<" + "<<PC<<"(PC)"<<endl;
  //--- end --- PART 2: Implement (St,Qt) strategy -----------
  //---begin--- PART 3: Implement (R,S) strategy -------------
      
       //重新初始化
       Y[32]={0}, 
       O[32]={0}; 
       TI=0,//總I[i] i從1到T
       TL=0,//總L[i] i從1到T
       TY=0,//總Y[i] i從0到T-1
       TO=0,//總O[i] i從1到T
       IC=0,//存貨成本
       BC=0,//缺貨成本
       OC=0,//訂購成本
       PC=0,//進貨成本
       TC=0;//總成本
  cout<<endl;
  cout<<"(R="<<R<<",S="<<S<<") model:"<<endl;
  cout<<setw(4)<<"t"<<setw(4)<<"D"<<setw(4)<<"O"<<setw(4)<<"I"<<setw(4)<<"L"<<endl;
  cout<<"-----------------------------"<<endl;

  D[0]=0;
  I[0]=45;

for(int i=1,j=0;j<=T;i++,j++){                   
          if((j==V)||(j==V+R)||(j==V+2*R)||(j==V+3*R)||(j==V+4*R)){//第V日、V+nR日進貨(n=1~4)              
                Y[j]=1;
                O[i-1]=S-I[i-2];
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
       
        cout<<setw(4)<<j<<setw(4)<<D[j]<<setw(4)
        <<O[j]<<setw(4)<<I[j]<<setw(4)<<L[j]<<endl;
  }
  
       TI-=45; //減掉第0天的I
      
       for(int j=0;j<T;j++)
       TY+=Y[j];//總Y
 
       IC=H*TI;//算出IC
       BC=B*TL;//算出BC
       OC=A*TY;//算出OC
       PC=C*TO;//算出PC    
      cout<<endl;
      cout<<"=============================="<<endl;
 
      TC=IC+BC+PC+OC;//算出TC
      
      cout<<"Total cost: "<<TC<<" = "<<IC<<"(IC)"<<" + "<<BC<<"(BC)"<<
      " + "<<OC<<"(OC)"<<" + "<<PC<<"(PC)";

 
  //--- end --- PART 3: Implement (R,S) strategy -----------
    return 0 ;
 
}