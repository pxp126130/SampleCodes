function [Fcp,Fcs,Fs] = fselectlpf()

fprintf('Please enter all frequency values in Hz\n\n');
Fcp = input('Please provide a desired pass band cut off frequency: ');
t = input('Please provide a desired downward transition width in frequency: ');
Fcs = Fcp + t;
Fs = input('Please provide a desired sampling frequency: ');

end
    