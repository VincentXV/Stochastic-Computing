module top(
    input [2:0] Binary,
    input clk,
    output [7:0] SN
);
    // BMC
    wire [7:0] U_SN;

    BMC_8 bmc_inst (
        .B(Binary),
        .U(U_SN)
    );

    // Bit Shift Network
    wire [3:0] sel_init, sel_1, sel_2, sel_3, sel_4, sel_5, sel_6;

    LFSR4 lfsr (
        .clk(clk),
        .LFSR(sel_init)
    );
    BitShiftNet shift_net (
        .in(sel_init),
        .out1(sel_1),
        .out2(sel_2),
        .out3(sel_3),
        .out4(sel_4),
        .out5(sel_5),
        .out6(sel_6)
    );

    // Omega-Flip Network
    wire [7:0] temp1, temp2, temp3, temp4, temp5;
    
    Omega_L omega1 (
        .sel(sel_1),
        .I(U_SN),
        .O(temp1)
    );

    Omega_L omega2 (
        .sel(sel_2),
        .I(temp1),
        .O(temp2)
    );

    Omega_L omega3 (
        .sel(sel_3),
        .I(temp2),
        .O(temp3)
    );

    Flip_L flip1 (
        .sel(sel_4),
        .I(temp3),
        .O(temp4)
    );

    Flip_L flip2 (
        .sel(sel_5),
        .I(temp4),
        .O(temp5)
    );

    Flip_L flip3 (
        .sel(sel_6),
        .I(temp5),
        .O(SN)
    );
endmodule