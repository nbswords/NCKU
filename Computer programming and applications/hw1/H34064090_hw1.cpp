/*
   This code can be compiled and run ok.

   Given a row of n seats in a theater, you need to calculate and output the 
   seat no., stored in S[i], i=0,...,n-1
   seat preference, stored in P[i], i=0,...,n-1
   
   Then, input the no. of occupied seats, output the seat no. & preference by
   replacing the status of the occupied seats by "X"
   
   Then, input the number of adjacent seats requested, denoted by K, output the
   selected seats of the maximal total preference, or NONE if no such seats.
   
   usage (how to run):
    $ ./hw1 

    and then input  n(Number of seats) 
    and then input o(Occupied no.) 
    and then input K(number of adjacent seats you want)

   input file:
     none

   output file:
     none

   compile (how to compile):
     $ g++ -o hw1 H34064090_hw1.cpp

   pseudocode:
     with a given n, 

     如果n是奇數 且 m為最中間座位的index
      Initialize m to (n/2-1/2)
       
      座位號碼左半段(S[0~m]為左半段)的規律必為 n n-2 n-4 n-6...S[m]
      (規律為右值=左值-2) 

      Initialize variable i=0 
      i from 0 keep +1 to i=m
      Initialize variable j=0 
      j from 0 keep +2 to i=m 
      Store every once n-j value in the S[i]
           

      而右半段(S[m+1]~S[n-1])則是 S[m]+1 S[m]+3 S[m]+5..S[n-1] 
      (規律為右值=左值+2 但由於S[m]已有值且S[m+1]不等於S[m]+1 故先寫死S[m+1]=2)

      Initialize array element S[m+1] to 2
      Initialize variable u to m+2
      u from m+2 keep +1 to u=(n-1)
      Store every once S[u-1]+2 in the S[u]  
      
      喜好度在n是奇數的規則為 1 2 3...P[m]...3 2 1
      
      同樣從m分為左半段和右半段 
      (規律為右值=左值+1 因此需先初始化左半段的第一個喜好度P[0])
      
      左半段(P[0]~P[m-1])
      (規律為右值=左值+1)
      Initialize P[0] to 1
      Initialize variable i = 0
      i from 0 keep +1 to j=m-1 
      Initialize  variable j = 1
      j from 1 keep +1 to j=m-1
      Store every once P[i]+1 in the P[j]

      右半段(P[m]~P[n-1])的第一個喜好度是P[m]所以要寫死
      (規律為右值=左值-1)
      Initialize  P[m] to m+1
      Initialize  variable u to u=m+1
      u from m+1 keep +1 to n-1
      Initialize  variable r to r=m
      r from m keep +1 to u=n-1       
      Store every once P[r]-1 in the P[u]


    如果n是偶數 且 L為最中左數 R為最中右數
    Initialize L to (n/2)-1
    Initialize R to (n/2)  

      座位號碼左半段(S[0~L])的規律為n-1 n-3 n-5 ...S[L]
      (規律為右值=左值-2)
      Initialize variable i to 0
      i from 0 keep +1 to i=L
      Initialize variable j to 0     
      j from 0 keep +2 to i=L
      Store every once n-i-j in the S[i]

      座位號碼右半段(S[R~n-1])的規律為S[R] S[R]+2 S[R]+4...S[n-1]
      (規律為右值=左值+2)
      Initialize variable u to R+1
      u from R+1 keep +1 to u=n-1
      Store every once S[u-1]+2 in the S[u]

      喜好度在n是偶數的規律為 1 2 3 ...P[L] P[R]...3 2 1
      同樣分為左半段(P[0~L])和右半段(P[R~n-L])
      (規律為右值=左值+1)

      左半段的喜好度:
       Initialize array element P[0] to 1
       Initialize array element P[L] to n/2
       Initialize variable i to 0
       i from 0 keep +1 to i=L-1
       Initialize variable j to 1
       j from 1 keep +1 to i=L-1
       Store every once P[i]+1 in the P[j]
      右半段的喜好度:
       Initialize array P[r] to n/2
       Initialize variable u to r+1
       u from r+1 keep +1 to u=n-1
       Initialize variable q to r
       q from r keep +1 to u=n-1
       Store every once P[q]-1 in the P[u]

       print Seat Number : 
       Initialize variable i to 0
       i from 0 keep +1 to i=n-1
       print Seats

       print Seat Preference : 
       Initialize variable i to 0
       i from 0 keep +1 to i=n-1
       print Preference
     

     for a known occupied seat o, !! describe how you reset its no. and preference !! 
      
      首先 利用for搜尋S[]並將座位號碼=o的座位和喜好度先令為0  (o=被佔據的座位號碼)
      Initialize variable i to 0
      i from 0 keep +1 to i=n-1
      if S[i]==o
      assign 0 to S[i] and P[i]
   
     再來 利用for搜尋S[]
     如果 座位號碼=0 則輸出X
     否則 輸出座位S[]
     Initialize variable i to 0
     i from 0 keep +1 to i=n-1
     if S[i]==0
     print X
     else
     print Seat

     如果 喜好度=0 則輸出X
     否則 輸出喜好度P[]
     Initialize variable i to 0
     i from 0 jkeep +1 to i=n-1
     if P[i]==0
     print X
     else
     print Preference

	 for a given K, !! describe how you search for the best K adjacent seats !!

     首先  依題目需求  一定需要一個大小為K的陣列
     因此設a[k]拿來存取相鄰座位 
     接下來 由於為了尋找擁有最大喜好度總合的相鄰座位  
     需要在加總喜好度(內層的for loop)後 再搜尋其他的座位(外層的for loop)
     Initialize array a size to number of adjacentSeats user want
     Initialize variable i to K 
     i from K keep +1 to i=n
     Every once loop reset the TotalPreference and plus 1 to number of adjacent seats

     Initialize variable j to number of adjacent seats and keep +1 until j<K
        if Preference not equal to 0  then plus its preference to Total Preference
        if TotalPreference > Max preference
         then make TotalPreference become Max preference
     Initialize variable j(keep loop in the above loop) = count and variable x = 0
         j from count and x from 0 keep +1 to j<i
         Store every once S[j] in the a[x]


    then print adjacent seats K and Best adjacent seats no. and their total preference



   coded by Che-Wei YU(游哲瑋), ID: H34064090, email: dessertn626@gmail.com
   date: 2018.03.03
*/
#include <iostream>
#include <iomanip>
using namespace std ;

