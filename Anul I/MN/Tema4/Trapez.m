function I = Trapez(x,y) 
	%	Functia calculeaza integrala(aria) unui poligon prin metoda trapezelor
	%	x 	abcisele punctelor
	%	y	ordonatele punctelor
	
	n = length(x);
	
	I = sum ( (y(1:n-1)+y(2:n)) .* (x(1:n-1)-x(2:n)) / 2 );

end
