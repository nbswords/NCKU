module fpadder (src1 , src2 ,out);

//src1�Msrc2�O��J����ӼƦr
input[31:0]src1 ; 
input[31:0]src2 ;
output[31:0]out ;

reg [31:0]out ; //�Ȧs����X

reg sign1  ; // �Ĥ@�ӼƦr��sign
reg sign2  ; // �ĤG�ӼƦr��sign
reg sign_ans ; // ���ת�sign

reg g ; // guard bit
reg r ; // round bit
reg s ; // sticky bit

reg [7:0] exp1 ; // �Ĥ@�ӼƦr��exponent 8bit
reg [7:0] exp2 ; // �ĤG�ӼƦr��exponent 8bit
reg [7:0] diff ; // ��Ʀrexponent���t 8bit
reg [7:0] exp_ans  ; // ���ת�exponent 8bit

reg [23:0]frac1  ; // �Ĥ@�ӼƦr��fraction 23bit + hidden 1 
reg [23:0]frac2  ; // �ĤG�ӼƦr��fraction 23bit + hideen 1
reg [26:0]frac1_normalized ; // 23+3(GRS) bit
reg [26:0]frac2_normalized ;

reg [27:0]frac_sum ; 
reg [26:0]frac_sum_normalized ;
reg [22:0]frac_ans ; //���ת�fraction 

reg [4:0] i ; //for loop �� i
reg [4:0] counter ;  //�p��[�_�Ӥ��᪺�첾�q


//single precision floating point�T�ر��p:
// 1. Exponent and    Fraction���O0         -> ���״N�O0
// 2. Exponent =0     and Fraction!=0       -> +-��"denormalizedmailize" number
// 3. Exponent 1-254  and Fraction�����N��   -> +-��float


always@* begin
    sign1 = src1[31] ; //src1���̫�@��bit�Osign  
    sign2 = src2[31] ; //src2���̫�@��bit�]�Osign
    i = 0 ; 
    // ��l��
    g = 0 ;
    r = 0 ; 
    s = 0 ; 

