function [Fcp,Fcs,Fs] = hpfvalidate()
    er = 1;
    while(er == 1)
        clc;
        [Fcp,Fcs,Fs] = fselecthpf();
        if(Fcp < Fcs)
            fprintf('\n\nPass Band cut off should be greater than Stop Band cut off\n');
            fprintf('Please re-enter');
            pause(5)
        elseif(Fcp >= Fs/2)
            fprintf('\n\Pass Band cut off should be less than half the Sampling Frequency\n');
            fprintf('Please re-enter');
            pause(5)
        else
            er = 0;
        end
    end
end