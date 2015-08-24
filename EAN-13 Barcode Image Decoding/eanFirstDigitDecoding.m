function [digit] = eanFirstDigitDecoding(parity)


    if(parity == [1,1,1,1,1,1])
            digit = 0;
    elseif(parity == [1,1,2,1,2,2])
            digit = 1;
    elseif(parity == [1,1,2,2,1,2])
            digit = 2;
    elseif(parity == [1,1,2,2,2,1])
            digit = 3;
    elseif(parity == [1,2,1,1,2,2])
            digit = 4; 
    elseif(parity == [1,2,2,1,1,2])
            digit = 5;
    elseif(parity == [1,2,2,2,1,1])
            digit = 6;
    elseif(parity ==  [1,2,1,2,1,2])
            digit = 7;
    elseif(parity == [1,2,1,2,2,1])
            digit = 8;
    elseif(parity == [1,2,2,1,2,1])
            digit = 9;
    else
        display('Corrupt Entry. Retry');
    end


end