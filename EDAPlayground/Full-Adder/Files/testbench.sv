// Code your testbench here:

module tb_FULLADD_4;
    localparam CLK_PER = 10;    // Clock period
    reg clk = 0; 			          // Initialize Clock signal to Zero
  	reg [3:0] X, Y;			        // Inputs
    reg CINtest;
    
    wire COUTtest;			        // Outputs
  	wire [3:0] SUMtest;

    always #((CLK_PER) / 2) clk = ~clk;   // Clock Generation

  	fulladd4 fa4 (  // Connect Test Bench to the Module Under Test
        .A4(X),
        .B4(Y), 
        .CINf(CINtest),
        .COUTf(COUTtest),
        .SUM4(SUMtest)
    );

    // Test Routine Start:
    initial begin
        X = 4'b0000; // Change values as neededSUMtest
        Y = 4'b0000;
        CINtest = 0;

        // Apply stimulus
        #10 X = 4'b0001; Y = 4'b0001; CINtest = 0;
      	#20 X = 4'b0001; Y = 4'b0000; CINtest = 1;
      	#30 X = 4'b0010; Y = 4'b0001; CINtest = 0;
        #40 X = 4'b0010; Y = 4'b0000; CINtest = 1;
      	#50 X = 4'b0010; Y = 4'b0001; CINtest = 1;
      	#60 X = 4'b1010; Y = 4'b0101; CINtest = 0;
      	#70 X = 4'b1010; Y = 4'b0101; CINtest = 1;
      	#80 $finish;
    end

    always @(posedge clk) begin
    	  $display("Time = %0t \tX = %b \tY = %b \tCIN = %b \tSUM = %b \tCOUT = %b", $time,X,Y,CINtest,SUMtest,COUTtest);
    end
  	initial begin
    	  $dumpfile("tb_FULLADD_4.vcd"); 	// Extract waveforms from test bench to file.
    	  $dumpvars(0, tb_FULLADD_4);		// Extract Variable values to file.
  	end
endmodule