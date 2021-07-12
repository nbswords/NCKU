module fpadder (src1 , src2 ,out);

//src1和src2是輸入的兩個數字
input[31:0]src1 ; 
input[31:0]src2 ;
output[31:0]out ;

reg [31:0]out ; //暫存的輸出

reg sign1  ; // 第一個數字的sign
reg sign2  ; // 第二個數字的sign
reg sign_ans ; // 答案的sign

reg g ; // guard bit
reg r ; // round bit
reg s ; // sticky bit

reg [7:0] exp1 ; // 第一個數字的exponent 8bit
reg [7:0] exp2 ; // 第二個數字的exponent 8bit
reg [7:0] diff ; // 兩數字exponent的差 8bit
reg [7:0] exp_ans  ; // 答案的exponent 8bit

reg [23:0]frac1  ; // 第一個數字的fraction 23bit + hidden 1 
reg [23:0]frac2  ; // 第二個數字的fraction 23bit + hideen 1
reg [26:0]frac1_normalized ; // 23+3(GRS) bit
reg [26:0]frac2_normalized ;

reg [27:0]frac_sum ; 
reg [26:0]frac_sum_normalized ;
reg [22:0]frac_ans ; //答案的fraction 

reg [4:0] i ; //for loop 的 i
reg [4:0] counter ;  //計算加起來之後的位移量


//single precision floating point三種情況:
// 1. Exponent and    Fraction都是0         -> 答案就是0
// 2. Exponent =0     and Fraction!=0       -> +-的"denormalizedmailize" number
// 3. Exponent 1-254  and Fraction為任意數   -> +-的float


always@* begin
    sign1 = src1[31] ; //src1的最後一個bit是sign  
    sign2 = src2[31] ; //src2的最後一個bit也是sign
    i = 0 ; 
    // 初始化
    g = 0 ;
    r = 0 ; 
    s = 0 ; 

