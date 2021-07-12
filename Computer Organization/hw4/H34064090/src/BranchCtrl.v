module BranchCtrl(
    input        [6:0]     opcode,
    input        [2:0]     funct3,
    input        [31:0]    alu_out,
    input                  PCtoRegSrc,
    input        [31:0]    reg_instr_addr,
    input        [31:0]    imm,
    output reg   [31:0]    PC,
    output reg   [31:0]    pc_to_reg
);

always @(opcode or funct3 or alu_out or reg_instr_addr or imm) begin

    case(opcode)

        7'b1100111: begin
            PC <= alu_out;
        end
        7'b1100011: begin
            if(alu_out == 1)
                PC <= reg_instr_addr + imm;
            else
                PC <= reg_instr_addr + 4;
        end
        7'b1101111: begin
            PC <= reg_instr_addr + imm;
        end
        default: 
            PC <= reg_instr_addr + 4;
    endcase
end
always @(reg_instr_addr or imm or PCtoRegSrc or opcode) begin
	
    if(opcode == 7'b0110111) 
        pc_to_reg <= imm;
    else if(PCtoRegSrc == 0) 
        pc_to_reg <= reg_instr_addr + imm;
    else 
        pc_to_reg <= reg_instr_addr + 4;
end

endmodule