/*
  Use Intelligent Search to play Battleship Board Game.
  We have two maps, one is Battle Map, another is Secret Map.
  Judge whether we hit the boat(1) or sea(0). And calculate the
  hit rate , hit rate low bound and space that hit rate can improve 

   usage (how to run):
   ./HW41 (then input data_4_6_1.txt and isDebug==1 or 0)  

   input file:
   data_4_6_1.txt 

   output file:
   none  

   compile (how to compile):
    g++ -o HW42 HW42.cpp

   pseudocode:
   First declare variable i need, and input data into dataname, M, N, n_BT, BTL[], BTN[], SMap
   Second dynamic calculate n_targets, set array, set vector, initialize SMap, Target0's status and id.
   Third use if else to ask Debug mode (True or False), then use 1 while and 3 for to let it keep doing 
   IS stragegy (隨機打 打中的話就將上下左右化為3優先打 並將斜上下左右化為1 必不打)
   until n_hits==n_target.And calculate hr,hitrate,hrlb and print.

   coded by Che-Wei Yu, ID: H34064090, email: dessertn626@gmail.com
   date: 2018.5.10
*/
#include<iostream>
#include<fstream>
#include<ctime>
#include<vector>
#include<iomanip>
#include<random>
#include<string>
using namespace std;
struct Zone{ //此為各網格之 struct
int row; //儲存其縱軸座標，即由上而下第 row 列, row=0,…,M-1
int col; //儲存其橫軸座標，即由左而右第 col 行, col=0,…,N-1
int id; //儲存其編號，id=0,…,MN-1
int status; //儲存其狀態，status= -1,0,1,2,3,4 (或可再自行定義)
bool isB;//儲存其答案，若為船身則 isB=1，否則為海水 isB=0
};
int main(){
int M, N, //海域之縱、橫軸總格數
    n_BT, //戰艦類型(Battleship Type)總數，譬如 n_BT=2 代表 2 種船型
    *BTL=new int [10000], //各船型之長度，譬如 BTL[1]=2 代表第 2 類船型之長度為 2
    *BTN=new int [10000], //各船型之船數，譬如 BTN[3]=1 代表第 4 類船共有 1 艘
    **BMap,//記錄作戰圖，BMap[i][j]= -1, 0, 1, 2, 3 or 4
    n_shots=0, //記錄當下已擊發之炮彈總數
    n_hits=0,  //記錄當下已擊中之炮彈總數，當 n_hits==n_targets 則終止測試
    n_targets=0; //全部之船格總數，譬如圖 1(b)之 n_targets=(2+3)*2=10
bool **SMap,   //記錄解答圖，若座標(, ) i j 為船身，則 SMap[i][j]=1，否則為 0
     isDebug;  //詳細模式(isDebug=1,PART1&2 用)，列出每步驟過程
               //扼要模式(isDebug=0,PART3 用)，僅列出最後統計資料
double hr, //命中率 hitrate 百分比=100.*n_hits/n_shots 以%為單位
       hrlb, //命中率百分比下限=100.*n_targets/(M*N) 以%為單位
       hrimprate;//命中改善率=100.*(hr-hrlb)/(100.-hrlb) 以%為單位
                 //代表在可改善的空間(100.-hrlb)中，hr 的改善(hr-hrlb)比率
string dataname; //記錄輸入之測試檔名，譬如 data_4_6_1.txt 
fstream file;
//vector of int
vector <int> Target1 (0);//儲存狀態為1的網格，除debug外作用不大，或可不用到
vector <int> Target3 (0);//儲存狀態為3的網格，通常為射擊網格之相鄰網格
vector <int> Target4 (0);//儲存狀態為4的網格，因不易找到，可能也用不到
srand(time(NULL));
  while(1){
       cout<<"Enter fileName: ";
       cin>>dataname;
       file.open(dataname);
     if(file.is_open()){
       break;
     }
      cout<<"The file does not exist!! "<<endl;
   }
    file>>dataname>>M>>N>>n_BT;
     for(int i=0;i<n_BT;i++){
        file>>BTL[i]>>BTN[i];
     }
     //計算n_targets
     for(int h=0;h<n_BT;h++){
      n_targets+=BTL[h]*BTN[h];
    }
    //配置vector 
    vector <Zone>Target0 (M*N); //儲存狀態為 0 的網格，初始時全部網格皆存於此
     //配置BMap&SMap    
     BMap=new int*[M];
     SMap=new bool*[M];
     for(int i=0;i<M;i++){
        BMap[i]=new int [N];
        SMap[i]=new bool [N]; 
     }
    //input data to SMap
    for(int i=0;i<M;i++){ 
      for(int j=0;j<N;j++){ 
        file>>SMap[i][j];  
      }
    }
    //初始化BMap
    for(int i=0;i<M;i++){
      for(int j=0;j<N;j++){
        BMap[i][j]=0;
      }
    }   
    //初始化target0的status和id以及row col
    for(int i=0;i<(M*N);i++){
      Target0[i].status=0;
      Target0[i].id=i;
      Target0[i].row=i/N;
      Target0[i].col=i%N;
     }
    cout<<"Debug mode(1:yes)?";
    cin>>isDebug;

  if(isDebug==1){
    cout<<"A"<<" "<<M<<"x"<<N<<" map, "<<n_BT<<" types, length={";
    for(int i=0;i<n_BT;i++){
      cout<<BTL[i]<<",";
    }
    cout<<"}, number={";
      for(int i=0;i<n_BT;i++){
      cout<<BTN[i]<<",";
    }
    cout<<"}, n_targets="<<n_targets<<endl;
  }
    int randIndex;
    int TempID;

//直到打完船 
while(n_hits!=n_targets){
     randIndex=rand()%Target0.size();//隨機取一個index
        //如果打中的話
      if(SMap[Target0[randIndex].row][Target0[randIndex].col]==1){ 
        n_shots++;
        Target0[randIndex].status=2;
        n_hits++;
if(isDebug==1){
       cout<<"Trial "<<n_shots<<": zone "<<Target0[randIndex].id
       <<"=("<<Target0[randIndex].row<<","
       <<Target0[randIndex].col<<") hit "<<n_hits
       <<", "<<n_targets-n_hits<<" to go!!";
       if(n_hits==n_targets){
           cout<<"  DONE!!"<<endl;
           break;     
       }
       else{
           cout<<endl;
      }
}
    TempID=Target0[randIndex].id;//隨機取一個id
    Target0.erase(Target0.begin()+randIndex);//刪掉取到的這個id
    //打中船後將上下左右化為3  斜上下左右化為1
    for(int i=0;i<Target0.size();i++){
        //打上 (row+1,col) 並將0變3
        if(Target0[i].row==TempID/N+1 && Target0[i].col==TempID%N && Target0[randIndex].status==0){
           Target3.push_back(Target0[i].id);
           Target0[randIndex].status=3;
           Target0.erase(Target0.begin()+i);  
            i--;
      }
        //打下 (row-1,col) 將0變3 並放入Target3中
        if(Target0[i].row==TempID/N-1 && Target0[i].col==TempID%N && Target0[randIndex].status==0){
            Target3.push_back(Target0[i].id);
            Target0[randIndex].status=3;
            Target0.erase(Target0.begin()+i);           
             i--;
       }
        //打左 (row,col-1) 並將0變3 並放入Target3中
        if(Target0[i].row==TempID/N && Target0[i].col==TempID%N-1 && Target0[randIndex].status==0){
           Target3.push_back(Target0[i].id);
           Target0[randIndex].status=3;
           Target0.erase(Target0.begin()+i);
            i--;
       }
        //打右 (row,col+1) 並將0變3 並放入Target3中
        if(Target0[i].row==TempID/N && Target0[i].col==TempID%N+1 && Target0[randIndex].status==0){
           Target3.push_back(Target0[i].id);
           Target0[randIndex].status=3;
           Target0.erase(Target0.begin()+i);
             i--;
       }
        //左上 (row-1,col-1) 將0變1 並放入Target1中
        if(Target0[i].row==TempID/N-1 && Target0[i].col==TempID%N-1 && Target0[randIndex].status==0){
            Target1.push_back(Target0[i].id);
            Target0[randIndex].status=1;
            Target0.erase(Target0.begin()+i);
             i--;
       }
       //右上 (row-1,col+1) 將0變1 並放入Target1中
       if(Target0[i].row==TempID/N-1 && Target0[i].col==TempID%N+1 && Target0[randIndex].status==0){
            Target1.push_back(Target0[i].id);
            Target0[randIndex].status=1;
            Target0.erase(Target0.begin()+i);
             i--;
       }
        //左下 (row+1,col-1) 將0變1 並放入Target1中
       if(Target0[i].row==TempID/N+1 && Target0[i].col==TempID%N-1 && Target0[randIndex].status==0){
           Target1.push_back(Target0[i].id);
           Target0[randIndex].status=1;
           Target0.erase(Target0.begin()+i);
            i--;
       }
       //右下 (row+1,col-1) 將0變1 並放入Target1中
        if(Target0[i].row==TempID/N+1 && Target0[i].col==TempID%N+1 && Target0[randIndex].status==0){
            Target1.push_back(Target0[i].id);
            Target0[randIndex].status=1;
            Target0.erase(Target0.begin()+i);
             i--;
       }
    }//end of for
//將3全部打完
while(Target3.size()>0 && n_hits!=n_targets){
        randIndex=rand()%Target3.size();
    //如果打中船
    if(SMap[Target3[randIndex]/N][Target3[randIndex]%N]==1){
          n_shots++;
          n_hits++;
          if(isDebug==1){
                 cout<<"Trial "<<n_shots<<": zone "<<Target0[randIndex].id
                 <<"=("<<Target0[randIndex].row<<","
                 <<Target0[randIndex].col<<") hit "<<n_hits
                 <<", "<<n_targets-n_hits<<" to go!!";
                 if(n_hits==n_targets){
                     cout<<"  DONE!!"<<endl;
                     break;     
                 }
                 else{
                     cout<<endl;
                 }
          }
    //優先打3
    TempID=Target3[randIndex];
    Target3.erase(Target3.begin()+randIndex);
    for(int k=0;k<Target3.size();k++){
         //如果要打的格子的左上是3
         if(Target3[k]/N==TempID/N-1 && Target3[k]%N==TempID%N-1 && Target0[randIndex].status==3){
            Target1.push_back(Target3[k]);
            Target3.erase(Target3.begin()+k);       
             k--;
         }
         //如果要打的格子的右上是3
         if(Target3[k]/N==TempID/N-1 && Target3[k]%N==TempID%N+1 && Target0[randIndex].status==3) {
            Target1.push_back(Target3[k]);
            Target3.erase(Target3.begin()+k);
             k--;
         }
         //如果要打的格子的左下是3
         if(Target3[k]/N==TempID/N+1 && Target3[k]%N==TempID%N-1 && Target0[randIndex].status==3){
            Target1.push_back(Target3[k]);
            Target3.erase(Target3.begin()+k);
             k--;
         }
         //如果要打的格子的右下是3
         if(Target3[k]/N==TempID/N+1 && Target3[k]%N==TempID%N+1 && Target0[randIndex].status==3){
              Target1.push_back(Target3[k]);
             Target3.erase(Target3.begin()+k); 
                k--;
         }
    } 
      //3打完了繼續打0
      for(int j=0;j<Target0.size();j++){
         //左上 將0變1並放入Target1中
         if(Target0[j].row==TempID/N-1 && Target0[j].col==TempID%N-1 && Target0[randIndex].status==0){
                 Target1.push_back(Target0[j].id);
                 Target0[randIndex].status=1;
                 Target0.erase(Target0.begin()+j);
                 j--;
        }
         //右上 將0變1並放入Target1中
         if(Target0[j].row==TempID/N-1 && Target0[j].col==TempID%N+1 && Target0[randIndex].status==0){
                 Target1.push_back(Target0[j].id);
                 Target0[randIndex].status=1;
                 Target0.erase(Target0.begin()+j);      
                  j--;
        }
          //左下 將0變1並放入Target1中
          if(Target0[j].row==TempID/N+1 && Target0[j].col==TempID%N-1 && Target0[randIndex].status==0){
                   Target1.push_back(Target0[j].id);
                   Target0[randIndex].status=1;
                   Target0.erase(Target0.begin()+j);
                   j--;
           }
          //右下 將0變1並放入Target1中
          if(Target0[j].row==TempID/N+1 && Target0[j].col==TempID%N+1 && Target0[randIndex].status==0){
                  Target1.push_back(Target0[j].id);
                  Target0[randIndex].status=1;
                  Target0.erase(Target0.begin()+j);     
                   j--;
          }
          //上 將0變3並放入Target3中
          if(Target0[j].row==TempID/N+1 && Target0[j].col==TempID%N && Target0[randIndex].status==0){
                   Target3.push_back(Target0[j].id);
                   Target0[randIndex].status=3;
                   Target0.erase(Target0.begin()+j); 
                   j--;
             }
          //下 將0變3並放入Target3中
          if(Target0[j].row==TempID/N-1 && Target0[j].col==TempID%N && Target0[randIndex].status==0){
                   Target3.push_back(Target0[j].id);
                   Target0[randIndex].status=3;
                   Target0.erase(Target0.begin()+j);
                   j--;
       }
          //左 將0變3並放入Target3中
          if(Target0[j].row==TempID/N && Target0[j].col==TempID%N-1 && Target0[randIndex].status==0){
                   Target3.push_back(Target0[j].id);
                   Target0[randIndex].status=3;
                   Target0.erase(Target0.begin()+j);
                   j--;
          }
          //右 將0變3並放入Target3中
          if(Target0[j].row==TempID/N && Target0[j].col==TempID%N+1 && Target0[randIndex].status==0){
                   Target3.push_back(Target0[j].id);
                   Target0[randIndex].status=3;
                   Target0.erase(Target0.begin()+j);
                   j--;
          }
      }//end of for
    }//end of if
       else{ 
        n_shots++;
            if(isDebug==1){
              cout<<"Trial "<<n_shots<<": zone "<<Target3[randIndex]<<"=("
              <<Target3[randIndex]/N<<","<<Target3[randIndex]%N<<") missed"<<endl;
            }
              //刪掉選過的3
              Target3.erase(Target3.begin()+randIndex);
       }
    }//end of while (將做到將3全部打完)
 }
     //如果沒中
      else{ 
         n_shots++;
         Target0[randIndex].status=-1;
          if(isDebug==1){
              cout<<"Trial "<<n_shots<<": zone "<<Target0[randIndex].id<<"=("
              <<Target0[randIndex].row<<","<<Target0[randIndex].col<<") missed"<<endl;
            }
              Target0.erase(Target0.begin()+randIndex);//把已經選過的刪除
      }
}
       hr=100.*n_hits/n_shots;
       hrlb=100.*n_targets/(M*N);
       hrimprate=100.*(hr-hrlb)/(100.-hrlb);
       if(isDebug==1){
            cout<<"Hitrate  = "<<n_hits<<"/"<<n_shots<<" = "<<setprecision(4)<<hr<<"%"<<endl;
            cout<<"Hitrate  lower  bound = "<<n_targets<<"/"<<M*N<<" = "<<setprecision(4)<<hrlb<<"%"<<endl;
            cout<<"Hitrate  Improvement  Rate  =  100.*("<<hr<<"-"<<hrlb<<")/(100.-"
            <<hrlb<<") = "<<setprecision(4)<<hrimprate<<"%"<<endl;
       }
       else{
            cout<<setprecision(4)<<"H34064090"<<" "<<dataname<<" IS "<<(M*N)
            <<" "<<n_targets<<" "<<n_shots<<" "<<hr<<" "<<hrlb<<" "<<hrimprate<<endl;
       }
  return 0;
}