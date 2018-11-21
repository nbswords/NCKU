/*
  Use Random Search to play Battleship Board Game.
  We have two maps, one is Battle Map, another is Secret Map.
  Judge whether we hit the boat(1) or sea(0). And calculate the
  hit rate , hit rate low bound and space that hit rate can improve 

   usage (how to run):
   ./hw41 (then input data_4_6_1.txt and isDebug==1 or 0)  

   input file:
   data_4_6_1.txt 

   output file:
   none  

   compile (how to compile):
    g++ -o hw41 hw41.cpp

   pseudocode:
   First declare variable i need, and input data into dataname, M, N, n_BT, BTL[], BTN[], SMap
   Second dynamic calculate n_targets, set array, set vector, initialize SMap, Target0's status and id.
   Third use if else to ask Debug mode (True or False), then use while to let it keep doing 
   until n_hits==n_targets and target0 is empty. And in the while use two if. One represent hit boat, another represnt hit sea, and
   calculate n_hits and n_shots simultaneously.
   請注意這與之前上傳的不一樣
   
   coded by Che-Wei Yu, ID: H34064090, email: dessertn626@gmail.com
   date: 2018.4.29
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
    vector<Zone> 
    Target0 (M*N); //儲存狀態為 0 的網格，初始時全部網格皆存於此

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
    //RS
   int randIndex;//隨機取一個index
    while(n_hits!=n_targets){
        randIndex=rand()%Target0.size();

         //如果打中
         if(SMap[Target0[randIndex].row][Target0[randIndex].col]==1  && Target0[randIndex].status==0){
            BMap[Target0[randIndex].row][Target0[randIndex].col]=2; 
            Target0[randIndex].isB=1;
            Target0[randIndex].status=2;
            n_hits++;
            n_shots++;
              if(n_hits==n_targets){//如果把船打完了
                 cout<<"Trial "<<n_shots<<" :"<<"zone"<<Target0[randIndex].id
                 <<"=("<<Target0[randIndex].id/N<<","<<Target0[randIndex].id%N<<")"<<" hit "<<n_hits
                 <<", "<<(n_targets-n_hits)<<" to go!! DONE!!"<<endl; 
                 break;
              }
                 cout<<"Trial "<<n_shots<<" :"<<"zone"<<Target0[randIndex].id
                 <<"=("<<Target0[randIndex].id/N<<","<<Target0[randIndex].id%N<<")"<<" hit "<<n_hits
                 <<", "<<(n_targets-n_hits)<<" to go!!"<<endl;          
            Target0.erase(Target0.begin()+randIndex);
         }
        //如果沒打中
         if(SMap[Target0[randIndex].row][Target0[randIndex].col]==0 && Target0[randIndex].status==0){
           BMap[Target0[randIndex].row][Target0[randIndex].col]=-1;
           Target0[randIndex].isB=0;
           Target0[randIndex].status=-1;
           n_shots++;
           cout<<"Trial "<<n_shots<<" :"<<"zone"<<Target0[randIndex].id<<"=("<<Target0[randIndex].row<<","<<Target0[randIndex].col<<") missed"<<endl;
           Target0.erase(Target0.begin()+randIndex);
         }       
     }
       hr=100.*n_hits/n_shots;
       hrlb=100.*n_targets/(M*N);
       hrimprate=100.*(hr-hrlb)/(100.-hrlb);
       cout<<setprecision(4)<<"Hitrate = "<<n_hits<<"/"<<n_shots<<" = "<<hr<<"%"<<endl;
       cout<<setprecision(4)<<"Hitrate lower bound = "<<n_hits<<"/"<<(M*N)<<" = "<<hrlb<<"%"<<endl;
       cout<<setprecision(4)<<"Hitrate Improvement Rate = "<<"100.*("<<hr
       <<"-"<<hrlb<<")/("<<"100.-"<<hrlb<<")"<<" = "<<hrimprate<<"%";
 }
 else{//isDebug=0
   //RS
   int randIndex;//隨機取一個index
    while(!Target0.empty()){
        randIndex=rand()%Target0.size();
         //如果打中
         if(SMap[Target0[randIndex].row][Target0[randIndex].col]==1 && Target0[randIndex].status==0){
            BMap[Target0[randIndex].row][Target0[randIndex].col]=2; 
            Target0[randIndex].isB=1;
            Target0[randIndex].status=2;
            n_hits++;
            n_shots++;
              if(n_hits==n_targets){//如果把船打完了
                 break;
              }
         }
        //如果沒打中
         if(SMap[Target0[randIndex].row][Target0[randIndex].col]==0 && Target0[randIndex].status==0){
           BMap[Target0[randIndex].row][Target0[randIndex].col]=-1;
           Target0[randIndex].isB=0;
           Target0[randIndex].status=-1;
           n_shots++;
           Target0.erase(Target0.begin()+randIndex);
         }       
     }
       hr=100.*n_hits/n_shots;
       hrlb=100.*n_targets/(M*N);
       hrimprate=100.*(hr-hrlb)/(100.-hrlb);
       cout<<setprecision(4)<<"H34064090"<<" "<<dataname<<" RS "<<(M*N)
       <<" "<<n_targets<<" "<<n_shots<<" "<<hr<<" "<<hrlb<<" "<<hrimprate<<endl;
  }
 return 0;
 }
