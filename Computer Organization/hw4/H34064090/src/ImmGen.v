module ImmGen(
     input     [31:0]     instr,
     input     [6:0]     opcode,
     output  [31:0]     imm
);
reg [31:0] imm_out;
assign imm = imm_out;

always @(instr) begin
     case (opcode)
          7'b0000011: begin
               imm_out[31:12] <= instr[31] ? -20'h1 : 0;
               imm_out[11:0] <= instr[31:20];
          end
          7'b0010011: begin // I-type
               imm_out[31:12] <= instr[31] ? -20'h1 : 0;
               imm_out[11:0] <= instr[31:20];
          end
          7'b1100111: begin // JALR
               imm_out[31:12] <= instr[31] ? -20'h1 : 0;
               imm_out[11:0] <= instr[31:20];
          end
          7'b0100011: begin // SW
               imm_out[31:12] <= instr[31] ? -20'h1 : 0;
               imm_out[11:5] <= instr[31:25];
               imm_out[4:0] <= instr[11:7];
          end
          7'b1100011: begin // B-type
               imm_out[31:13] <= instr[31] ? -19'h1 : 0;
               imm_out[12] <= instr[31];
               imm_out[10:5] <= instr[30:25];
               imm_out[4:1] <= instr[11:8];
               imm_out[11] <= instr[7];
               imm_out[0] <= 0;
          end
          7'b0010111: begin // AUIPC
               imm_out[31:12] <= instr[31:12];
               imm_out[11:0] <= instr[31] ? 0 : 0;
          end
          7'b0110111: begin // LUI
               imm_out[31:12] <= instr[31:12];
               imm_out[11:0] <= instr[31] ? 0 : 0;
          end
          7'b1101111: begin // JAL
               imm_out[31:21] <= instr[31] ? -11'h1 : 0;
               imm_out[20] <= instr[31];
               imm_out[10:1] <= instr[30:21];
               imm_out[11] <= instr[20];
               imm_out[19:12] <= instr[19:12];
               imm_out[0] <= 0;
          end
          default: imm_out[31:0] <= 0;
     endcase

end

endmodule