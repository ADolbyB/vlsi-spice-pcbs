// Code your testbench here:

module tb_AND;
  
  reg A, B;
  wire Y, Z;
  
  // note the syntax: connect a to A, b to B, Y to y, etc.
  and2 a1 (.a(A), .b(B), .y(Y), .z(Z)); // instantiation: connect TB to Design

  initial begin
  	A = 1'b0; // Force 1 bit value of 0 to A
    B = 1'b0; // Force 1 bit value of 0 to B
    #45 $finish;
  end
  
  always #6 A = ~A;
  always #3 B = ~B;
  always @(Y, Z)
    
 	// Display time and variable values:
    $display("time = %0t \tINPUT VALUES: \tA = %b \tB = %b \tOutput Y = %b \tOutput Z = %b",$time,A,B,Y,Z); 
  initial begin
    $dumpfile("tb_AND.vcd"); 	// Extract waveforms from test bench to file.
    $dumpvars(0, tb_AND);		// Extract Variable values to file.
  end
endmodule