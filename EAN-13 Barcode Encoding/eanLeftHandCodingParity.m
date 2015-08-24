function [parity] = eanLeftHandCodingParity(EANCode)

% This function returns the parity to be encoded for the Left Hand Encoding

    digit = EANCode(1);
    parity = zeros(1,6);

    switch digit
        case 0
            parity = [1,1,1,1,1,1];
        case 1
            parity = [1,1,2,1,2,2];
        case 2
            parity = [1,1,2,2,1,2];
        case 3
            parity = [1,1,2,2,2,1];
        case 4
            parity = [1,2,1,1,2,2];
        case 5
            parity = [1,2,2,1,1,2];
        case 6
            parity = [1,2,2,2,1,1];
        case 7
            parity = [1,2,1,2,1,2];
        case 8
            parity = [1,2,1,2,2,1];
        case 9
            parity = [1,2,2,1,2,1];
        otherwise
            display('Corrupt Entry. Retry');
    end
end

