function [w,N] = mykaiser(Fcp,Fcs,Fs)

fcp = Fcp/Fs;
fcs = Fcs/Fs;
f = abs(fcp - fcs);
N = round(2.507/f);
if( mod(N,2) ~= 0 )
    N = N+1;
end
w = kaiser(N+1,5.1822);

end
