`timescale 1ns/10ps
`include "fpadder.v"
`define CYCLE 10
`define GOLDEN_PATH "./golden.hex"
`define DATA_PATH   "./data.hex"

module top_tb;

reg         clk;
reg         rst;
reg  [31:0] cnt;
reg  [31:0] GOLDEN [0:65535];
reg  [63:0] DATA   [0:65535];
wire [31:0] src1;
wire [31:0] src2;
wire [31:0] out;
wire [31:0] ans;
reg  [31:0] err;

// Setup signal
initial begin
    clk <= 1'b0;
    rst <= 1'b0;
    #(`CYCLE / 10)
    rst <= 1'b1;
    #(`CYCLE)
    rst <= 1'b0;
end

always #(`CYCLE / 2) clk <= ~clk;

// Golden data
initial $readmemh(`DATA_PATH, DATA);
integer gf, num;
initial begin
    num = 0;
    gf = $fopen(`GOLDEN_PATH, "r");
    while (!$feof(gf)) begin
      $fscanf(gf, "%h\n", GOLDEN[num]);
      num = num + 1;
    end
    $fclose(gf);
end

always @(negedge clk) begin
    if (rst) cnt <= 32'b0;
    else     cnt <= cnt + 32'b1;
end

assign {src1, src2} = DATA[cnt];
assign ans          = GOLDEN[cnt];

assign fin = cnt == num;

always @(negedge clk) begin
    if (rst) begin
        err <= 32'b0;
    end
    else begin
        if (ans !== out) begin
            err <= err + 32'b1;
            $write("Error at %0dth cycle: output %8h != expect %8h\n", cnt, out, ans);
        end
    end
end

always @(negedge clk) begin
    if (fin) begin
        if(err) begin
            $display("\n");
            $display("====================================================================================");
            $display("-------------- (/`n`)/ ~#  There was %2d errors in your code !! ---------------------",err); 
            $display("--------- The simulation has finished with some error, Please check it !!! ---------"); 
            $display("====================================================================================");
            $display("\n");
        end 
        else begin
            $display("\n");
            $display("        ****************************               ");
            $display("        **                        **       /|__/|  ");
            $display("        **  Congratulations !!    **      / O,O  | ");
            $display("        **                        **    /_____   | ");
            $display("        **  Simulation PASS!!     **   /^ ^ ^ \\  |");
            $display("        **                        **  |^ ^ ^ ^ |w| ");
            $display("        *************** ************   \\m___m__|_|");                                                                                          
            $display("\n");
        end 
        $finish;
    end
end

fpadder DUV (
    .src1( src1 ),
    .src2( src2 ),
    .out ( out  ) 
);

`ifdef FSDB
initial begin
    $fsdbDumpfile("fpadder.fsdb");
    $fsdbDumpvars;
    $fsdbDumpMDA;
end
`endif

endmodule
