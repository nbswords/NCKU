module Decoder(
    input     [6:0]    opcode,
    output            RegWrite,
    output    [1:0]    ALUOPCODE,
    output            PCtoRegSrc,
    output            ALUSrc,
    output            RDSrc,
    output            MemRead,
    output            MemWrite,
    output            MemtoReg
);
reg         reg_RegWrite;
reg [1:0]    reg_ALUOPCODE;
reg         reg_PCtoRegSrc;
reg         reg_ALUSrc;
reg         reg_RDSrc;
reg         reg_MemRead;
reg         reg_MemWrite;
reg         reg_MemtoReg;

assign RegWrite      = reg_RegWrite;
assign ALUOPCODE     = reg_ALUOPCODE;
assign PCtoRegSrc    = reg_PCtoRegSrc;
assign ALUSrc        = reg_ALUSrc;
assign RDSrc         = reg_RDSrc;
assign MemRead       = reg_MemRead;
assign MemWrite      = reg_MemWrite;
assign MemtoReg      = reg_MemtoReg;

always @(opcode) begin
    case(opcode)
        7'b0110011: begin // R-type
            reg_RegWrite    <= 1;
            reg_ALUOPCODE       <= 1; 
            reg_ALUSrc      <= 0; // R-type 0, else 1
            reg_RDSrc       <= 0; // from ALU 0, from pc_to_reg 1
            reg_MemRead     <= 0;
            reg_MemWrite    <= 0;
            reg_MemtoReg    <= 0;
            reg_PCtoRegSrc  <= 0; // PC+imm 0, PC+4 1
        end
        7'b0000011: begin // LW
            reg_RegWrite    <= 1;
            reg_ALUOPCODE       <= 2; 
            reg_ALUSrc      <= 1; // R-type 0, else 1
            reg_RDSrc       <= 0; // from ALU 0, from pc_to_reg 1
            reg_MemRead     <= 1;
            reg_MemWrite    <= 0;
            reg_MemtoReg    <= 1;
            reg_PCtoRegSrc  <= 0; // PC+imm 0, PC+4 1
        end
        7'b0010011: begin // I-type
            reg_RegWrite    <= 1;
            reg_ALUOPCODE       <= 0; 
            reg_ALUSrc      <= 1; // R-type 0, else 1
            reg_RDSrc       <= 0; // from ALU 0, from pc_to_reg 1
            reg_MemRead     <= 0;
            reg_MemWrite    <= 0;
            reg_MemtoReg    <= 0;
            reg_PCtoRegSrc  <= 0; // PC+imm 0, PC+4 1
        end
        7'b1100111: begin // JALR
            reg_RegWrite    <= 1;
            reg_ALUOPCODE       <= 0; 
            reg_ALUSrc      <= 1; // R-type 0, else 1
            reg_RDSrc       <= 1; // from ALU 0, from pc_to_reg 1
            reg_MemRead     <= 0;
            reg_MemWrite    <= 0;
            reg_MemtoReg    <= 0;
            reg_PCtoRegSrc  <= 1; // PC+imm 0, PC+4 1
        end
        7'b0100011: begin // SW
            reg_RegWrite    <= 0;
            reg_ALUOPCODE       <= 2; 
            reg_ALUSrc      <= 1; // R-type 0, else 1
            reg_RDSrc       <= 0; // from ALU 0, from pc_to_reg 1
            reg_MemRead     <= 0;
            reg_MemWrite    <= 1;
            reg_MemtoReg    <= 0;
            reg_PCtoRegSrc  <= 0; // PC+imm 0, PC+4 1
        end
        7'b1100011: begin // B-type
            reg_RegWrite    <= 0;
            reg_ALUOPCODE       <= 3; 
            reg_ALUSrc      <= 0; // R-type 0, else 1
            reg_RDSrc       <= 0; // from ALU 0, from pc_to_reg 1
            reg_MemRead     <= 0;
            reg_MemWrite    <= 0;
            reg_MemtoReg    <= 0;
            reg_PCtoRegSrc  <= 0; // PC+imm 0, PC+4 1        
        end
        7'b0010111: begin // AUIPC
            reg_RegWrite    <= 1;
            reg_ALUOPCODE       <= 0; 
            reg_ALUSrc      <= 1; // R-type 0, else 1
            reg_RDSrc       <= 1; // from ALU 0, from pc_to_reg 1
            reg_MemRead     <= 0;
            reg_MemWrite    <= 0;
            reg_MemtoReg    <= 0;
            reg_PCtoRegSrc  <= 0; // PC+imm 0, PC+4 1
        end
        7'b0110111: begin // LUI
            reg_RegWrite    <= 1;
            reg_ALUOPCODE       <= 0; 
            reg_ALUSrc      <= 1; // R-type 0, else 1
            reg_RDSrc       <= 1; // from ALU 0, from pc_to_reg 1
            reg_MemRead     <= 0;
            reg_MemWrite    <= 0;
            reg_MemtoReg    <= 0;
            reg_PCtoRegSrc  <= 0; // PC+imm 0, PC+4 1
        end
        7'b1101111: begin // JAL
            reg_RegWrite    <= 1;
            reg_ALUOPCODE       <= 0; 
            reg_ALUSrc      <= 1; // R-type 0, else 1
            reg_RDSrc       <= 1; // from ALU 0, from pc_to_reg 1
            reg_MemRead     <= 0;
            reg_MemWrite    <= 0;
            reg_MemtoReg    <= 0;
            reg_PCtoRegSrc  <= 1; // PC+imm 0, PC+4 1
        end
        default: begin end
    endcase
end

endmodule