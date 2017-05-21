function [x,y] = loadxy(i) 

	file = sprintf('./heart_test/heart%d.dat',i);
	date = load(file);
	x = date (:,1);
	y = date (:,2);

end
