`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:09:18 05/13/2012 
// Design Name: 
// Module Name:    ALU16 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module ALU16(S, A, B, M, CNb, F, PBo, GBo, CN16b, AEBo);
	
	input [15:0] A, B;
	input [3:0] S;
	input CNb, M; 
	output [15:0] F;
	output AEBo, PBo, GBo, CN16b;
	
	wire [3:0] AEB;
	
	wire [3:0] PB, GB;
	wire CNX, CNY, CNZ;
	Circuit74182 cla0(CNb, PB, GB, PBo, GBo, CNX, CNY, CNZ);
	
	wire [2:0] CN4;
	
	Circuit74181 alu0(S, A[ 3: 0], B[ 3: 0], M, CNb, F[ 3: 0], PB[0], GB[0], CN4[0], AEB[0]);
	Circuit74181 alu1(S, A[ 7: 4], B[ 7: 4], M, CNX, F[ 7: 4], PB[1], GB[1], CN4[1], AEB[1]);
	Circuit74181 alu2(S, A[11: 8], B[11: 8], M, CNY, F[11: 8], PB[2], GB[2], CN4[2], AEB[2]);
	Circuit74181 alu3(S, A[15:12], B[15:12], M, CNZ, F[15:12], PB[3], GB[3], CN16b, AEB[3]);

	assign AEBo = AEB[0] & AEB[1] & AEB[2] & AEB[3];
	
endmodule