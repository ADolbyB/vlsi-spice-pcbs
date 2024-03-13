// Code your testbench here

module tb_practice2;

  // Clock Generation
  localparam CLK_PER = 10; // Clock period in units
  reg clk = 0;
  
  reg A, B, C, D;
  wire Q;

  // Instantiate the module under test
  practice2 p1 (
    .a(A),
    .b(B),
    .c(C),
    .d(D),
    .q(Q)
  );

  always #((CLK_PER) / 2) clk = ~clk;

  initial begin

    A = 0; B = 0; C = 0; D = 0;	    // Initialize inputs
    #20 A = 0; B = 0; C = 0; D = 0;
    #20 A = 0; B = 0; C = 0; D = 1;
    #20 A = 0; B = 0; C = 1; D = 0;
    #20 A = 0; B = 0; C = 1; D = 1;
    #20 A = 0; B = 1; C = 0; D = 0;
    #20 A = 0; B = 1; C = 0; D = 1;
    #20 A = 0; B = 1; C = 1; D = 0;
    #20 A = 0; B = 1; C = 1; D = 1;
    #20 A = 1; B = 0; C = 0; D = 0;
    #20 A = 1; B = 0; C = 0; D = 1;
    #20 A = 1; B = 0; C = 1; D = 0;
    #20 A = 1; B = 0; C = 1; D = 1;
    #20 A = 1; B = 1; C = 0; D = 0;
    #20 A = 1; B = 1; C = 0; D = 1;
    #20 A = 1; B = 1; C = 1; D = 0;
    #20 A = 1; B = 1; C = 1; D = 1;
    #40 $finish;					// Delay before end in time units.

  end

  always @(posedge clk) begin
    
    $display("Time = %0t \tA = %b \tB = %b \tC = %b \tD = %b \tQ = %b", $time, A, B, C, D, Q);

  end
  initial begin
    $dumpfile("tb_practice2.vcd"); 	// Extract waveforms from test bench to file.
    $dumpvars(0, tb_practice2);		// Extract Variable values to file.
  end

endmodule