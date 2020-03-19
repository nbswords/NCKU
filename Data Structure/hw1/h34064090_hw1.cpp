/*
   This program can be compile and run ok

   Use:
   Ask user input a natual number n,
   an integral lower bound a_min, 
   an integral upper bound a_max, 
   and a random seed rseed.
   
   Produce a sequence of n random integer numbers with random seed rseed (i.e. A[0], A[1],..., A[n-1]) 
   uniformly distributed in the range [a_min, a_max]
   
   write a function to printout this array
   write another function to duplicate this array
   write two function for MergeSort and InsertionSort to sort numbers in DECENDING order
   Use two sort function to sort the duplicate array and printout

   input file:
	 n,a_min,a_max,rseed
   output file:
	 none
   compile (how to compile):
	 g++ -o h34064090_hw1 h34064090_hw1.cpp

   pseudocode:
   use two for loop to duplicate two dimension array
   and to printout
   
   MergeSort: Core idea is divide & conquer algorithm.
   Here, i separate original question into two sub question in every step.   
   
   InsertSort:Core idea is linear search and store a compared variable temporary.
   use linear search to find where to insert and use this temporary variable to compare. 

   coded by Che-Wei YU, ID: H34064090, email: dessertn626@gmail.com
   date: 2018.11.11
*/
#include <iostream>
#include <random>
using namespace std;

void Duplicate_array(int **A,int **B,int m,int n){

	for(int i=0; i<m; i++){
		for(int j=0 ; j<n; j++){
			B[i][j]=A[i][j];
		}
	}
}

void Printout_arrary(int **A,int m,int n){

	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			cout<<A[i][j]<<" ";
		}
	}
} 

/* Merge function:
	Let L[0...n1] and R[0...n2] be new arrays
	and copy B[start...end] to L[0...n1] 
	and copy B[start+1...end] to R[0...n2] */
void Merge(int **B, int start, int mid, int end) {

	int n1 = mid - start + 1; //子陣列的最後一個元素
	int n2 = end - mid;  //另一個子陣列的最後一個元素 

	int* L = new int [n1]; //Left subarray
	int* R = new int [n2]; //Right subarray


		for(int i=0; i<n1; i++){
			L[i] = B[0][start+i]; 
		}
	
		for(int j=0; j<n2; j++){
			R[j] = B[0][mid+j+1]; 
		}
	
	L[n1] = -2147483648; //將兩子陣列的最後一個元素設很小 
	R[n2] = -2147483648;
	
	int i = 0, j = 0; //i is Index of Left subarray
					  //j is Index of Right subarray
	//左右子陣列相比  比較大的就丟入B array中
	for (int k = start; k <= end; k++) {
		if (L[i] >= R[j]){ 
			B[0][k] = L[i];
			i++;
		}
		else {
			B[0][k] = R[j];
			j++;
		}
	}
}

/*Merge_Sort function:
   use recursive call function to sort two subarray
   and merge them */
void mg_sort(int **B, int start, int end) {
	if (start < end) {
		int mid = (start + end) / 2; //count the middle number m
		mg_sort(B, start, mid);  //Sort subarray B[start...m]
		mg_sort(B, mid+1, end); //Sort subarray B[m+1...end]
		Merge(B, start, mid, end); //Merge L and R
	}
}

//InsertSort function:
void ins_sort(int **A, int n){
  //A是要排序的陣列，n是陣列大小
  for(int next=1; next<n; next++){//next要移動的數的位置
	int insert = A[0][next];//insert = 暫存要移動的數
	int move = next;//move = 最大移動次數
	
	while( (move>0) && (A[0][move-1]<insert) ){
	  //條件一:(move>0)避免移動到arr[-1]
	  //條件二:左數小於右數的時候，將右數覆寫成左數
	  A[0][move] = A[0][move-1];
	  move--;
	}
	A[0][move] = insert;
	//把要移動的數插入到正確的位置上
  }
}

int main() {
	int n,a_min,a_max;
	int m=1;//A,B陣列第一維大小固定為1
	unsigned int rseed;

	cout<<"Please input [n, a_min, a_max, rseed]: "<<endl;
	cin>>n>>a_min>>a_max>>rseed;
    cout<<endl;
    
	srand(rseed);

	//construct A[][]
	int **A= new int *[m];
	for (int i=0 ; i<m ; i++){
		A[i] = new int [n];
	}

	//將亂數產生的n個數傳入A[0][]
	for (int i=0 ; i<n ; i++){
		A[0][i]=rand()%(a_max-a_min+1)+a_min; 
	}

	cout<<"A= ";
	Printout_arrary(A,m,n);
	cout<<endl;
	
	//construct B[][]
	int **B= new int *[m];
	for (int i=0; i<m; i++){
		B[i] = new int [n];
	}
	Duplicate_array(A,B,m,n);
	ins_sort(A,n);
	cout<<"By insertion sort: "<<endl;
	cout<<"A= ";
	Printout_arrary(A,m,n);
	
	cout<<endl;
	mg_sort(B,0,n-1);
	cout<<"By merge sort: "<<endl;
	cout<<"A= ";
	Printout_arrary(B,m,n);
	
	return 0;
}


