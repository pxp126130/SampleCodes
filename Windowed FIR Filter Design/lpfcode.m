function [mag,phase,N,Fcp,Fcs,Fs] = lpfcode()

    [Fcp,Fcs,Fs] = lpfvalidate();
    
    ch = menu('Select a Window','Rectangular','Hamming','Kaiser');
    if(ch == 1)
        
        [mag,phase,N] = lpfrect(Fcp,Fcs,Fs);

    elseif(ch == 2)

        [mag,phase,N] = lpfhamming(Fcp,Fcs,Fs);
        
    elseif(ch == 3)

        [mag,phase,N] = lpfkaiser(Fcp,Fcs,Fs);
        
    end
    
end