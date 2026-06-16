module WBG (
    input clk,
    input reg [3:0] bn = 4'b1011,
    output sn,
    output [3:0] lfsr_out
    );

    wire l3, l2, l1, l0;
    wire w3, w2, w1, w0;
    wire o3, o2, o1, o0;

    assign lfsr_out = {l3, l2, l1, l0};
    
    LFSR4 lfsr(clk, {l3, l2, l1, l0});
    
    assign w3 = l3;
    assign w2 = l2 & !l3;
    assign w1 = l1 & !l2 & !l3;
    assign w0 = l0 & !l1 & !l2 & !l3;

    assign o3 = w3 & bn[3];
    assign o2 = w2 & bn[2];
    assign o1 = w1 & bn[1];
    assign o0 = w0 & bn[0];

    assign sn = o3 | o2 | o1 | o0;
    
endmodule
