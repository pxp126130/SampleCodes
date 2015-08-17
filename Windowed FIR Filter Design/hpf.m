function [h] = hpf(N,Fcp,Fcs,Fs)

fcp = Fcp/Fs;
fcs = Fcs/Fs;
fc = (fcp + fcs)/2;

for i = 1 : N+1
    h(i) = -sin( 2 * pi * fc * ( (i) - N/2) )/(((i) - N/2) * pi );
end

h(N/2) = 1 - 2*fc;
end
    