/*
   This program can be compile and run ok

   Use:
   User input a string like {3~@$$]^[]&[*(*&*(&*(^%)$$2j4~!-+dz;m'k)'')]}
   judge whether the brackets are symmetrical using a stack and output where string should add brackets to correct.
   Usage(how to run):
     input a string and enter
   input file:
     none
   output file:
   none
   compile (how to compile):
     g++ -o h34064090_hw2 h34064090_hw2.cpp

   pseudocode:
    
   construct class node and class stack to implement STACK With Linked List 
   Ask user to input a string
   judge whether the characters in this string is { [ ( 
   if is , push they into Linked List
   and then judge whether the characters in this string is } ] )
   if is , meanwhile, the Top of Linked List is paired with this character
   then pop it out of Linked List and record the index of Valid pair
   if not , Push it into Linked List and record the index of InValid 

   Finally, if test is pass, output test pass
            but if not ,
            First, Calculate Number of left brackets (L) and right brackets (R) had search

            if Left brackets alone
              then if L>R
                 output after all element before this string element should add
              if L<R
                 output before all element before this string element should add                 
            if Right brackets alone       
              then if L<R
                 output after all element before this string element should add
              if L>R
                 output before all element before this string element should add    
            Until the linked list is empty.

   coded by Che-Wei YU, ID: H34064090, email: dessertn626@gmail.com
   date: 2018.11.11
*/
#include<iostream>
#include<string>
using namespace std;

//定義node   一個node裡面有data prev next
class node{
private:
    char data;
    node *prev,*next;
public:
  //constructor
    node():data(0){ next = 0; }
    node(char x): data(x){ next = 0; }
    node(char x, node *NextNode): data(x),next(NextNode){};
    friend class stack;  //和stack class共用private
};


//stack class
class stack {
private:
    node *top;  //stack的最上方元素
    int size;   //stack的大小 (Linked list 大小)
public:                 
  //constructor
    stack():size(0),top(0){};
    void Push(char x);
    void Pop();
    bool IsEmpty();
    char Top();
    int Size();
};

//push
void stack::Push(char x){

    if (IsEmpty()) {
        top = new node (x);  //創造一個node x當成top
        size++;
        return;
    }
    node *NewNode = new node(x); //每push一個node進去就分配一個node大小的空間給他  
    NewNode -> next = top;  //這個新的node(也就是head) 的next指向top
    top = NewNode; //把push進來的元素當成新的top
    size++; 
}

//pop
void stack::Pop(){

    if (IsEmpty()) {
        cout << "Stack is empty !";
        return;
    }
    node *DeleteNode = top; //將top放入一個node中
    top = top->next; //assign下一個top給原本的top
    delete DeleteNode; //刪掉top
    size--;
}

bool stack::IsEmpty(){

    return (size == 0); //如果size=0 就return True
}

char stack::Top(){

    //Linked list是空的就return -1
    if (IsEmpty()) { return -1; }
    return top -> data; //如果Linked list不是空的 就return top元素
}

