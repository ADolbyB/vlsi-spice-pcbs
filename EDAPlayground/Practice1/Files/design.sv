// Code your design here

module practice1 (
  input a, b, c,
  output q
);
  
  wire aNandB, aNorB;
  
  assign aNandB = !(a & b);	// Intermediate connections
  assign aNorB = !(a | b);
  
  assign q = aNandB & aNorB & c;
  
endmodule