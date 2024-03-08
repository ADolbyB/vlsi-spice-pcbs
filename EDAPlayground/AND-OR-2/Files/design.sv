// Code Verilog Here:
// Data Flow Model

module and2(a, b, y, z);
	input a, b;
  	output y, z;
  
  	assign y = a & b; // 2 input AND Gate
  	assign z = a | b; // 2 input OR Gate
  
endmodule