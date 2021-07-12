module traffic_light (
    input  clk,
    input  rst,
    input  pass,
    output reg R,
    output reg G,
    output reg Y
);
    // 16 count for counting time (1024s, 512s, 128s)
    // the 16 is refer to tb.v file 
	reg [15:0]count = 16'd0;
	// 4 state (R, G, Y, N)
	reg [3:0]state = 4'b0000;

	// reset (green light 1024s)
	always@(posedge rst)begin
		state <=  4'b0000;
		count <= 16'b0;
		R = 0;
		G = 1;
		Y = 0;
	end
    
    // 1024G > 128N > 128G > 128N > 128G > 512Y > 1024R 
	always@(posedge clk)
	begin
		count <= count + 16'd1;
		// pass setting 
		// 若pass=true且不是初始狀態(G 1024)
		if(pass == 1'b1 && state!= 4'b0000)
		begin
			state <=  4'b0000;
			count <= 16'b1;
			R = 0;
			G = 1;
			Y = 0;
		end
		// G 1024 -> N 128
		if (state== 4'b0000 && count==16'd1024)
		 begin
			state <= 4'b0001;
			count <= 16'b1;
			R = 0;
			G = 0;
			Y = 0;
		end
		// N 128 -> G 128
		else if (state==4'b0001 && count==16'd128) 
		begin
			state <= 4'b0010;
			count <= 16'b1;
			R = 0;
			G = 1;
			Y = 0;
		end
		// G 128 -> N 128
		else if (state==4'b0010 && count==16'd128) 
		begin
			state <= 4'b0011;
			count <= 16'b1;
			R = 0;
			G = 0;
			Y = 0;
		end
		// N 128 -> G 128
		else if (state==4'b0011 && count==16'd128)
		 begin
			state <= 4'b0100;
			count <= 16'b1;
			R = 0;
			G = 1;
			Y = 0;
		end
		// G 128 -> Y 512
		else if (state==4'b0100 && count==16'd128) 
		begin
			state <= 4'b0101;
			count <= 16'b1;
			R = 0;
			G = 0;
			Y = 1;
		end
		// Y 512 -> R 1024
		else if (state==4'b0101 && count==16'd512)
		begin
			state <= 4'b0110;
			count <= 16'b1;
			R = 1;
			G = 0;
			Y = 0;
		end
		// R 1024 ->  G 1024
		else if (state==4'b0110 && count==16'd1024)
		begin
			state <=  4'b0000;
			count <= 16'b1;
			R = 0;
			G = 1;
			Y = 0;
		end
	end
endmodule