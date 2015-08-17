function [mag,phase,N] = hpfrect(Fcp,Fcs,Fs)

[w,N] = myrect(Fcp,Fcs,2*Fs);
hd = hpf(N,Fcp,Fcs,2*Fs);
h = hd.*w';
[f,phase] = freqz(h);
mag = abs(f);

end