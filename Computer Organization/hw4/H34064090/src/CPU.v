`include "Decoder.v"
`include "ImmGen.v"
`include "BranchCtrl.v"
`include "ALU.v"

module CPU(
    input         clk,
    input         rst,
    output        instr_read,
    output [31:0] instr_addr,
    input  [31:0] instr_out,
    output        data_read,
    output        data_write,
    output [31:0] data_addr,
    output [31:0] data_in,
    input  [31:0] data_out
);

reg         reg_instr_read;
reg [31:0]  reg_instr_addr;
reg [31:0]  reg_data_addr;
reg         reg_data_read;
reg         reg_data_write;
reg [31:0]  reg_data_in;


assign instr_read 	= reg_instr_read;
assign instr_addr 	= reg_instr_addr;
assign data_addr 	= reg_data_addr;
assign data_in 		= reg_data_in;
assign data_read 	= reg_data_read;
assign data_write 	= reg_data_write;

reg [2:0]       state;
reg [31:0]      x[31:0];
reg [31:0]      reg_rs1_data;
reg [31:0]      reg_rs2_data;
reg [31:0]      reg_imm;
reg [31:0]      reg_alu_out;

wire            RegWrite;
wire [1:0]      ALUOPCODE;
wire            PCtoRegSrc;
wire            ALUSrc;
wire            RDSrc;
wire            MemtoReg;

wire [4:0]      rs1_addr = instr_out[19:15];
wire [4:0]      rs2_addr = instr_out[24:20];
wire [4:0]      rd_addr  = instr_out[11:7];
wire [31:0]     rs2_data = x[rs2_addr < 32 ? rs2_addr : 0];
wire [2:0]      funct3   = instr_out[14:12];
wire [6:0]      funct7   = instr_out[31:25];
wire [6:0]      opcode   = instr_out[6:0];
wire [31:0]     imm;
wire [31:0]     PC;
wire [31:0]     alu_out;
wire [31:0]     pc_to_reg;
wire [31:0]     pc_or_alu_out = RDSrc ? pc_to_reg : alu_out;
wire [31:0]     rd_data = MemtoReg ? data_out : pc_or_alu_out;
wire            later_data_read;
wire            later_data_write;

Decoder Decoder(
    .opcode(opcode),
    .RegWrite(RegWrite),
    .ALUOPCODE(ALUOPCODE),
    .PCtoRegSrc(PCtoRegSrc),
    .ALUSrc(ALUSrc),
    .RDSrc(RDSrc),
    .MemRead(later_data_read),
    .MemWrite(later_data_write),
    .MemtoReg(MemtoReg)
);
ImmGen ImmGen(
    .instr(instr_out),
    .opcode(opcode),
    .imm(imm)
);
BranchCtrl BranchCtrl(
    .opcode(opcode),
    .funct3(funct3),
    .alu_out(alu_out),
    .PCtoRegSrc(PCtoRegSrc),
    .reg_instr_addr(reg_instr_addr),
    .imm(reg_imm),
    .PC(PC),
    .pc_to_reg(pc_to_reg)
);
ALU ALU(
    .ALUOPCODE(ALUOPCODE),
    .funct3(funct3),
    .funct7(funct7),
    .rs1_data(reg_rs1_data),
    .rs2_data(reg_rs2_data),
    .imm(reg_imm),
    .ALUSrc(ALUSrc),
    .alu_out(alu_out)
);


always @(posedge clk) begin
    if(rst) begin // init
        reg_instr_read 	<= 1;
        reg_instr_addr 	<= 32'hfffffffc;
        reg_data_addr	<= 32'h00000000;
        reg_data_in		<= 32'h00000000;
        x[0] <= 0;
        state <= 0;
    end
    else begin
        case(state)
            3'd0: begin // IF
                reg_data_read  <= 0;
                reg_data_write <= 0;
                reg_instr_read <= 1;
                reg_instr_addr <= PC;
                x[0] <= 0;
            end
            3'd1: begin // ID
                reg_instr_read  <= 0;
                reg_rs1_data    <= x[rs1_addr < 32 ? rs1_addr : 0];
                reg_rs2_data    <= x[rs2_addr < 32 ? rs2_addr : 0];
                reg_imm         <= imm;
            end
            3'd2: begin // EX
                reg_alu_out <= alu_out;
            end
            3'd3: begin // MEM
                reg_data_write <= later_data_write;
                reg_data_read <= later_data_read;
                reg_data_addr <= reg_alu_out;
                reg_data_in <= reg_rs2_data;
                
            end
            3'd4: begin // WB
                if(RegWrite) begin
                    x[rd_addr] <= rd_data;
                end
            end
        endcase
        state <= (state + 1) % 5;
    end
end

endmodule