int main(){

int L,R=0; //左右括號數量
string TempStr; //暫存輸入的原字串 (因未知大小 故先使用字串存)
cout<<"Please input a string:";
cin>>TempStr;
int InValidIdx [TempStr.size()] ={0} ;//存無效pair的index
int ValidIdx [TempStr.size()] ={0}; //存有效pair的index
stack chklist; //stack with linked list


//判斷括號  
//如果是左括號就push進chklist, 如果是右括號就判斷是否top是與其配對的左括號
 for (int i=0; i< (int)TempStr.size(); i++){
  if (TempStr[i] == '(' )
        chklist.Push(TempStr[i]);
  if (TempStr[i] == '[' )
        chklist.Push(TempStr[i]);
    if (TempStr[i] == '{' )
        chklist.Push(TempStr[i]);

//-----------------------------------------------  
  
    if (TempStr[i]== ')' && chklist.Top()== '('){
      chklist.Pop();  
      ValidIdx[i] = -1;
    }

    else if (TempStr[i] ==')'){
      chklist.Push(TempStr[i]);
      InValidIdx[i] = -1;
    }

    if (TempStr[i]== ']' && chklist.Top()== '['){
      chklist.Pop();
      ValidIdx[i] = -1; 
    }
    else if (TempStr[i] ==']') {
      chklist.Push(TempStr[i]);
      InValidIdx[i] = -1;
    }

    if (TempStr[i]== '}' && chklist.Top()== '{'){
      chklist.Pop();  
      ValidIdx[i] = -1;
    }
    else if (TempStr[i] =='}') {
      chklist.Push(TempStr[i]);
      InValidIdx[i] = -1;
    }
 }

    //判斷是否pass
    //若pass
    if (chklist.IsEmpty()) {
      cout<<"OK! The string ";
      cout<<TempStr;
      cout<<" passed the check!!";
    }
    //若沒pass
    else{

        for (int k=0; k<(int)TempStr.size(); k++){

          //計算已經掃到的左右括號數量
          if (TempStr[k]=='(' || TempStr[k]=='[' || TempStr[k]=='{' )
              L++;
          if (TempStr[k]==')' || TempStr[k]==']' || TempStr[k]=='}' )
              R++;

          //若是左括號落單
           if (chklist.Top() == '(' && ValidIdx[k] != -1){
              if(L > R){ //表示缺右括號在After
              cout<<"Error!! You should add ) After ";
              for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                    cout<<" !!"<<endl;
                    chklist.Pop();
              }
              else if (L < R){ //表示缺右括號在Before
            cout<<"Error!! You should add ) Before ";
              for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                    cout<<" !!"<<endl;
                    chklist.Pop();
              } 
            }
//-------------------------------------------------------------
           if (chklist.Top() == '[' && ValidIdx[k] != -1){
             if(L > R){
               cout<<"Error!! You should add ] After ";
             for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                   cout<<" !!"<<endl;
                   chklist.Pop();
               }  
             else if (L < R){
             cout<<"Error!! You should add ] Before ";
             for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                   cout<<" !!"<<endl;
                   chklist.Pop();
             }
           }  
//-------------------------------------------------------------
           if (chklist.Top() == '{' && ValidIdx[k] != -1){
              if(L > R){
                cout<<"Error!! You should add } After ";
              for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                    cout<<" !!"<<endl;
                    chklist.Pop();
              }
              else if (L < R){
                cout<<"Error!! You should add } Before ";
              for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                   cout<<" !!"<<endl;
                   chklist.Pop();
              }
           }

         //若是右括號落單
         if (chklist.Top() == ')' && InValidIdx[k] == -1){ 
            if(L < R){
               cout<<"Error!! You should add ( Before ";
               for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                    cout<<" !!"<<endl;
                    chklist.Pop();
              } 
              else if (L > R){
                cout<<"Error!! You should add ( After ";
              for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                   cout<<" !!"<<endl;
                   chklist.Pop();

              }
            }
//------------------------------------------------------------------
           if (chklist.Top() == ']' && InValidIdx[k] == -1){ 
             if(L < R){
             cout<<"Error!! You should add [ Before ";
             for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                 cout<<" !!"<<endl;
                 chklist.Pop(); 
             }
             else if (L > R){
               cout<<"Error!! You should add [ After ";
             for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                 cout<<" !!"<<endl;
                 chklist.Pop();

             }
           }
//------------------------------------------------------------------             
         if (chklist.Top() == '}' && InValidIdx[k] == -1){ 
              if(L < R){
              cout<<"Error!! You should add { Before ";
              for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                   cout<<" !!"<<endl;
                   chklist.Pop(); 
                }
              else if (L > R){
                cout<<"Error!! You should add { After ";
              for(int l=0; l<=k ; l++) { cout<<TempStr[l]; }
                   cout<<" !!"<<endl;
                   chklist.Pop();
            }
          }
          if (chklist.IsEmpty()) break;
        }
    }
  return 0;
}