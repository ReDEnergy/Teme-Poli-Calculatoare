`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   22:21:53 05/13/2012
// Design Name:   ALU16
// Module Name:   /mnt/hgfs/Dropbox/ReDEnergy/CN/Tema2/test.v
// Project Name:  Tema2
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: ALU16
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module test;

	// Inputs
	reg [3:0] S;
	reg [15:0] A;
	reg [15:0] B;
	reg M;
	reg CNb;

	// Outputs
	wire [15:0] F;
	wire PBo;
	wire GBo;
	wire CN16b;
	wire AEBo;

	// Instantiate the Unit Under Test (UUT)
	ALU16 uut (
		.S(S), 
		.A(A), 
		.B(B), 
		.M(M), 
		.CNb(CNb), 
		.F(F), 
		.PBo(PBo), 
		.GBo(GBo), 
		.CN16b(CN16b), 
		.AEBo(AEBo)
	);

	initial begin
		// Initialize Inputs
		S = 0;
		A = 0;
		B = 0;
		M = 0;
		CNb = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here
		M=1;
		A=16'b11101010111001;
		B=16'b01001000110010;
		//    01000110000
		//    00010001011
		
		#30 S=4'b0110;
		
		#30 S=4'b1011;
		
		#30 M=0; CNb=1;
		S=4'd6;
		A=16'd12345;
		B=16'd11938;
		
		//#30 M=0; CNb=1;
    end

		initial begin
			//$monitor("time=",$time,, "A=%b B=%b S=%b M=%b => Sum=%b Cout=%b PG=%b GG=%b", A, B, S, M, F, CN16b, PBo, GBo);
			$monitor("time=",$time,, "A=%d B=%d S=%b M=%b => Sum=%d Cout=%b PG=%b GG=%b", A, B, S, M, F, CN16b, PBo, GBo);
		end   
      
endmodule

