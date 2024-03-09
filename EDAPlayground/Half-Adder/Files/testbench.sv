// Code your testbench here:

module tb_HALFADD;
  
  reg A, B;
  wire SUM, CARRY;
  
  // note the syntax: connect a to A, b to B, Y to y, etc.
  halfadd1 ha1 (.a(A), .b(B), .sum(SUM), .carry(CARRY)); // instantiation: connect TB to Design

  initial begin
  	A = 1'b0; // Force 1 bit value of 0 to A
    B = 1'b0; // Force 1 bit value of 0 to B
    #40 $finish;
  end
  
  always #2 A = ~A;
  always #4 B = ~B;
  always @(SUM, CARRY)
    
 	// Display time and variable values:
    $display("time = %0t \tINPUT VALUES: \tA = %b \tB = %b \tOutput SUM = %b \tOutput CARRY = %b",$time,A,B,SUM,CARRY); 
  initial begin
    $dumpfile("tb_HALFADD.vcd"); 	// Extract waveforms from test bench to file.
    $dumpvars(0, tb_HALFADD);		// Extract Variable values to file.
  end
endmodule