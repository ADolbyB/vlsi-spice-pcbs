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