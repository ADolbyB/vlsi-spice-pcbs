// Code your design here

module practice2 (
  input a, b, c, d,
  output q
);
  
  wire aAndBorNotc;
  assign aAndBorNotC = (a & b) | (!c); // Intermediate connection
  
  assign q = !(aAndBorNotC & d);
  
endmodule