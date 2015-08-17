function [w,N] = myhamming(Fcp,Fcs,Fs)

fcp = Fcp/Fs;
fcs = Fcs/Fs;
f = abs(fcp - fcs);
N = round(3.3/f);
if( mod(N,2) ~= 0 )
    N = N+1;
end
w = hamming(N+1);

end
