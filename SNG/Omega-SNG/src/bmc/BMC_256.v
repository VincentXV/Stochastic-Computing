module BMC_256 (
    input [7:0] B,
    output [255:0] U
);
    assign U[0] = B[0];
    assign U[2:1] = {2{B[1]}};
    assign U[6:3] = {4{B[2]}};
    assign U[14:7] = {8{B[3]}};
    assign U[30:15] = {16{B[4]}};
    assign U[62:31] = {32{B[5]}};
    assign U[126:63] = {64{B[6]}};
    assign U[255:127] = {128{B[7]}};
endmodule