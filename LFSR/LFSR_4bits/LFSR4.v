module LFSR4(
    input clk,
    output reg [3:0] LFSR = 4'b0010
    );

    wire feedback = LFSR[0] ^ LFSR[1];

    always @(posedge clk)
        begin
        LFSR[0] <= LFSR[1];
        LFSR[1] <= LFSR[2];
        LFSR[2] <= LFSR[3];
        LFSR[3] <= feedback;
        end
endmodule
