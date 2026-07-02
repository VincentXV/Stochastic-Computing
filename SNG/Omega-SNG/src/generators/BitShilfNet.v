module BitShiftNet(
    input clk,
    input [3:0] in,
    output [3:0] out1
    output [3:0] out2
    output [3:0] out3
    output [3:0] out4
    );

    module 4_Shift shift1(
        .clk(clk),
        .in(in),
        .out(out1)
    );

    module 4_Shift shift2(
        .clk(clk),
        .in(out1),
        .out(out2)
    );

    module 4_Shift shift3(
        .clk(clk),
        .in(out2),
        .out(out3)
    );

    module 4_Shift shift4(
        .clk(clk),
        .in(out3),
        .out(out4)
    );
endmodule