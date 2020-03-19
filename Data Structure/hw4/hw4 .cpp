/*
   This program can be compile and run ok
   P.S:
   因為我覺得A要先把資料duplicate給B後再sort A
   之後還要把B assign回去給A不太直覺(A一開始就是儲存原始資料的陣列所以...)
   因此我使用的方法是
   >印出A 
   >A先duplicate給B
   >然後sort B  
   >印出B
   如此一來  
   A永遠都只存原始資料
   而B一直都是被sort的陣列

   Use:
   Ask user input a natural number n,
   an integral lower bound a_min, 
   an integral upper bound a_max, 
   and a random seed rseed.
   
   Produce a sequence of n random integer numbers with random seed rseed (i.e. A[0], A[1],..., A[n-1]) 
   uniformly distributed in the range [a_min, a_max]

   input file:
	 n,a_min,a_max,rseed
   output file:
	 none
   compile (how to compile):
	 g++ -o h34064090_hw4 h34064090_hw4.cpp

   pseudocode: 

   coded by Che-Wei YU, ID: H34064090, email: dessertn626@gmail.com
   date: 2018.12.25
*/
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
using namespace std;
 
void Duplicate_array(int *A, int *B, int n){

		for(int j=1 ; j<=n; j++)
		      B[j]=A[j];
}


void Printout_arrary_Zero(int *A, int n){

		for(int i=0; i<n; i++)
			cout<<A[i]<<" ";
} 
void Printout_arrary_One(int *A, int n){

        for(int i=1; i<=n; i++)
            cout<<A[i]<<" ";
} 

void One_To_Zero (int *B, int n){
    //儲存在B[1]~B[n]的資料 換成儲存在B[0]~B[n]
    for (int i=0; i<n; i++)
        B[i] = B[i+1];
}

void Zero_To_One (int *B, int n){
    for (int i=n-1; i>=0; i--)
        B[i] = B[i-1];
}

//注意是確認從1~n的資料 所以注意得是這樣的資料儲存位置才能work
bool is_B_desc(int *B, int n){

    for ( int i=1; i < n; i++ ){
         if ( B[i-1] > B[i] )
            return false;
       }
            return true;
}
//-------------Insertion Sort-----------------------//
//InsertSort function:
void ins_sort(int *B, int n){
  //A是要排序的陣列，n是陣列大小
  for(int next=1; next<=n; next++){//next要移動的數的位置
	int insert = B[next];//insert = 暫存要移動的數
	int move = next;//move = 最大移動次數
	
	while( (move>0) && (B[move-1]<insert) ){
	  //條件一:(move>0)避免移動到arr[-1]
	  //條件二:左數小於右數的時候，將右數覆寫成左數
	  B[move] = B[move-1];
	  move--;
	}
	B[move] = insert;
	//把要移動的數插入到正確的位置上
  }
}
//------------end of Insertion Sort-------------------------//
//-------------Merge Sort--------------------------//
/* Merge function:
    Let L[0...n1] and R[0...n2] be new arrays
    and copy B[start...end] to L[0...n1] 
    and copy B[start+1...end] to R[0...n2] */
void Merge(int *B, int start, int mid, int end) {

    int n1 = mid - start + 1; //子陣列的最後一個元素
    int n2 = end - mid;  //另一個子陣列的最後一個元素 

    int* L = new int [n1]; //Left subarray
    int* R = new int [n2]; //Right subarray


        for(int i=0; i<n1; i++){
            L[i] = B[start+i]; 
        }
    
        for(int j=0; j<n2; j++){
            R[j] = B[mid+j+1]; 
        }
    
    L[n1] = -2147483648; //將兩子陣列的最後一個元素設很小 
    R[n2] = -2147483648;
    
    int i = 0, j = 0; //i is Index of Left subarray
                      //j is Index of Right subarray
    //左右子陣列相比  比較大的就丟入B array中
    for (int k = start; k <= end; k++) {
        if (L[i] >= R[j]){ 
            B[k] = L[i];
            i++;
        }
        else {
            B[k] = R[j];
            j++;
        }
    }
}

