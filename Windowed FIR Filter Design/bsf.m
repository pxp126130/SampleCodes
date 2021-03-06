function[h] = bsf(N,Fcp1,Fcs1,Fcp2,Fcs2,Fs)

fcp1 = Fcp1/Fs;
fcs1 = Fcs1/Fs;
fc1 = (fcp1 + fcs1)/2;

fcp2 = Fcp2/Fs;
fcs2 = Fcs2/Fs;
fc2 = (fcp2 + fcs2)/2;

hp = lpf(N,Fcp1,Fcs1,Fs);
hs = hpf(N,Fcp2,Fcs2,Fs);

h = hp+hs;

h(N/2) = 1- 2 * (fc2 - fc1);

end