module BSNG (
    input clk,
    input reg [4:1] bn,
    output sn
    );

    wire l4, l3, l2, l1;
    wire bl4, bl3, bl2, bl1;

    wire cmp, nor_res, nand_res, or_res, and_res;

    LFSR4 lfsr( .clk(clk), .LFSR({l4, l3, l2, l1}), .BLFSR({bl4, bl3, bl2, bl1}) );

    assign cmp = {bn[3], bn[2], bn[1]} >= {l3, l2, l1};
    assign nor_res = ~(bl4 | bn[4]);
    assign or_res = cmp | nor_res;
    assign nand_res = ~(bl4 & bn[4]);
    assign and_res = or_res & nand_res;

    assign sn = and_res;

endmodule