//------------���P�_exponent�Mfraction-------------

 //�p�Gsrc1��exponent�O0 �N�O Denormalized number
 if(src1[30:23] == 0) begin 

    exp1 = 8'b00000001 ;
    frac1 = {1'b0 , src1[22:0]} ;  

 end
 
  //�p�Gsrc1��exponent���O0
 else begin
  
  exp1  = src1[30:23] ; 
  frac1 = {1'b1 , src1[22:0]} ; 
 end
 
//�p�Gsrc2��exponent�O0 �N�O Denormalized number
 if(src2[30:23] == 0)begin

  exp2  = 8'b00000001 ; 
  frac2 = {1'b0 , src2[22:0]} ; 
 end

// �p�Gsrc2��exponent���O0
 else begin 

  exp2  = src2[30:23] ;
  frac2 = {1'b1 , src2[22:0]} ;  
 end

  
 // �Ĥ@�ӼƦr��exponent�Mfraction���O0�����p
 // �h�Ĥ@�ӼƦr�O0  �i�H�L�� �����Nsign, exp, frac����src2
 if ((exp1 == 0) && (frac1== 0)) begin  

    sign_ans = sign2 ; 
    exp_ans  = exp2 ; 
    frac_sum = frac2 ; 
 end
 // �ĤG�ӼƦr��exponent�Mfraction���O0�����p
 // �h�ĤG�ӼƦr�O0 �i�H�L�� �����Nsign, exp, frac����src1
 else if ((exp2 == 0) && (frac2 == 0)) begin 

 	sign_ans = sign1 ; 
    exp_ans  = exp1 ; 
    frac_sum = frac1 ; 
 end
 
 else begin
  
  // ��Ʀrexponent�ۦP�����p
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

  // exp1������exp2
  else begin 
    //exp1 > exp2-------------------------------------------
    if(exp1 > exp2) begin
       exp_ans  = exp1  ; 
       sign_ans = sign1 ;
       diff = exp1 - exp2 ; 
    // s bit
    // �u�bdiff > 2�B <=24�����p�U�~�Q��s bit
        if(diff > 2 && diff <= 24) begin
            for(i = 0 ; i < diff - 2 ; i = i + 1) begin
                if(frac2[i] == 1) begin
                  s = 1 ; 
                end
            end
        end
        // diff <= 2 �Ϊ� >24 (�W�X�d�� ���Q��s)
                else begin
                     s = 0 ;
                end

        // guard bit
        // �p�Gdiff <=24  �h g bit�N�O�˼Ʋ�2�Ӽ�
        if(diff <= 24) begin
           g  = frac2[diff - 1] ; 
        end
        // �W�X�d�� , �S��g
        else begin
             g = 0 ; 
        end
        // round bit
        // diff > 1 �B <=25 , r�O�˼Ʋ�1�Ӽ�
        if(diff > 1 && diff <= 25) begin
           r = frac2[diff - 2] ; 
        end
        // �W�X�d�� , �S��r
        else begin
             r = 0 ; 
        end
    
        // normalized fraction
        frac1_normalized = {frac1 , 3'b000} ;  
        frac2     =  frac2 >> diff ;
        frac2_normalized = {frac2[23:0] , g , r , s} ;  

        // �P���N+
        if(sign1 == sign2) begin
           frac_sum = frac1_normalized + frac2_normalized ; 
        end

        //�����N-
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
        // �u�bdiff >= 3�B <=24 �����p�U�~�Q��s
        if(diff >= 3 && diff <= 24) begin
            for(i = 0 ; i < diff - 2 ; i = i + 1) begin
                if(frac1[i] == 1) begin
                    s = 1 ; 
                end
            end
        end
    // diff <= 24 �Ϊ� >24 (�W�X�d�� ���Q��s)
        else begin
            s=0;
        end
    
    // guard bit
    // �p�Gdiff <=24  �h g �N�O�˼Ʋ�2�Ӽ�
    if(diff <= 24) begin
       g  = frac1[diff - 1] ; 
    end
    // �W�X�d�� �S�� g
    else begin
       g = 0 ; 
    end
    // round bit
    // �p�Gdiff >=2 �B <= 25 �h r �N�O�˼ƲĤ@�Ӽ�
    if(diff >= 2 && diff <= 25) begin
       r = frac1[diff - 2] ; 
    end
    // �W�X�d�� �S�� r
    else begin
       r = 0 ; 
    end
    // normalized fraction
    frac2_normalized = {frac2 , 3'b000} ; 
    frac1     =  frac1 >> diff ; 
    frac1_normalized = {frac1 , g , r , s} ; 
    // �P���N+
    if(sign1 == sign2) begin       
       frac_sum = frac2_normalized + frac1_normalized ;  
    end
    // �����N-
    else begin
       frac_sum = frac2_normalized - frac1_normalized ; 
    end
   end
  end
 end 
  
//----------�B�z�[�_�Ӥ��᪺fraction,exponent--------------------------
//----------�P�_�O�_�n�i��
  

  if(frac_sum[27] == 1'b1) begin
  	// GRS > 100
    if( {frac_sum[2], frac_sum[1], frac_sum[0]} > 3'b100) begin
       frac_sum = frac_sum + 4'b1000 ;
    end 
    // GRS = 100s
    else if( {frac_sum[2], frac_sum[1], frac_sum[0]} == 3'b100) begin
    	    // G�e�����p�G�O1
            if(frac_sum[3] == 1) begin
               frac_sum = frac_sum + 4'b1000 ; 
            end
            // G�e�������O1
            else begin
               frac_sum = frac_sum ; 
            end
         end
    
    else begin
      frac_sum = frac_sum ; 
    end
    //hidden 1
    exp_ans = exp_ans + 1 ;
    //fraction �k�� 
    frac_sum = frac_sum >> 1 ; 

    // GRS > 100
    if({frac_sum[2], frac_sum[1], frac_sum[0]} > 3'b100) begin
      frac_sum = frac_sum + 4'b1000 ; 
    end
    // GRS = 100
    else if({frac_sum[2] , frac_sum[1] , frac_sum[0]} == 3'b100) begin
        // G�e�����p�G�O1
        if(frac_sum[3] == 1) begin
           frac_sum = frac_sum + 4'b1000 ; 
        end
        // G�e�������O1
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
  
  // �X��1�N�첾�X�� 
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
   //shift fraction �ç� shift�᪺fraction��answer
   exp_ans = exp_ans - counter ;
   frac_sum = frac_sum << counter ; 
   frac_ans = frac_sum[25:3] ; 
   

   // �̫�@����round
   // �u�nfrac��T��(GRS)�j��100���n�i��
   if({frac_sum[2] , frac_sum[1] , frac_sum[0]} > 3'b100) begin 
    frac_ans = frac_ans + 1 ; 
   end
   // frac��T�쵥��100���ܭn�P�_���t
   else if({frac_sum[2] , frac_sum[1] , frac_sum[0]} == 3'b100) begin
    //�t��
    if(frac_ans[0] == 1) begin
     frac_ans = frac_ans + 1 ; 
    end
    //����
    else begin
     frac_ans = frac_ans ; 
    end 
   end
   // fraction��T��<100
   else begin
    frac_ans = frac_ans ; 
   end
 end

   // ��exponent = 255�����p !?  ��...^$^%%#$#%%%^%$#$#$%#$%
   if(exp_ans == 8'b11111111) begin
      frac_ans = 0 ; 
   end
  out = {sign_ans  , exp_ans , frac_ans} ;
end

endmodule