function [mag,phase,N,Fcp1,Fcs1,Fcp2,Fcs2,Fs] = bpfcode()

    [Fcp1,Fcs1,Fcp2,Fcs2,Fs] = bpfvalidate();
    
    ch = menu('Select a Window','Rectangular','Hamming','Kaiser');
    if(ch == 1)
        
        [mag,phase,N] = bpfrect(Fcp1,Fcs1,Fcp2,Fcs2,Fs);

    elseif(ch == 2)

        [mag,phase,N] = bpfhamming(Fcp1,Fcs1,Fcp2,Fcs2,Fs);
        
    elseif(ch == 3)

        [mag,phase,N] = bpfkaiser(Fcp1,Fcs1,Fcp2,Fcs2,Fs);
        
    end
    
end