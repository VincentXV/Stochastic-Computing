module BitShiftNet(
    input [3:0] in,
    output [3:0] out1, out2, out3, out4, out5, out6
    );

    Shift_4 shift1(
        .in(in),
        .out(out1)
    );

    Shift_4 shift2(
        .in(out1),
        .out(out2)
    );

    Shift_4 shift3(
        .in(out2),
        .out(out3)
    );

    Shift_4 shift4(
        .in(out3),
        .out(out4)
    );

    Shift_4 shift5(
        .in(out4),
        .out(out5)
    );

    Shift_4 shift6(
        .in(out5),
        .out(out6)
    );
endmodule