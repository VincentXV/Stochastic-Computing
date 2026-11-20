module LFSR#(
    parameter seed = 8'b10110001
)(
    input clk,
    input rst,
    output reg [7:0] LFSR,
    output wire [7:0] BLFSR
    );

    wire feedback = LFSR[7] ^ LFSR[6];

    always @(posedge clk) begin
        if (rst) // 
            LFSR <= seed;
        else
            LFSR[0] <= feedback;
            LFSR[1] <= LFSR[0];
            LFSR[2] <= LFSR[1];
            LFSR[3] <= LFSR[2];
            LFSR[4] <= LFSR[3];
            LFSR[5] <= LFSR[4];
            LFSR[6] <= LFSR[5];
            LFSR[7] <= LFSR[6];
    end
    assign BLFSR = ~LFSR;
endmodule