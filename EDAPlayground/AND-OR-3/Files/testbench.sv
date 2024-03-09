// Code your testbench here:

module tb_ANDOR_3;
  
  reg A, B, C;
  wire Y, Z;
  
  // note the syntax: connect a to A, b to B, Y to y, etc.
  andor3 ao1 (.a(A), .b(B), .c(C), .y(Y), .z(Z)); // instantiation: connect TB to Design

  initial begin
  	A = 1'b0; // Force 1 bit value of 0 to A
    B = 1'b0; // Force 1 bit value of 0 to B
    C = 1'b0; //
    #50 $finish;
  end
  
  always #2 A = ~A;
  always #4 B = ~B;
  always #6 C = ~C;
  always @(Y, Z)
    
 	// Display time and variable values:
    $display("time = %0t \tINPUT VALUES: \tA = %b \tB = %b \tC = %b \tOutput Y = %b \tOutput Z = %b",$time,A,B,C,Y,Z); 
  initial begin
    $dumpfile("tb_ANDOR_3.vcd"); 	// Extract waveforms from test bench to file.
    $dumpvars(0, tb_ANDOR_3);		// Extract Variable values to file.
  end
endmodule