function [mser] = mse(x1,x2)

d1 = x1-x2;			% taking difference 
d2 = s1.^2;			% squaring each value
d3 = sum(d2(:));		% summing up to get mean square error
[r c] = size(x1);		

mser = d3/(r*c);		% normalizing the mean square error

end