int main() {
	//---begin--- PART 1: Declaration -----------
    int n=0, K=0;  //n = Number of Seats and K = Number of seats adjacent
    int S[20]={0}; //Seat
    int P[20]={0}; //Preference
	  int m=0;//最中間的座位(當n=奇數)
    int L,R=0;//最中間的左數為L  最中間的右數為R(當n=偶數)

  //-----------以下變數Part3才會用到--------------------------  

    int o=-1;//被佔據的座位號碼 先初始化為非0的數 

  //-----------以下變數Part4才會用到---------------------------

    int a[K];  //a[]用來存相鄰的座位 a=adjacent
    int count=0;//算有幾個相鄰座位
    int TP=0;  //TP=TotalPreference 總喜好度
    int Max=0;//加起來喜好度最大的相鄰座位的總合

	//--- end --- PART 1: Declaration -----------
	//---begin--- PART 2: Calculate S[] & P[] -----------
      cout << "Number of seats n in [5,20] = ? ";  //input total seat 
      cin >> n ;
	   while(n<5||n>20){       //check if the input is right in the range[5,20].
	      	cout << "!!WRONG!! Please input an integer in [5,20] = ?";  //if not,input again.
          cin >> n;
     }
       
  //如果n是奇數
  if(n%2!=0){
     
     //排座位------------------------
     
     //從S[0]向右-2到S[m]
      m=(n/2-1/2);//令m為最中間的數

     //從S[0]開始向右-2到S[m]
    for(int i=0,j=0;i<=m;i++,j+=2){
            S[i]=n-j;
    }
     //從S[m+1]開始向右+2到S[n-1]  
      S[m+1]=2; 
    
    for(int u=(m+2);u<n;u++){
            S[u]=S[u-1]+2;
    }

    //排喜好度------------------------
      P[0]=1;   
    //從P[1]開始排到P[m-1]
    for(int i=0,j=1;j<m;i++,j++){
            P[j]=P[i]+1;
    }

      P[m]=(m+1); 
    //從P[m+1]開始排到P[n-1]
    for(int u=(m+1),r=m;u<n;u++,r++){
            P[u]=P[r]-1;
    }
  }



 //如果n是偶數
  if(n%2==0){

       L=(n/2)-1;
       R=n/2;//令R為最中的右數
  //排座位----------------------------
     
     //從S[0]開始向右-2到S[L]
        for(int i=0,j=0;i<=L;i++,j+=2){
              S[i]=n-1-j;
        }
      
      S[R]=2;//最中的右數的座位必=2
    
     //從S[R+1]開始向右+2   
       for(int u=(R+1);u<n;u++){
              S[u]=S[u-1]+2;
       }
   //排喜好度---------------------------
      P[0]=1;
      P[L]=n/2;
    
    //從P[1]開始排到P[L-1]
       for(int i=0,j=1;j<L;i++,j++){
              P[j]=P[i]+1;
       }

      P[R]=n/2;
    //從P[R+1]開始排到P[n-1]
    
       for(int u=(R+1),q=R;u<n;u++,q++){
              P[u]=P[q]-1;
       }
   }

  //印出seat number和seat preference
  cout<<"Seat Number : ";
     for(int i=0;i<n;i++){
        cout<<" "<<S[i];
     }   
  cout<<endl;
  cout<<"Seat preference : ";
  
     for(int i=0;i<n;i++){
         cout<<" "<<P[i];
     }
	//--- end --- PART 2: Calculate S[] & P[] -----------
	//---begin--- PART 3: Output Updated Seat Status with known Occupied seats -----------
  cout<<endl<<"Occupied seat no.: ? "; 

  while(o!=0){
     cin>>o;
       for(int i=0;i<n;i++){
             if(S[i]==o){
                S[i]=0; //令被佔據的座位為0
                P[i]=0;        
             }       
       }
  }

  cout<<endl<<"Seat Number : ";

  for(int i=0;i<n;i++){
      if(S[i]==0){     //若是0則印X
        cout<<" "<<'X';
      }
      else
        cout<<" "<<S[i]; //若不是則印出座位
  }

  cout<<endl<<"Seat preference : ";
  
  for(int i=0;i<n;i++){
       if(P[i]==0){
          cout<<" "<<'X'; 
       }
      else
          cout<<" "<<P[i];
  }

	
	//--- end --- PART 3: Output Updated Seat Status with known Occupied seats -----------
	//---begin--- PART 4: Seek optimal K adjacent seats -----------

	  cout << endl <<"How many adjacent seats to seek? ";   //input how many adjacent seat(K) to calculate max seat preference
    cin >> K;  
    
 
    for(int i=K;i<=n;i++){ //從K跑到n
       TP=0;
         for(int j=count;j<i;j++){  //從0跑到K
           //如果Preference=0就跳出
           if(P[j]==0){
             TP=0;
             break;
           }
           //如果Preference不等於0就加進Total Preference中
           if(P[j]!=0){
             TP=TP+P[j];
           }
           //如果Total Preference比Max大就變成Max 
           if(TP>Max){
             Max=TP;
               for(int j=count,x=0;j<i;j++,x++){
                  a[x]=S[j];
               }
           }
         }
           count++;
    }
   
    if(TP==0){

      cout<<"Sorry! No "<<K<<" adjacent seats!";
    }
    else{

      cout<<"Best "<<K<<" adjacent seats: no. ";
          for(int i=0;i<K;i++){
             cout<<a[i]<<" ";
          }
      cout<<", with total preference "<<Max;
    }
	//--- end --- PART 4: Seek optimal K adjacent seats -----------
    return 0 ;	
}
