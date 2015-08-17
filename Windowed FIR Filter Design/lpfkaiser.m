function [mag,phase,N] = lpfkaiser(Fcp,Fcs,Fs)

[w,N] = mykaiser(Fcp,Fcs,2*Fs);
hd = lpf(N,Fcp,Fcs,2*Fs);
h = hd.*w';
[f,phase] = freqz(h);
mag = abs(f);
    
end