//------------先判斷exponent和fraction-------------

 //如果src1的exponent是0 就是 Denormalized number
 if(src1[30:23] == 0) begin 

    exp1 = 8'b00000001 ;
    frac1 = {1'b0 , src1[22:0]} ;  

 end
 
  //如果src1的exponent不是0
 else begin
  
  exp1  = src1[30:23] ; 
  frac1 = {1'b1 , src1[22:0]} ; 
 end
 
//如果src2的exponent是0 就是 Denormalized number
 if(src2[30:23] == 0)begin

  exp2  = 8'b00000001 ; 
  frac2 = {1'b0 , src2[22:0]} ; 
 end

// 如果src2的exponent不是0
 else begin 

  exp2  = src2[30:23] ;
  frac2 = {1'b1 , src2[22:0]} ;  
 end

  
 // 第一個數字的exponent和fraction都是0的情況
 // 則第一個數字是0  可以無視 直接將sign, exp, frac都給src2
 if ((exp1 == 0) && (frac1== 0)) begin  

    sign_ans = sign2 ; 
    exp_ans  = exp2 ; 
    frac_sum = frac2 ; 
 end
 // 第二個數字的exponent和fraction都是0的情況
 // 則第二個數字是0 可以無視 直接將sign, exp, frac都給src1
 else if ((exp2 == 0) && (frac2 == 0)) begin 

 	sign_ans = sign1 ; 
    exp_ans  = exp1 ; 
    frac_sum = frac1 ; 
 end
 
 else begin
  
  // 兩數字exponent相同的情況
  if(exp1 == exp2) begin
    exp_ans = exp1 ; 
    if(sign1 == sign2) begin

      frac1_normalized = {frac1 , 3'b000} ; 
      frac2_normalized = {frac2  , 3'b000} ;  
      sign_ans = sign1 ;
      g = 0 ; 
      r = 0 ; 
      s = 0 ; 
      frac_sum = frac1_normalized + frac2_normalized ; 
    end
   
    else begin
    
    if(frac1 > frac2) begin

      g = 0 ; 
      r = 0 ; 
      s = 0 ; 
      frac1_normalized = {frac1 , 3'b000} ; 
      frac2_normalized = {frac2 , 3'b000} ;  
      frac_sum = frac1_normalized - frac2_normalized; 
      sign_ans = sign1 ; 
    end
    
    else if(frac1 == frac2) begin
     
      frac_sum = 0 ; 
      sign_ans = 0 ;
      exp_ans = 0 ;  
    end
    //frac1 < frac2
    else begin
     
      g = 0 ;
      r = 0 ;
      s = 0 ; 
      frac1_normalized = {frac1 , 3'b000} ; 
      frac2_normalized = {frac2 , 3'b000} ; 
      frac_sum = frac2_normalized - frac1_normalized ; 
      sign_ans = sign2 ; 
    end
   end
  end

  // exp1不等於exp2
  else begin 
    //exp1 > exp2-------------------------------------------
    if(exp1 > exp2) begin
       exp_ans  = exp1  ; 
       sign_ans = sign1 ;
       diff = exp1 - exp2 ; 
    // s bit
    // 只在diff > 2且 <=24的情況下才討論s bit
        if(diff > 2 && diff <= 24) begin
            for(i = 0 ; i < diff - 2 ; i = i + 1) begin
                if(frac2[i] == 1) begin
                  s = 1 ; 
                end
            end
        end
        // diff <= 2 或者 >24 (超出範圍 不討論s)
                else begin
                     s = 0 ;
                end

        // guard bit
        // 如果diff <=24  則 g bit就是倒數第2個數
        if(diff <= 24) begin
           g  = frac2[diff - 1] ; 
        end
        // 超出範圍 , 沒有g
        else begin
             g = 0 ; 
        end
        // round bit
        // diff > 1 且 <=25 , r是倒數第1個數
        if(diff > 1 && diff <= 25) begin
           r = frac2[diff - 2] ; 
        end
        // 超出範圍 , 沒有r
        else begin
             r = 0 ; 
        end
    
        // normalized fraction
        frac1_normalized = {frac1 , 3'b000} ;  
        frac2     =  frac2 >> diff ;
        frac2_normalized = {frac2[23:0] , g , r , s} ;  

        // 同號就+
        if(sign1 == sign2) begin
           frac_sum = frac1_normalized + frac2_normalized ; 
        end

        //異號就-
        else begin
             frac_sum = frac1_normalized - frac2_normalized ; 
        end 
    end

    // exp1 < exp2----------------------------------
    else if(exp1 < exp2) begin
        exp_ans  = exp2  ; 
        sign_ans = sign2 ; 
        diff = exp2 - exp1 ;
        // sticky bit
        // 只在diff >= 3且 <=24 的情況下才討論s
        if(diff >= 3 && diff <= 24) begin
            for(i = 0 ; i < diff - 2 ; i = i + 1) begin
                if(frac1[i] == 1) begin
                    s = 1 ; 
                end
            end
        end
    // diff <= 24 或者 >24 (超出範圍 不討論s)
        else begin
            s=0;
        end
    
    // guard bit
    // 如果diff <=24  則 g 就是倒數第2個數
    if(diff <= 24) begin
       g  = frac1[diff - 1] ; 
    end
    // 超出範圍 沒有 g
    else begin
       g = 0 ; 
    end
    // round bit
    // 如果diff >=2 且 <= 25 則 r 就是倒數第一個數
    if(diff >= 2 && diff <= 25) begin
       r = frac1[diff - 2] ; 
    end
    // 超出範圍 沒有 r
    else begin
       r = 0 ; 
    end
    // normalized fraction
    frac2_normalized = {frac2 , 3'b000} ; 
    frac1     =  frac1 >> diff ; 
    frac1_normalized = {frac1 , g , r , s} ; 
    // 同號就+
    if(sign1 == sign2) begin       
       frac_sum = frac2_normalized + frac1_normalized ;  
    end
    // 異號就-
    else begin
       frac_sum = frac2_normalized - frac1_normalized ; 
    end
   end
  end
 end 
  
//----------處理加起來之後的fraction,exponent--------------------------
//----------判斷是否要進位
  

  if(frac_sum[27] == 1'b1) begin
  	// GRS > 100
    if( {frac_sum[2], frac_sum[1], frac_sum[0]} > 3'b100) begin
       frac_sum = frac_sum + 4'b1000 ;
    end 
    // GRS = 100s
    else if( {frac_sum[2], frac_sum[1], frac_sum[0]} == 3'b100) begin
    	    // G前面的如果是1
            if(frac_sum[3] == 1) begin
               frac_sum = frac_sum + 4'b1000 ; 
            end
            // G前面的不是1
            else begin
               frac_sum = frac_sum ; 
            end
         end
    
    else begin
      frac_sum = frac_sum ; 
    end
    //hidden 1
    exp_ans = exp_ans + 1 ;
    //fraction 右移 
    frac_sum = frac_sum >> 1 ; 

    // GRS > 100
    if({frac_sum[2], frac_sum[1], frac_sum[0]} > 3'b100) begin
      frac_sum = frac_sum + 4'b1000 ; 
    end
    // GRS = 100
    else if({frac_sum[2] , frac_sum[1] , frac_sum[0]} == 3'b100) begin
        // G前面的如果是1
        if(frac_sum[3] == 1) begin
           frac_sum = frac_sum + 4'b1000 ; 
        end
        // G前面的不是1
        else begin
             frac_sum = frac_sum ; 
        end
    end
    // GRS < 100
    else begin 
        frac_sum = frac_sum ; 
    end
        frac_ans = frac_sum[25:3] ; 
    end
  
  // 幾個1就位移幾位 
  else begin
   if(frac_sum[26] == 1)
    counter = 0 ;
   else if(frac_sum[25] == 1)
    counter = 1 ; 
   else if(frac_sum[24] == 1)
    counter = 2 ; 
   else if(frac_sum[23] == 1)
    counter = 3 ; 
   else if(frac_sum[22] == 1)
    counter = 4 ; 
   else if(frac_sum[21] == 1)
    counter = 5 ; 
   else if(frac_sum[20] == 1)
    counter = 6 ; 
   else if(frac_sum[19] == 1)
    counter = 7 ; 
   else if(frac_sum[18] == 1)
    counter = 8 ; 
   else if(frac_sum[17] == 1)
    counter = 9 ; 
   else if(frac_sum[16] == 1)
    counter = 10 ; 
   else if(frac_sum[15] == 1)
    counter = 11 ; 
   else if(frac_sum[14] == 1)
    counter = 12 ; 
   else if(frac_sum[13] == 1)
    counter = 13 ; 
   else if(frac_sum[12] == 1)
    counter = 14 ; 
   else if(frac_sum[11] == 1)
    counter = 15 ; 
   else if(frac_sum[10] == 1)
    counter = 16 ; 
   else if(frac_sum[9]  == 1)
    counter = 17 ; 
   else if(frac_sum[8]  == 1)
     counter = 18 ;
   else if(frac_sum[7]  == 1)
    counter = 19 ; 
   else if(frac_sum[6]  == 1)
    counter = 20 ; 
   else if(frac_sum[5]  == 1)
    counter = 21 ; 
   else if(frac_sum[4]  == 1)
    counter = 22 ; 
   else if(frac_sum[3]  == 1)
    counter = 23 ; 
   else if(frac_sum[2]  == 1)
    counter = 24 ; 
   else if(frac_sum[1]  == 1)
    counter = 25 ; 
   else if(frac_sum[0]  == 1)
    counter = 26 ; 
   else begin
    counter = 0 ; 
    exp_ans = 0 ; 
    frac_sum = 0 ; 
   end
   //shift fraction 並把 shift後的fraction當成answer
   exp_ans = exp_ans - counter ;
   frac_sum = frac_sum << counter ; 
   frac_ans = frac_sum[25:3] ; 
   

   // 最後一次的round
   // 只要frac後三位(GRS)大於100都要進位
   if({frac_sum[2] , frac_sum[1] , frac_sum[0]} > 3'b100) begin 
    frac_ans = frac_ans + 1 ; 
   end
   // frac後三位等於100的話要判斷正負
   else if({frac_sum[2] , frac_sum[1] , frac_sum[0]} == 3'b100) begin
    //負數
    if(frac_ans[0] == 1) begin
     frac_ans = frac_ans + 1 ; 
    end
    //正數
    else begin
     frac_ans = frac_ans ; 
    end 
   end
   // fraction後三位<100
   else begin
    frac_ans = frac_ans ; 
   end
 end

   // 有exponent = 255的情況 !?  痾...^$^%%#$#%%%^%$#$#$%#$%
   if(exp_ans == 8'b11111111) begin
      frac_ans = 0 ; 
   end
  out = {sign_ans  , exp_ans , frac_ans} ;
end

endmodule