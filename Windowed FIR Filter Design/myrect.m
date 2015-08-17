function [w,N] = myrect(Fcp,Fcs,Fs)

fcp = Fcp/Fs;
fcs = Fcs/Fs;
f = abs(fcp - fcs);
N = round(0.9/f);
if( mod(N,2) ~= 0 )
    N = N+1;
end
w = rectwin(N+1);

end
