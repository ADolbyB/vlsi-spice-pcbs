// Code Verilog Here:
// Data Flow Model

module andor3(a, b, c, y, z);
  input a, b, c;
  output y, z;
  
  assign y = a & b & c;
  assign z = a | b | c;
  
endmodule