/*Merge_Sort function:
   use recursive call function to sort two subarray
   and merge them */
void mg_sort(int *B, int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2; //count the middle number m
        mg_sort(B, start, mid);  //Sort subarray B[start...m]
        mg_sort(B, mid+1, end); //Sort subarray B[m+1...end]
        Merge(B, start, mid, end); //Merge L and R
    }
}
//-------------end of Merge Sort---------------------//
//--------------Heap Sort-------------------------//

//交換陣列元素 (用在Heap Sort 和 Quick Sort)
void Swap(int &a,int &b) {
    int temp;
    temp = a;
    a = b;
    b = temp;
}
//將tree變成Min heap
void Min_heapify (int *B,int Parent,int n){

    int L = 2*Parent;       // Left child 左子代的index
    int R = 2*Parent + 1;   // Right child 右子代的index
    int Smallest;          // parent和兩個child間值最小node的index

    // 左子代和父母比較  比較小的則標為最小
    if (L <= n && B[L] < B[Parent])
        Smallest = L;
    else
        Smallest = Parent;

    //右子代和最小的比較  比較小的標為最小
    if (R <= n && B[R] < B[Smallest])
        Smallest = R;

    if (Smallest != Parent) {                //如果目前Parent不是三者中的最小數
        Swap(B[Smallest], B[Parent]);  // 就交換Parent和最小的node的位置
        Min_heapify(B, Smallest, n);         // 重新將新的subtree變成Minheap tree
    }
}


void Build_Min_heap (int *B,int n){
    for (int i=n/2 ; i >= 1 ; i--){ //從n/2 -> 1 (檢查每個有child的node)
        Min_heapify(B,i,n);
    }
}

void Heap_Sort(int *B,int n){
    Build_Min_heap(B,n);                // 將A初始化成Min Heap
    int size = n;
    for (int i = n ; i >= 2 ; i--) {
        Swap(B[1], B[i]);        // 將最小的node和最後一個node位置交換
        size--;                        //抽掉最後一個node
        Min_heapify(B,1,size);         // 重新把新的subtree變成Maxheap tree
    }
}
//------------end of Heap Sort------------------------//

//------------Quick Sort------------------------//
//找出Pivot並且Divide
int Partition(int *B,int p,int r){
	int Pivot = B[r];
    int i = p-1;
    for (int j=p ; j<r ; j++){
    	if (B[j] >= Pivot){
    		i++;
    		Swap(B[i],B[j]);
    	}
    }
    i++;
    Swap(B[i],B[r]);
    return i;
}

void Quick_Sort(int B[],int p,int r){
	if (p < r){
		int q=Partition(B,p,r);
		Quick_Sort(B,p,q-1);
		Quick_Sort(B,q+1,r);
	}
}
//----------End of Quick Sort------------------------//
//----------Counting Sort---------------------------//
void Counting_Sort(int A[],int B[],int n){
    int k = 0; //每種數字累積的次數
    //算出k應該是多少(有幾種數字)
    for(int i=1; i<=n; i++){
        if(A[i]>k)
           k=A[i];              
    }
    //C是儲存累積數字出現次數的動態陣列
    int *C = new int [k];
    
    //先將C的值都先初始化為0
    for(int i=0; i<=k; i++)
        C[i]=0;
    //每遇到一個A[j]都增加其數量 
    //(A中有幾個2存在C[2] 有幾個3存在C[3]....) 
    for(int j=1; j<=n; j++)
        C[A[j]]++;              
    //計算出每一個數累積的數量並覆蓋掉C的值
    //EX: 原本的C=[1,2,3] -> C[1,3,6]
    for(int i=1; i<=k; i++)
        C[i]+=C[i-1];        
    //根據C來排序
    //每用過一個數字就-1
    for(int j=n; j>=1; j--){
        B[C[A[j]]]=A[j];          
        C[A[j]]--;        
    }
    delete[]C; 
}
//-----------end of Counting Sort----------
//-----------Radix Sort--------------------
//找出最大值
int GetMax(int *A, int n){
    //先假設第一個值是最大的
    int Max = A[0];

    //若後面有比較大的值就換人當最大值
    for (int i = 1; i < n;  i++){
        if (A[i] > Max){
            Max = A[i];
        }
    }
    return Max;
}


