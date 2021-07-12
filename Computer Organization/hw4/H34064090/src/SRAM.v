// Please DO NOT modify this file !!!

module SRAM(
    clk,
    rst,
    addr,
    read,
    write,
    DI,
    DO
);

parameter BYTES_SIZE     = 8;
parameter BYTES_CNT      = 4;
parameter WORD_SIZE      = BYTES_SIZE * BYTES_CNT;
parameter WORD_ADDR_BITS = 14;
parameter WORD_CNT       = 1 << WORD_ADDR_BITS;

input                       clk;
input                       rst;
input  [WORD_ADDR_BITS-1:0] addr;
input                       read;
input                       write;
input  [     WORD_SIZE-1:0] DI;
output [     WORD_SIZE-1:0] DO;

reg [BYTES_SIZE-1:0] Memory_byte3 [0:WORD_CNT-1];
reg [BYTES_SIZE-1:0] Memory_byte2 [0:WORD_CNT-1];
reg [BYTES_SIZE-1:0] Memory_byte1 [0:WORD_CNT-1];
reg [BYTES_SIZE-1:0] Memory_byte0 [0:WORD_CNT-1];

assign DO = { Memory_byte3[addr],
              Memory_byte2[addr],
              Memory_byte1[addr],
              Memory_byte0[addr] };

always @(posedge clk) begin
    if (write) begin
        Memory_byte3[addr] <= DI[31:24];
        Memory_byte2[addr] <= DI[23:16];
        Memory_byte1[addr] <= DI[15: 8];
        Memory_byte0[addr] <= DI[ 7: 0];
    end
end

endmodule
