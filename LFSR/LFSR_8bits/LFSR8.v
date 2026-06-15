module LFSR8(
    input clk,
    output reg [7:0] LFSR = 8'b10001010
    );

    wire feedback = LFSR[0] ^ LFSR[1];

    always @(posedge clk)
        begin
        LFSR[0] <= LFSR[1];
        LFSR[1] <= LFSR[2];
        LFSR[2] <= LFSR[3];
        LFSR[3] <= LFSR[4];
        LFSR[4] <= LFSR[5];
        LFSR[5] <= LFSR[6];
        LFSR[6] <= LFSR[7];
        LFSR[7] <= feedback;
        end
endmodule
