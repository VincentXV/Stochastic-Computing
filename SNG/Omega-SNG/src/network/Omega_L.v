module Omega_L (
    input [7:0] I,
    output [7:0] O,
    input [3:0] sel
);
    MUX_2x1 mux0 (.in0(I[0]), .in1(I[4]), .sel(sel[0]), .out(O[0]));
    MUX_2x1 mux1 (.in0(I[4]), .in1(I[0]), .sel(sel[0]), .out(O[1]));

    MUX_2x1 mux2 (.in0(I[1]), .in1(I[5]), .sel(sel[1]), .out(O[2]));
    MUX_2x1 mux3 (.in0(I[5]), .in1(I[1]), .sel(sel[1]), .out(O[3]));

    MUX_2x1 mux4 (.in0(I[2]), .in1(I[6]), .sel(sel[2]), .out(O[4]));
    MUX_2x1 mux5 (.in0(I[6]), .in1(I[2]), .sel(sel[2]), .out(O[5]));

    MUX_2x1 mux6 (.in0(I[3]), .in1(I[7]), .sel(sel[3]), .out(O[6]));
    MUX_2x1 mux7 (.in0(I[7]), .in1(I[3]), .sel(sel[3]), .out(O[7]));
endmodule