function [Fcp,Fcs,Fs] = fselecthpf()

fprintf('Please enter all frequency values in Hz\n\n');
Fcp = input('Please provide a desired pass band cut off frequency: ');
t = input('Please provide a desired upward transition width in frequency: ');
Fcs = Fcp - t;
Fs = input('Please provide a desired sampling frequency: ');

end
    