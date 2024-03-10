// Code Verilog Here:
// Data Flow Model

module fulladd1 (
    input A,
    input B,
    input CIN,
    output SUM,
    output COUT 
);

    assign SUM = A ^ B ^ CIN;
  	assign COUT = (A & B) | (CIN & (A ^ B));

endmodule

// connect 4 of the single bit adders
module fulladd4( 
    input [3:0] A4,
    input [3:0] B4,
    input CINf,
    output COUTf,
    output [3:0] SUM4
);

    wire [3:0] C4;

    // Instantiate 4 single bit adders for a 4 bit adder
    fulladd1 FA0( .A(A4[0]), .B(B4[0]), .CIN(CINf), .COUT(C4[0]), .SUM(SUM4[0]));
    fulladd1 FA1( .A(A4[1]), .B(B4[1]), .CIN(C4[0]), .COUT(C4[1]), .SUM(SUM4[1]));
    fulladd1 FA2( .A(A4[2]), .B(B4[2]), .CIN(C4[1]), .COUT(C4[2]), .SUM(SUM4[2]));
    fulladd1 FA3( .A(A4[3]), .B(B4[3]), .CIN(C4[2]), .COUT(C4[3]), .SUM(SUM4[3]));

    assign COUTf = C4[3];

endmodule