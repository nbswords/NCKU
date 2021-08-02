from tkinter import *

class Application(Frame):

    def __init__(self, master):
        super(Application, self).__init__(master)  
        self.grid()
        self.create_widgets()

    def create_widgets(self):
 
        # 第一個數字的Lebel和其輸入框
        self.num1 = Label(self, text = "1st number :")
        self.num1.grid(row=0, column=0, sticky=W)

        self.one_ent = Entry(self, width = 6)
        self.one_ent.grid(row = 0, column = 1, sticky = W)

        # 第二個數字的Lebel和其輸入框
        self.num2 = Label(self, text = "2nd number :")
        self.num2.grid(row=1, column=0, sticky=W)

        self.two_ent = Entry(self, width = 6)
        self.two_ent.grid(row = 1, column = 1, sticky = W)

        # 運算Label
        self.cal = Label(self, text = "Operation : ")
        self.cal.grid(row = 2, column = 0, sticky = W)
        
        # 四個選擇按鈕 +,-,*,/
        self.sign = StringVar()
        self.sign.set(None)

        Radiobutton(self,width = 3,text = "+",
        variable=self.sign,value = "+",).grid(row = 2, column = 1, sticky = W)

        Radiobutton(self,width = 3,text = "-",
        variable=self.sign,value = "-", ).grid(row = 2, column = 2, sticky = W)

        Radiobutton(self,width = 3,text = "*",
        variable=self.sign,value = "*",).grid(row = 2, column = 3, sticky = W)

        Radiobutton(self,width = 3,text = "/", 
        variable=self.sign, value = "/",).grid(row = 2, column = 4, sticky = W)

        # result/Run 按鈕
        self.run_bttn = Button(self, text = "Run/Result", command = self.calculate)
        self.run_bttn.grid(row = 3, column = 0, sticky = W)

        # result/Run 結果
        self.result = Text(self, width = 5, height = 1, wrap = WORD)
        self.result.grid(row = 3, column = 1, sticky = W)  

    def calculate(self):
        number1 = eval(self.one_ent.get())
        number2 = eval(self.two_ent.get())
        operation = self.sign.get()
        if operation =="+":
            Result=number1+number2

        if operation =="-":
            Result=number1-number2
            
        if operation =="*":
            Result=number1*number2
            
        if operation =="/":
            Result=number1/number2

        self.result.delete(0.0, END)
        self.result.insert(0.0, str(Result))

# main func
root = Tk()
root.title("Calculate")
app = Application(root)
root.mainloop()
