function [Fcp1,Fcs1,Fcp2,Fcs2,Fs] = bsfvalidate()
    
er = 1;
    while(er == 1)
        clc;
        [Fcp1,Fcs1,Fcp2,Fcs2,Fs] = fselectbsf();
        if(Fcp1 > Fcs1 || Fcp2 < Fcs2)
            fprintf('\n\nTransition width must be positive.\n');
            fprintf('Please re-enter');
            pause(5)
        elseif(Fcp2 >= Fs/2 || Fcs1 >= Fs/2)
            fprintf('\n\n Cut off should be less than half the Sampling Frequency\n');
            fprintf('Please re-enter');
            pause(5)
        else
            er = 0;
        end
    end

end