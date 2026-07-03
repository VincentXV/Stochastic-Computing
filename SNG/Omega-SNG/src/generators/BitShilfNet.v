module BitShiftNet(
    input clk,
    input [3:0] in,
    output [3:0] out1,
    output [3:0] out2,
    output [3:0] out3,
    output [3:0] out4
    );

    Shift_4 shift1(
        .clk(clk),
        .in(in),
        .out(out1)
    );

    Shift_4 shift2(
        .clk(clk),
        .in(out1),
        .out(out2)
    );

    Shift_4 shift3(
        .clk(clk),
        .in(out2),
        .out(out3)
    );

    Shift_4 shift4(
        .clk(clk),
        .in(out3),
        .out(out4)
    );
endmodule