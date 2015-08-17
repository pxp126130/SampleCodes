function [Fcp,Fcs,Fs] = lpfvalidate()
    er = 1;
    while(er == 1)
        clc;
        [Fcp,Fcs,Fs] = fselectlpf();
        if(Fcp > Fcs)
            fprintf('\n\nPass Band cut off should be greater than Stop Band cut off\n');
            fprintf('Please re-enter');
            pause(5)   
        elseif(Fcs >= Fs/2)
            fprintf('\n\nStop Band cut off should be less than half the Sampling Frequency\n');
            fprintf('Please re-enter');
            pause(5)
        else
            er = 0;
        end
    end
end