module BMC_8 (
    input [2:0] B,
    output [7:0] U
);
    assign U[0] = B[0];
    assign U[2:1] = {2{B[1]}};
    assign U[6:3] = {4{B[2]}};
endmodule