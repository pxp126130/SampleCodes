function [Fcp1,Fcs1,Fcp2,Fcs2,Fs] = fselectbsf()

fprintf('Please enter all frequency values in Hz\n\n');
Fcp1 = input('Please provide a desired lower pass band cut off frequency: ');
t1 = input('Please provide a desired downward transition width in frequency: ');
Fcs1 = Fcp1 + t1;
Fcp2 = input('Please provide a desired upper pass band cut off frequency: ');
t2 = input('Please provide a desired upward transition width in frequency: ');
Fcs2 = Fcp2 - t2;
Fs = input('Please provide a desired sampling frequency: ');

end
    