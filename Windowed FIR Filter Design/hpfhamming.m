function [mag,phase,N] = hpfhamming(Fcp,Fcs,Fs)

[w,N] = myhamming(Fcp,Fcs,2*Fs);
hd = hpf(N,Fcp,Fcs,2*Fs);
h = hd.*w';
[f,phase] = freqz(h);
mag = abs(f);

end