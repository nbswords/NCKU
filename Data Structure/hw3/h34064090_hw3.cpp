/*
   This program can be compile and run ok

   Use:
   Ask user input a natual number n,
   an integral lower bound a_min, 
   an integral upper bound a_max, 
   
   Produce a sequence of n random integer numbers (i.e. A[0], A[1],..., A[n-1]) 
   uniformly distributed in the range [a_min, a_max]
   
   write a function to printout this array
   write another function to duplicate this array
   write two function for HeapSort and QuickSort to sort numbers in  INCREASING  order
   which heap will be a MAX HEAP
   then use two sort function to sort the duplicate array and printout

   input file:
	 n,a_min,a_max
   output file:
	 none
   compile (how to compile):
	 g++ -o h34064090_hw3 h34064090_hw3.cpp

   pseudocode:

   use two for loop to duplicate two dimension array
   and to printout

   HeapSort: Core idea is Binary tree.
   every element of array is a node.
   One parent have two childs
            Parent
           /     \
       Child_1   Child_2
   in max heap Parent > child_1 and child_2
   
   use this property , first Max_heapify the array (tree)
   exchange first element(node) in max heap with last element(node)
   which first is the largest element in the array and last is the smallest

   now the largest is in the last place
   and the smallest is in the first place

   then delete element in the last place (largest element) 
   Max_heapify the new tree again
   
   Finally, repeat these steps and 
   check every node which have childs 
   
   QuickSort: Core idea is divide & conquer
   First set pick a element in array as Pivot
   Use Pivot to separte
   array into p...Pivot-1 , Pivot , Pivot+1...r
   which p...Pivot-1 are <= Pivot
   and Pivot+1...r are > Pivot


   coded by Che-Wei YU, ID: H34064090, email: dessertn626@gmail.com
   date: 2018.12.2
*/
#include <iostream>
#include <ctime>
using namespace std;

void Duplicate_array(int **A,int **B,int m,int n){

	for(int i=0; i<m; i++){
		for(int j=1 ; j<=n; j++){
			B[i][j]=A[i][j];
		}
	}
}

void Printout_arrary(int **A,int m,int n){
	for(int i=0; i<m; i++){
		for(int j=1; j<=n; j++){
			cout<<A[i][j]<<" ";
		}
	}
} 

//交換陣列元素
void Swap(int &a,int &b) {
    int temp;
    temp = a;
    a = b;
    b = temp;
}

//將tree變成Max heap
void Max_heapify (int **A,int Parent,int n){

    int L = 2*Parent;       // Left child 左子代的index
    int R = 2*Parent + 1;   // Right child 右子代的index
    int Largest;          // parent和兩個child間值最大node的index

    // 左子代和父母比較  比較大的則標為最大
    if (L <= n && A[0][L] > A[0][Parent])
        Largest = L;
    else
        Largest = Parent;

    //右子代和最大的比較  比較大的標為最大
    if (R <= n && A[0][R] > A[0][Largest])
        Largest = R;

    if (Largest != Parent) {                //如果目前Parent不是三者中的最大數
        Swap(A[0][Largest], A[0][Parent]);  // 就交換Parent和最大的node的位置
        Max_heapify(A, Largest, n);         // 重新將新的subtree變成Maxheap tree
    }
}


void Build_Max_heap (int **A,int n){
	for (int i=n/2 ; i >= 1 ; i--){ //從n/2 -> 1 (檢查每個有child的node)
		Max_heapify(A,i,n);
	}
}

void Heap_Sort(int**A,int n){
    Build_Max_heap(A,n);                // 將A初始化成Max Heap
    int size = n;
    for (int i = n ; i >= 2 ; i--) {
        Swap(A[0][1], A[0][i]);        // 將最大的node和最後一個node位置交換
        size--;                        //抽掉最後一個node
        Max_heapify(A,1,size);         // 重新把新的subtree變成Maxheap tree
    }
}


//找出Pivot並且Divide
int Partition(int**B,int p,int r){
	int Pivot = B[0][r];
    int i = p-1;
    for (int j=p ; j<r ; j++){
    	if (B[0][j] <= Pivot){
    		i++;
    		Swap(B[0][i],B[0][j]);
    	}
    }
    i++;
    Swap(B[0][i],B[0][r]);
    return i;
}

void Quick_Sort(int**B,int p,int r){
	if (p < r){
		int q=Partition(B,p,r);
		Quick_Sort(B,p,q-1);
		Quick_Sort(B,q+1,r);
	}
}

int main() {
	int n,a_min,a_max;
	int m = 1; //A,B陣列第一維大小固定為1
    srand( time(NULL) ); //利用電腦時間當作亂數種子
	cout<<"Please input [n, a_min, a_max]: "<<endl;
	cin>>n>>a_min>>a_max;
    cout<<endl;

	//construct A[0][0....n]
	//若只construct A[0][1....n]會malloc error 想嘗試可改掉後用 10 10 100測資去測
	//但只會用到 A[0][1....n]
	//原因是因為heap sort的特性 將第一個元素設為A[0][i]可以確定其
	//left child必定位在A[0][2i]；
  //right child必定位在A[0][2i+1]；
	int **A= new int *[m];
	for (int i=0 ; i<m ; i++){
		A[i] = new int [n+1];
	}

	//將亂數產生的n個數傳入A[0][1...n]
	for (int i=1 ; i<=n ; i++){
		A[0][i]=rand()%(a_max-a_min+1)+a_min; 
	}
  cout<<"A= ";
  Printout_arrary(A,m,n);
  cout<<endl;

  //construct B[0][0....n]
  //但只會用到 B[0][1....n]
  int **B= new int *[m];
  for (int i=0; i<m; i++){
    B[i] = new int [n+1];
  }
    

	Duplicate_array(A,B,m,n);

	Heap_Sort(A,n);
	cout<<"By Heap sort: "<<endl;
	cout<<"A= ";
	Printout_arrary(A,m,n);
    
 	cout<<endl;
	Quick_Sort(B,1,n);   //要排的數字是從B[0][1]到B[0][n]
	cout<<"By Quick sort: "<<endl;
	cout<<"B= ";
	Printout_arrary(B,m,n);    

}