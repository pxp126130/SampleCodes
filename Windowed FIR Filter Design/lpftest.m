clear all;
clc;
close all;

N = 500;

% Fc = 10000;
Fcp = 8000;
Fcs = 9000;
Fs = 30000;

hl = lpf(N,Fcp,Fcs,Fs);
% hh = hpf(N,Fc,Fs);
% hp = bpf(N,Fcp,Fcs,Fs);
% hs = bsf(N,Fcp,Fcs,Fs);

fl = (freqz(hl));
[m,p] = freqz(hl);
% fh = abs(freqz(hh));
% fp = abs(freqz(hp));
% fs = abs(freqz(hs));

% plot(fl)
% figure
% plot(fh)
% figure
% plot(fp)
% figure
% plot(fs)
% 
% figure
% wvtool(fl)

m1 = abs(fl);
plot(m1)
figure
plot(m)