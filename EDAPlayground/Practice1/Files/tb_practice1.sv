// Code your testbench here

module tb_practice1;

  // Clock Generation
  localparam CLK_PER = 10; // Clock period in units
  reg clk = 0;
  
  reg A, B, C;
  wire Q;

  // Instantiate the module under test
  practice1 p1 (
    .a(A),
    .b(B),
    .c(C),
    .q(Q)
  );

  always #((CLK_PER) / 2) clk = ~clk;

  initial begin

    A = 0; B = 0; C = 0; //Q = 0;	    // Initialize inputs

    #20 A = 0; B = 0; C = 0;
    #20 A = 0; B = 0; C = 1;
    #20 A = 0; B = 1; C = 0;
    #20 A = 0; B = 1; C = 1;
    #20 A = 1; B = 0; C = 0;
    #20 A = 1; B = 0; C = 1;
    #20 A = 1; B = 1; C = 0;
    #20 A = 1; B = 1; C = 1;
    #90 $finish;

  end

  always @(posedge clk) begin
    
    $display("Time = %0t \tA = %b \tB = %b \tC = %b \tQ = %b", $time, A, B, C, Q);

  end
  initial begin
    $dumpfile("tb_practice1.vcd"); 	// Extract waveforms from test bench to file.
    $dumpvars(0, tb_practice1);		// Extract Variable values to file.
  end

endmodule