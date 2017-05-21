function HeartModel(path,tol)
	% 	path	director din care se incarca informatiile despre fiecare 
	%			sectiune a inimii
	%	tol 	toleranta maxima acceptata
	
	if nargin < 2
		eps = 0.01;
	end
	
	%	Se calculeaza pentru fiecare sectiune aria prin cele 2 metode 
	for i = 1 : 9	
		file = sprintf('%s/heart%d.dat',path,i);
		date = load(file);
		x = date (:,1);
		y = date (:,2);
		z(i) = date (1,3);
		ArieT(i) = Trapez (x, y);
		ArieM(i) = MonteCarlo (x, y, tol);
	end

	%	Afisare aria , metoda Trapezelor
	for i = 1 : 9
		printf('%.3f ',ArieT(i));
	end
	printf('\n');

	%	Afisare aria , metoda MonteCarlo
	for i = 1 : 9
		printf('%.3f ',ArieM(i));
	end
	printf('\n');

	%	Calculul Volumului utilizand ariile sectiunilor calculate 
	%	prin cele 2 metode
	Volum1 = abs(Trapez(z,ArieT));
	Volum2 = abs(Trapez(z,ArieM));

	%	Afisarea Volumului 
	printf ('%.3f %.3f\n', Volum1, Volum2);

end
