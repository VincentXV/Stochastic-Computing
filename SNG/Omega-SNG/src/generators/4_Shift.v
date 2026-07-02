module 4_Shift(
    input clk,
    input [3:0] in,
    output [3:0] out
    );

    wire temp = out[3];

    always @(posedge clk)
        begin
        out[3] <= out[2];
        out[2] <= out[1];
        out[1] <= out[0];
        out[0] <= temp;
        end
endmodule