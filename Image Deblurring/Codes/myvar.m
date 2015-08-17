function [variance] = myvar(x)

[r c]=size(x);
xd=double(x);
m=mean(xd(:));
t=0;
for i = 1 : r
    for j = 1 : c
        t=t+(xd(i,j)-m)^2;
    end
end

variance=t/(r*c);


end

