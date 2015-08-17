function [mag,phase,N,Fcp,Fcs,Fs] = hpfcode()

    [Fcp,Fcs,Fs] = hpfvalidate();
    
    ch = menu('Select a Window','Rectangular','Hamming','Kaiser');
    if(ch == 1)
        
        [mag,phase,N] = hpfrect(Fcp,Fcs,Fs);

    elseif(ch == 2)

        [mag,phase,N] = hpfhamming(Fcp,Fcs,Fs);
        
    elseif(ch == 3)

        [mag,phase,N] = hpfkaiser(Fcp,Fcs,Fs);
        
    end
    
end