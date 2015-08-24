function leftCode = eanLeftHandEncoding(parity,eanCode)

% This code returns the bit pattern for the Left hand-side code based on
% the parity calculated from the eanLeftHandCodingParity function and the
% eanCode to be encoded itself.

    leftCode = [];

    ParityTable(1,:,1) = [0,0,0,1,1,0,1];
    ParityTable(1,:,2) = [0,0,1,1,0,0,1];
    ParityTable(1,:,3) = [0,0,1,0,0,1,1];
    ParityTable(1,:,4) = [0,1,1,1,1,0,1];
    ParityTable(1,:,5) = [0,1,0,0,0,1,1];
    ParityTable(1,:,6) = [0,1,1,0,0,0,1];
    ParityTable(1,:,7) = [0,1,0,1,1,1,1];
    ParityTable(1,:,8) = [0,1,1,1,0,1,1];
    ParityTable(1,:,9) = [0,1,1,0,1,1,1];
    ParityTable(1,:,10) = [0,0,0,1,0,1,1];

    ParityTable(2,:,1) = [0,1,0,0,1,1,1];
    ParityTable(2,:,2) = [0,1,1,0,0,1,1];
    ParityTable(2,:,3) = [0,0,1,1,0,1,1];
    ParityTable(2,:,4) = [0,1,0,0,0,0,1];
    ParityTable(2,:,5) = [0,0,1,1,1,0,1];
    ParityTable(2,:,6) = [0,1,1,1,0,0,1];
    ParityTable(2,:,7) = [0,0,0,0,1,0,1];
    ParityTable(2,:,8) = [0,0,1,0,0,0,1];
    ParityTable(2,:,9) = [0,0,0,1,0,0,1];
    ParityTable(2,:,10) = [0,0,1,0,1,1,1];


    left = eanCode(2:7);

    for i = 1 : length(parity)
        leftCode = [leftCode, ParityTable(parity(i),:,left(i)+1)];
    end
    
end