void Radix_Sort(int *B, int n) 
{ 
    int output[n]; // output array 
    int Count[10] = {0};
    int Max = GetMax(B, n); 
    for (int exp = 1; Max/exp > 0; exp *= 10) {
    
    for (int j=0; j<10;j++)
        Count[j] = 0;
  
    // Store count of occurrences in count[] 
    for (int i=0; i < n; i++) 
        Count[ (B[i]/exp)%10 ]++; 
  
    // Change count[i] so that count[i] now contains actual 
    //  position of this digit in output[] 
    for (int j=1; j < 10; j++) 
        Count[j] += Count[j - 1]; 
  
    // Build the output array 
    for (int i=n-1; i >= 0; i--) 
    { 
        output[Count[ (B[i]/exp)%10 ] - 1] = B[i]; 
        Count[ (B[i]/exp)%10 ]--; 
    } 
 
    for (int j = 0; j < n; j++){
          B[j] = output[j];  
    }
  }
} 
//-----------end of Radix Sort-------------
//-----------Bucket Sort-------------------
void Bucket_Sort(int* B, int n) {
    //先設第一個數字為最大、最小值
    int MinValue = B[0];
    int MaxValue = B[0];

    //找出真正的最大最小
    //不使用CountingSort的GetMax func因為感覺會讓code變很亂
    for (int i=1; i < n; i++){
        if (B[i] > MaxValue)
            MaxValue = B[i];
        if (B[i] < MinValue)
            MinValue = B[i];
    }

    //桶子長度 (第一維大小,也就是有幾個桶子)
    int BucketLength = MaxValue - MinValue + 1;
    //用二維的vector來實作桶子
    vector<int>* Bucket = new vector<int>[BucketLength];

    for (int i = 0; i < BucketLength; i++)
        Bucket[i] = vector<int>();

    for (int i = 0; i < n; i++)
        Bucket[ (B[i]-MinValue) ].push_back(B[i]);
    
    int k = 0;
    //把數字從桶子放回B中
    //根據每個桶子的大小來看要拿多少個數字
    for (int i = 0; i < BucketLength; i++){
    	//有幾個數字桶子大小就多大
        int BucketSize = Bucket[i].size();
        if (BucketSize > 0){
            for (int j = 0; j < BucketSize; j++){
                B[k] = Bucket[i][j];
                k++;
            }
        }
    }
}

void IS(int *A , int *B , int n){
    //------印出A(Sort前)
    cout<<"A= ";
    //印出1~n
    Printout_arrary_One(A,n);
    cout<<endl;
    //------印出B(Sort後)
    Duplicate_array(A,B,n);
    ins_sort(B,n);
    cout<<"By Insertion sort: "<<endl;
    cout<<"B= ";
    Printout_arrary_One(B,n);
    cout<<endl;
}

