function A = MonteCarlo( x, y, tol )
	%	Determinarea min si max a domeniului de puncte
	xmin = min(x);
	xmax = max(x);
	ymin = min(y);
	ymax = max(y);

	%	Distanta dintre min si max (lungimea de reprezentare a punctelor)	
	xdim = xmax - xmin;
	ydim = ymax - ymin;
	
	%	Definirea domeniului D	
	D = xdim * ydim;
	%	Aria initiala
	A = -inf;
	
	%	Lungimea 
	n  = length(x);
	
	%	Numarul de puncte initial
	N = 2^18;

	%	Determinarea numarului de puncte din interiorul poligonului
	%	Algoritmul se repeta pana cand toleranta dintre 2 calcule 
	%	consecutive este mai mica decat tol
	 	
	do 
		%	Dublarea numarului de puncte generata la fiecare iteratie
		N = 2 * N;
		%	Generare puncte
		[b,a] = Gen2DPoints( N, ymin, ymax, xmin, xmax );
		
		%	Numarul de puncte din interior
		Nint = zeros( 1, N, 'uint8' );
		
		for i = 2 : n
			%	Se salveaza coordonatele capetelor segmentului
			%	(x1,y1) si (x2,y2)
			x1 = x(i-1);
			y1 = y(i-1);
			x2 = x(i);		
			y2 = y(i);

			%	Orientarea segmentului
			if x2 > x1
				distx = x1 - x2;
				disty = y2 - y1;
			else
				distx = x2 - x1;
				disty = y1 - y2;
			end
				
			m = min ( x1, x2 );
			M = max ( x1, x2 );

			%	Verificare daca punctul este deasupra sau nu a segmentului
			s = ceil ( (m - xmin) / xdim * (N-1) + 1 );
			f = floor( (M - xmin) / xdim * (N-1) + 1 );

			pos = (( x1 - a(s:f) ) * disty + ( y1 - b(s:f) ) * distx) < 0;
			Nint(s:f) = bitxor( Nint(s:f), pos );

		end
		
		%	Se salveaza Aria precedenta
		Arie_prec = A;
		
		%	Calculul Ariei utilizand algoritmul MonteCarlo
		A = D * sum(Nint) / N;
		
		%	La i = 22 se opresete iteratia
		if (i == 22) 
			break;
		end
		
	until abs( A - Arie_prec ) < tol

end

