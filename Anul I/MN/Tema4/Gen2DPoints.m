function [y,x] = Gen2DPoints( N, a, b, c, d )
	%	Aplicarea algoritmului Hammersley pentru generarea unui set de puncte
	%	distribuit uniform in domeniul [a,b]x[c,d]
	
	%	Generarea abciselor echidistante
	x = linspace( c, d, N );
	p = ceil (log2(N));
	%	Definirea punctelor 
	y = zeros (1, 2^p);
	
	for i=1:p
		y( 2^(i-1)+1: 2^i ) = y( 1:2^(i-1) ) + 2^(-i);
	end
	%	Punctele se aduc in domeniul D
	y = a + (b-a)*y;
	%	Se pastreaza doar un numar de N de puncte
	y = y(1:N);

end

