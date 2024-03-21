// Code your testbench here:

module tb_FULLADD_1;

    localparam CLK_PER = 10; // Clock period
    reg clk = 0; 			       // Initialize Clock signal to Zero
  	reg X, Y;    			       // Inputs
    reg CINtest;
    
    wire COUTtest;			     // Outputs
  	wire SUMtest;

  	// Clock Generation
    always #((CLK_PER) / 2) clk = ~clk;

    // Connect Test Bench to the Module Under Test
  	fulladd1 fa1 (
        .A(X),
        .B(Y), 
        .CIN(CINtest),
        .COUT(COUTtest),
        .SUM(SUMtest)
    );

    // Test Routine Start:
    initial begin
        X = 1'b0;             // Change values as needed
        Y = 1'b0;
        CINtest = 0;

        // Apply stimulus
        #10 X = 1'b0; Y = 1'b0; CINtest = 0;
      	#10 X = 1'b0; Y = 1'b0; CINtest = 1;
      	#10 X = 1'b0; Y = 1'b1; CINtest = 0;
        #10 X = 1'b0; Y = 1'b1; CINtest = 1;
      	#10 X = 1'b1; Y = 1'b0; CINtest = 0;
      	#10 X = 1'b1; Y = 1'b0; CINtest = 1;
      	#10 X = 1'b1; Y = 1'b1; CINtest = 0;
        #10 X = 1'b1; Y = 1'b1; CINtest = 1;
      	#10 $finish;
    end

    always @(posedge clk) begin
    	  $display("Time = %0t \tX = %b \tY = %b \tCIN = %b \tSUM = %b \tCOUT = %b", $time,X,Y,CINtest,SUMtest,COUTtest);
    end
  	initial begin
      	$dumpfile("tb_FULLADD_1.vcd"); 	// Extract waveforms from test bench to file.
      	$dumpvars(0, tb_FULLADD_1);		  // Extract Variable values to file.
  	end
endmodule