void MS (int *A, int *B, int n){
    //印出A(Sort前)
    cout<<endl;
    cout<<"A= ";
    Printout_arrary_One(A,n);
    cout<<endl;
    //------印出B(Sort後)
    Duplicate_array(A,B,n);
    //**把資料換到0~n-1
    One_To_Zero(B,n);
    mg_sort(B,0,n-1);
    cout<<"By Merge sort: "<<endl;
    cout<<"B= ";
    //------印出0~n-1
    Printout_arrary_Zero(B,n);
    cout<<endl;
    //**把資料換到1~n
    Zero_To_One(B,n);
}
void HS (int *A, int *B, int n){
    //------印出A(Sort前)
    cout<<endl;
    cout<<"A= ";
    Printout_arrary_One(A,n);
    cout<<endl;
    //------印出B(Sort後)
    Duplicate_array(A,B,n);
    Heap_Sort(B,n);
    cout<<"By Heap sort: "<<endl;
    cout<<"B= ";
    Printout_arrary_One(B,n);
    cout<<endl;
}
void QS (int *A, int *B, int n){
    //------印出A(Sort前)
    cout<<endl;
    cout<<"A= ";
    Printout_arrary_One(A,n);
    cout<<endl;
    //------印出B(Sort後)
    Duplicate_array(A,B,n);
    Quick_Sort(B,1,n);   //要排的數字是從B[0][1]到B[0][n]
    cout<<"By Quick sort: "<<endl;
    cout<<"B= ";
    Printout_arrary_One(B,n);   
    cout<<endl;
}
void CS (int *A, int *B, int n){
    //------印出A(Sort前)
    cout<<endl;
    cout<<"A= ";
    Printout_arrary_One(A,n);
    cout<<endl;
    //------印出B(Sort後)
    Duplicate_array(A,B,n);
    Counting_Sort(A,B,n);
    reverse(B+1,B+n+1);
    cout<<"By Counting sort: "<<endl;
    cout<<"B= ";
    Printout_arrary_One(B,n);
    cout<<endl;
}
void RS(int *A, int *B, int n){
    //------印出A(Sort前)
    cout<<endl;
    cout<<"A= ";
    Printout_arrary_One(A,n);
    cout<<endl;
    //------印出B(Sort後)
    Duplicate_array(A,B,n);
    //把資料換到0~n-1
    One_To_Zero(B,n);
    Radix_Sort(B,n); 
    reverse(B,B+n);
    cout<<"By Radix sort: "<<endl;
    cout<<"B= ";
    Printout_arrary_Zero(B,n); 
    cout<<endl;
    //把資料換到1~n
    Zero_To_One(B,n);
}
void BS(int *A, int *B, int n){
    //------印出A(Sort前)
    cout<<endl;
    cout<<"A= ";
    Printout_arrary_One(A,n);
    cout<<endl;
    //------印出B(Sort後)
    Duplicate_array(A,B,n);
    //把資料換到0~n-1
    One_To_Zero(B,n);
    Bucket_Sort(B,n); 
    reverse(B,B+n);
    cout<<"By Bucket sort: "<<endl;
    cout<<"B= ";
    Printout_arrary_Zero(B,n); 
    cout<<endl;
    //把資料換到1~n
    Zero_To_One(B,n);
}
//-----------end of Bucket Sort------------
int main() {
	int n,a_min,a_max;
	unsigned int rs; //random seed
    float t1,t2;
    bool is_p;
	cout<<"Please input [is_p, n, a_min, a_max, rs]: "<<endl;
	cin>>is_p>>n>>a_min>>a_max>>rs;
    cout<<endl;

	srand(rs);
	//construct A[]  為了Quicksort設(n+1)大小
	int *A= new int [n+1];
    //construct B[]
    int *B= new int [n+1];

	//將亂數產生的n個數傳入A[1~n]
	for (int i=1 ; i<=n ; i++){
		A[i]=rand()%(a_max-a_min+1)+a_min; 
	}

//為了使用break 
//因此用一個沒有意義 只會跑一次的while
while (true){	
  if (is_p == true){ 
      
       cout<<"A= ";
       Printout_arrary_One(A,n);  
       break;
    }
  else{
   
    //IS
	IS(A,B,n);
    if (is_B_desc (B,n) == true){ break; }
   
    //MS
	MS(A,B,n);
    if (is_B_desc (B,n) == true){ break; }
    
    //HS
    HS(A,B,n);
    if (is_B_desc (B,n) == true){ break; }
    
    //QS
    QS(A,B,n);
    if (is_B_desc (B,n) == true){ break; }
   
    //CS
    CS(A,B,n);
    if (is_B_desc (B,n) == true){ break; }
    
    //RS
    RS(A,B,n);
    if (is_B_desc (B,n) == true){ break; }
    
    //BS
    BS(A,B,n);
    if (is_B_desc (B,n) == true){ break; }

  }
    break;
 }
   delete []A;
   delete []B;
   return 0;
}


 