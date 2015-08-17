function [mag,phase,N,Fcp1,Fcs1,Fcp2,Fcs2,Fs] = bsfcode()

    [Fcp1,Fcs1,Fcp2,Fcs2,Fs] = bsfvalidate();
    
    ch = menu('Select a Window','Rectangular','Hamming','Kaiser');
    if(ch == 1)
        
        [mag,phase,N] = bsfrect(Fcp1,Fcs1,Fcp2,Fcs2,Fs);

    elseif(ch == 2)

        [mag,phase,N] = bsfhamming(Fcp1,Fcs1,Fcp2,Fcs2,Fs);
        
    elseif(ch == 3)

        [mag,phase,N] = bsfkaiser(Fcp1,Fcs1,Fcp2,Fcs2,Fs);
        
    end
    
end