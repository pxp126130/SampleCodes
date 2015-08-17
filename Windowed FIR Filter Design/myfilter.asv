clear all;
close all;
clc;

e = input('Press enter to start this pakage:');
clc;
display('This program is a package that designs the following FIR Filters:');
fprintf('1. Low Pass Filter\n');
fprintf('2. High Pass Filter\n');
fprintf('3. Band Pass Filter\n');
fprintf('4. Band Stop Filter\n');
fprintf('The following windowing options have been provided:\n');
fprintf('A. Rectangular Window\n');
fprintf('B. Hamming Window\n');
fprintf('C. Kaiser Window\n\n');
fprintf('\nYour designing will begin shortly');
pause(2)
ch = menu('Select a Filter','Low Pass','High Pass','Band Pass','Band Stop');


if(ch == 1)

    fprintf('\nYou have selected a Low Pass filter\n');
    pause(2)
    [mag, phase,N,Fcp,Fcs,Fs] = lpfcode();
    
elseif(ch == 2)
    
    fprintf('\nYou have selected a High Pass filter\n');
    pause(2)
    [mag, phase,N,Fcp,Fcs,Fs] = hpfcode();
    
elseif(ch == 3)
    
    fprintf('\nYou have selected a Band Pass filter\n');
    pause(2)
    [mag,phase,N,Fcp1,Fcs1,Fcp2,Fcs2,Fs] = bpfcode();
    
elseif(ch == 4)
    
    fprintf('\nYou have selected a Band Stop filter\n');
    pause(2)
    [mag,phase,N,Fcp1,Fcs1,Fcp2,Fcs2,Fs] = bsfcode();

end



n = linspace(1,Fs,512);

subplot(2,1,1)
plot(n,mag,'r')
title('Magnitude Response');
xlabel('Frequency(Hz)');
ylabel('Magnitude');
axis([1,Fs,0,1.5]);

subplot(2,1,2)
plot(n,phase,'b')
title('Phase Response');
xlabel('Frequency(Hz)');
ylabel('Angle');