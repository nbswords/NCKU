module ALU(
	input	[1:0]	ALUOPCODE,
	input	[2:0]	funct3,
	input	[6:0]	funct7,
	input	[31:0]	rs1_data,
	input	[31:0]	rs2_data,
	input	[31:0]	imm,
	input			ALUSrc,
	output reg[31:0]alu_out
);
wire [31:0] rs1 = rs1_data;
wire [31:0] rs2;

assign rs2 = ALUSrc ? imm : rs2_data;

always @(rs1 or rs2 or funct3 or funct7 or ALUOPCODE) begin
	if(ALUOPCODE == 0) begin
		case(funct3)
			3'b000: begin
				alu_out <= rs1 + rs2;
			end
			3'b001: begin
				alu_out <= rs1 << rs2[4:0];
			end
			3'b010: begin
				alu_out <= $signed(rs1) < $signed(rs2) ? 1 : 0;
			end
			3'b011: begin
				alu_out <= rs1 < rs2 ? 1 : 0;
			end
			3'b100: begin
				alu_out <= rs1 ^ rs2;
			end
			3'b101: begin
				alu_out <= (funct7[5] && rs1[31]) ? ((rs1 >> rs2[4:0]) | ~(-32'd1 >> rs2[4:0])) : (rs1 >> rs2[4:0]);
			end
			3'b110: begin
				alu_out <= rs1 | rs2;
			end
			3'b111: begin
				alu_out <= rs1 & rs2;
			end
			default: begin end
		endcase
	end
	else if(ALUOPCODE == 1) begin
		case(funct3)
			3'b000: begin
				alu_out <= funct7[5] ? (rs1 - rs2) : (rs1 + rs2);
			end
			3'b001: begin
				alu_out <= rs1 << rs2[4:0];
			end
			3'b010: begin
				alu_out <= $signed(rs1) < $signed(rs2) ? 1 : 0;
			end
			3'b011: begin
				alu_out <= rs1 < rs2 ? 1 : 0;
			end
			3'b100: begin
				alu_out <= rs1 ^ rs2;
			end
			3'b101: begin
				alu_out <= (funct7[5] && rs1[31]) ? ((rs1 >> rs2[4:0]) | ~(-32'd1 >> rs2[4:0])) : (rs1 >> rs2[4:0]);
			end
			3'b110: begin
				alu_out <= rs1 | rs2;
			end
			3'b111: begin
				alu_out <= rs1 & rs2;
			end
			default: begin end
		endcase
	end
	else if(ALUOPCODE == 2) alu_out <= rs1 + rs2;
	else if(ALUOPCODE == 3) begin
		case(funct3)
			3'b000: begin
				alu_out <= rs1 == rs2 ? 1 : 0;
			end
			3'b001: begin
				alu_out <= rs1 != rs2 ? 1 : 0;
			end
			3'b100: begin
				alu_out <= $signed(rs1) < $signed(rs2) ? 1 : 0;
			end
			3'b101: begin
				alu_out <= $signed(rs1) >= $signed(rs2) ? 1 : 0;
			end
			3'b110: begin
				alu_out <= rs1 < rs2 ? 1 : 0;
			end
			3'b111: begin
				alu_out <= rs1 >= rs2 ? 1 : 0;
			end
			default: begin end
		endcase
	end
	else begin end
end


endmodule