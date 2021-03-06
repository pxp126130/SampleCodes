function [mag,phase,N] = bsfhamming(Fcp1,Fcs1,Fcp2,Fcs2,Fs)

[wa,Na] = myhamming(Fcp1,Fcs1,2*Fs);
[wb,Nb] = myhamming(Fcp2,Fcs2,2*Fs);

if(Na > Nb)
    w = wa;
    N = Na;
else
    w = wb;
    N = Nb;
end

hd = bsf(N,Fcp1,Fcs1,Fcp2,Fcs2,2*Fs);
h = hd.*w';
[f,phase] = freqz(h);
mag = abs(f);
    
end