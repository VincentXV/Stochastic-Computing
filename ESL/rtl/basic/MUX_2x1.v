module  MUX_2x1 (
    input in0,in1,
    output out,
    input sel
);
    assign out = sel ? in1 : in0;
endmodule