module LFSR6(
    input clk,
    input rst,
    input wire [5:0] seed_in,
    output reg [5:0] LFSR 
    );

    wire feedback = LFSR[5] ^ LFSR[4];

    always @(posedge clk or posedge rst)
        begin
        if (rst)
            LFSR <= seed_in; // set or reset to initial value
        else
            begin
            LFSR[0] <= feedback;
            LFSR[1] <= LFSR[0];
            LFSR[2] <= LFSR[1];
            LFSR[3] <= LFSR[2];
            LFSR[4] <= LFSR[3];
            LFSR[5] <= LFSR[4];
            end
        end
endmodule
