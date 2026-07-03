module Shift_4(
    input clk,
    input [3:0] in,
    output [3:0] out
    );

    always @(*)
        begin
        out[3] = in[2];
        out[2] = in[1];
        out[1] = in[0];
        out[0] = in[3];
        end
endmodule