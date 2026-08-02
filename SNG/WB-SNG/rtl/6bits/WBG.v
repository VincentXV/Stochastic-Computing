module WBG (
    input clk,
    input rst,
    input reg [5:0] bn,         // Binary Number = 44
    input wire [5:0] seed_in,    // Seed value for LFSR
    output sn
    );

    wire l5, l4, l3, l2, l1, l0;
    wire w5, w4, w3, w2, w1, w0;
    wire o5, o4, o3, o2, o1, o0;

    LFSR6 lfsr(clk, rst, seed_in, {l5, l4, l3, l2, l1, l0});

    assign w5 = l5;
    assign w4 = l4 & !l5;
    assign w3 = l3 & !l4 & !l5;
    assign w2 = l2 & !l3 & !l4 & !l5;
    assign w1 = l1 & !l2 & !l3 & !l4 & !l5;
    assign w0 = l0 & !l1 & !l2 & !l3 & !l4 & !l5;

    assign o5 = w5 & bn[5];
    assign o4 = w4 & bn[4];
    assign o3 = w3 & bn[3];
    assign o2 = w2 & bn[2];
    assign o1 = w1 & bn[1];
    assign o0 = w0 & bn[0];

    assign sn = o5 | o4 | o3 | o2 | o1 | o0;
    
endmodule
