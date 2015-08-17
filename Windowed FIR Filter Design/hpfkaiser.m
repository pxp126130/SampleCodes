function [mag,phase,N] = hpfkaiser(Fcp,Fcs,Fs)

[w,N] = mykaiser(Fcp,Fcs,2*Fs);
hd = hpf(N,Fcp,Fcs,2*Fs);
h = hd.*w';
[f,phase] = freqz(h);
mag = abs(f);
    
end